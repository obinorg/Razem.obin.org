<?php
// $Id: security_review.checks.inc,v 1.1.2.5 2009/12/08 21:53:05 coltrane Exp $

/**
 * @file
 * Checks for security_review module.
 *
 */

/**
 * Check that files aren't writeable by the server.
 */
function security_review_check_file_perms() {
  $result = TRUE;
  // Extract ending folder for file directory path.
  $file_path = rtrim(file_directory_path(), '/');
  $file_path = substr($file_path, strrpos($file_path, '/') + 1);
  // Set files to ignore.
  $ignore = array('..', 'CVS', $file_path);
  $files = _security_review_check_file_perms_scan('.', $ignore);
  if (count($files)) {
    $result = FALSE;
  }
  return array('result' => $result, 'value' => $files);
}

function _security_review_check_file_perms_scan($directory, $ignore) {
  $items = array();
  if ($handle = opendir($directory)) {
    while (($file = readdir($handle)) !== FALSE) {
      // Don't check hidden files or ones we said to ignore.
      if ($file[0] != "." && !in_array($file, $ignore)) {
        $file = $directory . "/" . $file;
        if (is_dir($file)) {
          $items = array_merge($items, _security_review_check_file_perms_scan($file, $ignore));
          if (is_writable( $file)) {
            $items[] = preg_replace("/\/\//si", "/", $file);
          }
        }
        elseif (is_writable( $file)) {
          $items[] = preg_replace("/\/\//si", "/", $file);
        }
      }
    }
    closedir($handle);
  }
  return $items;
}

function security_review_check_file_perms_help() {
  $element['title'] =  t("Web server file system permissions");
  $element['descriptions'][] = t("It is dangerous to allow the web server to write to files inside the document root of your server. Doing so would allow Drupal to write files that could then be executed. An attacker might use such a vulnerability to take control of your site. An exception is the files directory which Drupal needs permission to write to in order to provide features like file attachments.");
  $element['descriptions'][] = t("<a href='!link'>Read more about file system permissions in the handbooks.</a>", array('!link' => url('http://drupal.org/node/244924')));

  $last_check = security_review_get_last_check('security_review', 'file_perms');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $return = security_review_check_file_perms();
    $element['findings']['descriptions'][] = t('It is recommended that the following files or directories be corrected.');
    foreach ($return['value'] as $file) {
      $element['findings']['items'][] = check_plain($file);
    }
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

/**
 * Check for formats that do not have HTML filter that can be used by untrusted users.
 */
function security_review_check_input_formats() {
  $result = TRUE;
  $formats = filter_formats();
  $check_return_value = array();
  // Check formats that are accessible by untrusted users.
  $untrusted_roles = security_review_untrusted_roles();
  foreach ($formats as $id => $format) {
    $format_roles = array_filter(explode(',', $format->roles));
    $intersect = array_intersect($format_roles, $untrusted_roles);
    if (!empty($intersect)) {
      // Untrusted users can use this format.
      $filters = filter_list_format($format->format);
      // Check format for HTML filter.
      if (in_array('filter/0', array_keys($filters))) {
        // Using HTML Filter, good! Now check allowed tags if the filter is stripping instead of escaping.
        $setting = variable_get("filter_html_" . $format->format, FILTER_HTML_STRIP);
        if ($setting == FILTER_HTML_STRIP) {
          // Check for unsafe tags in allowed tags.
          $allowed_tags = variable_get("allowed_html_" . $format->format, '');
          $unsafe_tags = security_review_unsafe_tags();
          foreach ($unsafe_tags as $tag) {
            if (strpos($allowed_tags, '<' . $tag . '>') !== FALSE) {
              // Found an unsafe tag
              $check_return_value['tags'][$id] = $tag;
            }
          }
        }
      }
      else {
        // Format is usable by untrusted users but does not contain the HTML Filter.
        $check_return_value['formats'][$id] = $format->name;
      }
    }
  }

  if (!empty($check_return_value)) {
    $result = FALSE;
  }
  return array('result' => $result, 'value' => $check_return_value);
}

function security_review_check_input_formats_help() {
  $element['title'] = t('Allowed HTML tags in input formats');
  $element['descriptions'][] = t("Certain HTML tags can allow an attacker to take control of your site. Drupal's input format system makes use of a set filters to run on incoming text. The 'HTML Filter' strips out harmful tags and Javascript events and should be used on all formats accessible by untrusted users.");
  $element['descriptions'][] = t("<a href='!link'>Read more about Drupal's input formats in the handbooks.</a>", array('!link' => url('http://drupal.org/node/224921')));
  $last_check = security_review_get_last_check('security_review', 'input_formats');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $return = security_review_check_input_formats();
    if (!empty($return['value']['tags'])) {
      $element['findings']['descriptions'][] = t('<a href="!link">Review your input formats.</a>', array('!link' => url('admin/settings/filters')));
      $element['findings']['descriptions'][] = t('It is recommended you remove the following tags from roles accessible by untrusted users.');
      foreach ($return['value']['tags'] as $tag) {
        $element['findings']['items'][] = $tag; // Tag doesn't need filtering cause it's not user-defined.
      }
    }
    elseif (!empty($return['value']['formats'])) {
      $element['findings']['descriptions'][] = t('The following formats are usable by untrusted roles and do not filter allowed HTML tags.');
      foreach ($return['value']['formats'] as $id => $name) {
        $element['findings']['items'][] = l($name, 'admin/settings/filters/' . $id);
      }
    }
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

function security_review_check_error_reporting() {
  $error_level = variable_get('error_level', NULL);
  if (is_null($error_level) || intval($error_level) == 1) {
    // When the variable isn't set, or its set to 1 errors are printed to the screen.
    $result = FALSE;
  }
  else {
    $result = TRUE;
  }
  return array('result' => $result);
}

function security_review_check_error_reporting_help() {
  $element['title'] = t('Error reporting');
  $element['descriptions'][] = t('As a form of hardening your site you should avoid information disclosure. Drupal by default prints errors to the screen and writes them to the log. Error messages disclose the full path to the file where the error occured.');
  $return = security_review_check_error_reporting();
  if ($return['result'] === FALSE) {
    $element['findings']['descriptions'][] = t('You have error reporting set to both the screen and the log.');
    $element['findings']['descriptions'][] = t('<a href="!link">Alter error reporting settings.</a>', array('!link' => url('admin/settings/error-reporting')));
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

/**
 * If private files is enabled check that the directory is not under the web root.
 *
 * There is ample room for the user to get around this check. @TODO get more sophisticated?
 */
function security_review_check_private_files() {
  $file_downloads = variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC);
  if ($file_downloads == FILE_DOWNLOADS_PRIVATE) {
    $file_directory_path = file_directory_path();
    if (strpos($file_directory_path, '/') === 0) {
      // Path begins at root.
      $result = TRUE;
    }
    elseif (strpos($file_directory_path, '../') === 0) {
      // Path begins by moving up the system.
      $result = FALSE;
    }
    else {
      // Directory is relative (or crafty).
      $result = FALSE;
    }
  }
  else {
    return NULL;
  }
  return array('result' => $result);
}

function security_review_check_private_files_help() {
  $element['title'] = t('Private files');
  $element['descriptions'][] = t("If you have Drupal's private files feature enabled you should move the files directory outside of the web server's document root. While Drupal will control serving files when requested by way of content if a user knows the actual system path they can circumvent Drupal's private files feature. You can protect against this by specifying a files directory outside of the webserver root.");
  $last_check = security_review_get_last_check('security_review', 'private_files');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $element['findings']['descriptions'][] = t('Your files directory is not outside of the server root.');
    $element['findings']['descriptions'][] = t('<a href="!link">Edit the files directory path.</a>', array('!link' => url('admin/settings/file-system')));
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

/**
 * Check for unsafe allowed extensions.
 */
function security_review_check_upload_extensions() {
  $result = TRUE;
  $check_return_value = array();
  if (module_exists('upload')) {
    // First, check if any unsafe extensions are even allowed.
    $extensions = variable_get('upload_extensions_default', NULL);
    if (!is_null($extensions)) {
      $unsafe_extensions = security_review_unsafe_extensions();
      foreach ($unsafe_extensions as $unsafe_extension) {
        if (strpos($extensions, $unsafe_extension) !== FALSE) {
          // Found an unsafe extension.
          $check_return_value[] = $unsafe_extension;
        }
      }
    }
    if (!empty($check_return_value)) {
      $result = FALSE;
    }
  }
  else {
    return NULL;
  }
  return array('result' => $result, 'value' => $check_return_value);
}

function security_review_check_upload_extensions_help() {
  $element['title'] = t('Allowed upload extensions');
  $element['descriptions'][] = t("The upload module allows users to attach files to content. Some extensions are considered dangerous because the files can be evaluated and then executued in the browser. A malicious user could use this opening to gain control of your site.");
  $last_check = security_review_get_last_check('security_review', 'upload_extensions');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $return = security_review_check_upload_extensions();
    $element['findings']['descriptions'][] = t('<a href="!link">Alter file upload settings.</a>', array('!link' => url('admin/settings/uploads')));
    $element['findings']['descriptions'][] = t('The following extensions are considered unsafe and should be removed or limited from use. Or, be sure you are not granting untrusted users the ability to upload files.');
    foreach ($return['value'] as $extension) {
      $element['findings']['items'][] = $extension;
    }
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

function security_review_check_query_errors($last_check = NULL) {
  $timestamp = NULL;
  $check_result_value = array();
  $sql = "SELECT message, hostname FROM {watchdog} WHERE type = 'php' AND severity = %d";
  if (!is_null($last_check)) {
    $sql .= " AND timestamp >= %d";
    $timestamp = $last_check['lastrun'];
  }
  $results = db_query($sql, WATCHDOG_ERROR, $timestamp);
  while ($row = db_fetch_array($results)) {
    if (strpos($row['message'], 'SELECT') !== FALSE) {
      $entries[$row['hostname']][] = $row;
    }
  }
  $result = TRUE;
  if (!empty($entries)) {
    foreach ($entries as $ip => $records) {
      if (count($records) > 10) {
        $check_result_value[] = $ip;
      }
    }
  }
  if (!empty($check_result_value)) {
    $result = FALSE;
  }
  else {
    // Rather than worrying the user about the idea of query errors we skip reporting a pass.
    return NULL;
  }
  return array('result' => $result, 'value' => $check_result_value);
}

function security_review_check_query_errors_help() {
  $element['title'] = t('Abundant query errors from the same IP');
  $element['descriptions'][] = t("Database errors triggered from the same IP may be an artifact of a malicious user attempting to probe the system for weaknesses like SQL injection or information disclosure.");
  $last_check = security_review_get_last_check('security_review', 'query_errors');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $element['findings']['descriptions'][] = t('The following IPs were observed with an abundance of query errors.');
    $return = security_review_check_query_errors();
    foreach ($return['value'] as $ip) {
      $element['findings']['items'][] = check_plain($ip);
    }
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

function security_review_check_failed_logins($last_check = NULL) {
  $result = TRUE;
  $timestamp = NULL;
  $check_result_value = array();
  $sql = "SELECT message, hostname FROM {watchdog} WHERE type = 'user' AND severity = %d";
  if (!is_null($last_check)) {
    $sql .= " AND timestamp >= %d";
    $timestamp = $last_check['lastrun'];
  }
  $results = db_query($sql, WATCHDOG_NOTICE, $timestamp);
  while ($row = db_fetch_array($results)) {
    if (strpos($row['message'], 'Login attempt failed') !== FALSE) {
      $entries[$row['hostname']][] = $row;
    }
  }
  if (!empty($entries)) {
    foreach ($entries as $ip => $records) {
      if (count($records) > 10) {
        $check_result_value[] = $ip;
      }
    }
  }
  if (!empty($check_result_value)) {
    $result = FALSE;
  }
  else {
    // Rather than worrying the user about the idea of failed logins we skip reporting a pass.
    return NULL;
  }
  return array('result' => $result, 'value' => $check_result_value);
}

function security_review_check_failed_logins_help() {
  $element['title'] = t('Abundant failed logins from the same IP');
  $element['descriptions'][] = t("Failed login attempts from the same IP may be an artifact of a malicous user attempting to brute-force their way onto your site as an authenticated user to carry out nefarious deeds. ");

  $last_check = security_review_get_last_check('security_review', 'failed_logins');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $element['findings']['descriptions'][] = t('The following IPs were observed with an abundanced of failed login attempts.');
    $return = security_review_check_failed_logins();
    foreach ($return['value'] as $ip) {
      $element['findings']['items'][] = check_plain($ip);
    }
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

/**
 * Look for admin permissions granted to untrusted roles.
 */
function security_review_check_permissions() {
  $result = TRUE;
  $check_result_value = array();
  $untrusted_roles = security_review_untrusted_roles();
  $admin_perms = security_review_admin_permissions();
  foreach ($untrusted_roles as $rid) {
    $perms = array();
    $results = db_fetch_array(db_query('SELECT r.rid, p.perm FROM {role} r LEFT JOIN {permission} p ON r.rid = p.rid WHERE r.rid = %d', $rid));
    if ($results !== FALSE) {
      $perms = explode(',', str_replace(', ', ',', $results['perm']));
      $intersect = array_intersect($perms, $admin_perms);
      if (!empty($intersect)) {
        $check_result_value[$rid] = $intersect;
      }
    }
  }
  if (!empty($check_result_value)) {
    $result = FALSE;
  }
  return array('result' => $result, 'value' => $check_result_value);
}

function security_review_check_permissions_help() {
  $element['title'] = t('Permissions');
  $element['descriptions'][] = t("Drupal's permission system is extensive and allows for varying degrees of control. Certain permissions would allow a user total control, or the ability to escalate their control, over your site and should only be granted to trusted users.");
  $element['descriptions'][] = t('<a href="!link">Read more about trusted vs. untrusted roles and permissions on CrackingDrupal.com.</a>', array('!link' => url('http://crackingdrupal.com/blog/ben-jeavons/importance-user-roles-and-permissions-site-security')));
  $last_check = security_review_get_last_check('security_review', 'permissions');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $return = security_review_check_permissions();
    $element['findings']['descriptions'][] = t('You have granted untrusted roles the following administrative permissions that you should revoke.');
    foreach ($return['value'] as $rid => $permissions) {
      $role = db_fetch_array(db_query("SELECT name FROM {role} WHERE rid = %d", $rid));
      $item = t('<a href="!link">@name</a> has %permissions', array('!link' => url('admin/user/permissions/' . $rid), '@name' => $role['name'], '%permissions' => implode(', ', $permissions)));
      $element['findings']['items'][] = $item;
    }
  }
  $output = theme('security_review_check_help', $element);
  return $output;
}

function security_review_check_nodes($last_check = NULL) {
  $result = TRUE;
  $check_result_value = array();
  $timestamp = NULL;
  $sql = "SELECT n.nid FROM {node} n INNER JOIN {node_revisions} r ON n.vid = r.vid WHERE r.body LIKE '%<script%' OR r.body LIKE '%<?php%'";
  if (!is_null($last_check)) {
    $sql .= " AND n.changed >= %d";
    $timestamp = $last_check['lastrun'];
  }
  $results = pager_query($sql, 50, 0, NULL, $timestamp);
  while ($row = db_fetch_array($results)) {
    $check_result_value[] = $row['nid'];
  }
  if (!empty($check_result_value)) {
    $result = FALSE;
  }

  return array('result' => $result, 'value' => $check_result_value);
}

function security_review_check_nodes_help() {
  $element['title'] = t('Dangerous tags in nodes');
  $element['descriptions'][] = t("Script and PHP code in the body of nodes does not align with Drupal best practices and may be a vulnerability if an untrusted user is allowed to edit such content. It is recommended you remove such content from the body of nodes.");

  $last_check = security_review_get_last_check('security_review', 'nodes');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $element['findings']['descriptions'][] = t('The following nodes potentially have dangerous tags. The links go to the edit page.');
    $return = security_review_check_nodes(); // Don't pass $last_check because timestamp is wrong now.
    foreach ($return['value'] as $nid) {
      // There is no access checking. We state that the use of this module should be granted to trusted users only.
      $node = node_load($nid);
      $element['findings']['items'][][] = l($node->title, 'node/' . $node->nid . '/edit');
    }
    $element['findings']['pager'] = theme('pager', NULL, 50);
  }

  $output = theme('security_review_check_help', $element);
  return $output;
}

function security_review_check_comments($last_check = NULL) {
  $result = TRUE;
  $check_result_value = array();
  $timestamp = NULL;
  if (module_exists('comment')) {
    $sql = "SELECT nid, cid FROM {comments} WHERE comment LIKE '%<script%' OR comment LIKE '%<?php%'";
    if (!is_null($last_check)) {
      $sql .= " AND timestamp >= %d";
      $timestamp = $last_check['lastrun'];
    }
    $results = pager_query($sql, 20, 0, NULL, $timestamp);
    while ($row = db_fetch_array($results)) {
      $check_result_value[$row['cid']] = $row['nid'];
    }
    if (!empty($check_result_value)) {
      $result = FALSE;
    }
  }
  else {
    return NULL;
  }
  return array('result' => $result, 'value' => $check_result_value);
}

function security_review_check_comments_help() {
  $element['title'] = t('Dangerous tags in comments');
  $element['descriptions'][] = t("There is little reason for script and PHP tags to be in comments (unless they are code examples) and could be in use maliciously.");

  $last_check = security_review_get_last_check('security_review', 'comments');
  if ($last_check['skip'] == '1') {
    $element['findings']['descriptions'][] = _security_review_check_skipped($last_check);
  }
  elseif ($last_check['result'] == '0') {
    $element['findings']['descriptions'][] = t('The following comments have dangerous tags. The links go to the edit page.');
    $return = security_review_check_comments(); // Don't pass $last_check because timestamp is wrong now.
    foreach ($return['value'] as $cid => $nid) {
      $comment = _comment_load($cid);
      // There is no access checking. We state that the use of this module should be granted to trusted users only.
      $node = node_load($nid);
      $title = t('!subject on !title', array('!subject' => $comment->subject, '!title' => $node->title));
      $element['findings']['items'][] = l($title, 'comment/edit/' . $cid);
    }
    $element['findings']['pager'] = theme('pager', NULL, 20);
  }

  $output = theme('security_review_check_help', $element);
  return $output;
}