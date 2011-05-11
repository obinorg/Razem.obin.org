<?php

/**
 * @file
 * Advanced aggregation module; 404 handler.
 *
 */

/**
 * Menu Callback; regenerates a missing css file.
 *
 * @param $filepath
 *   filename
 * @return
 *   Boolean indicating if the bundle already exists.
 */
function advagg_missing_css() {
  ignore_user_abort();

  // Try to regenerate missing file
  $msg = advagg_missing_regenerate();

  // If here send out fast 404.
  advagg_missing_fast404($msg);
}

/**
 * Menu Callback; regenerates a missing js file.
 */
function advagg_missing_js() {
  ignore_user_abort();

  // Try to regenerate missing file
  $msg = advagg_missing_regenerate();

  // If here send out fast 404.
  advagg_missing_fast404($msg);
}

/**
 * regenerates a missing css file.
 *
 * @param $filename
 *   filename
 * @param $type
 *   css or js
 * @return
 *   false if bundle couldn't be generated.
 */
function advagg_missing_regenerate() {
  global $base_path, $conf;

  // Get filename from request.
  $arg = arg();
  $filename = array_pop($arg);
  $filename = array_shift(explode('?', $filename));

  // Verify requested filename has the correct pattern.
  if (preg_match('/^(j|cs)s_[0-9a-f]{32}_\d+\.(j|cs)s$/', $filename) == FALSE) {
    return t('Wrong Pattern.');
  }

  // Get type
  $type = substr($filename, 0, strpos($filename, '_'));

  // Get extension
  $ext = substr($filename, strpos($filename, '.', 37)+1);

  // Make sure extension is the same as the type.
  if ($ext != $type) {
    return t('Type does not match extension.');
  }

  // Extract info from wanted filename.
  if ($type == 'css') {
    $md5 = substr($filename, 4, 32);
    $counter = substr($filename, 37, strpos($filename, '.', 38)-37);
  }
  elseif ($type == 'js') {
    $md5 = substr($filename, 3, 32);
    $counter = substr($filename, 36, strpos($filename, '.', 37)-36);
  }
  else {
    return t('Wrong file type.');
  }

  $_GET['redirect_counter'] = isset($_GET['redirect_counter']) ? intval($_GET['redirect_counter']) : 0;
  if ($_GET['redirect_counter'] > 5) {
    watchdog('advagg', 'This request could not generate correctly. Loop detected. Request data: %info', array('%info' => $_GET['q']));
    return t('In a Loop.');
  }

  // Set file(s) in cache to FALSE.
  $arg[] = $filename;
  cache_set(implode('/', $arg), FALSE, 'cache_advagg', TRUE);
  advagg_missing_remove_cache($md5);

  // Counter in database.
  $counter_in_db = db_result(db_query("SELECT counter FROM {advagg_bundles} WHERE bundle_md5 = '%s'", $md5));
  if ($counter_in_db === FALSE) {
    return t('Not a valid bundle.');
  }

  // Only process if we got an older counter.
  if ($counter > $counter_in_db || $counter < 0) {
    return t('Counter out of range.');
  }

  // Rebuild file.
  $conf['advagg_async_generation'] = FALSE;
  $good = advagg_rebuild_bundle($md5, $counter, TRUE);
  if (!$good) {
    watchdog('advagg', 'This request could not generate correctly. Aggregate not generated. Request data: %info', array('%info' => $_GET['q']));
    return t('Rebuild Failed.');
  }

  // Redirect to file.
  $_GET['redirect_counter']++;
  $uri = $base_path . $_GET['q'] . '?redirect_counter=' . $_GET['redirect_counter'];
  usleep(250000); // Sleep for 250ms
  header('Location: ' . $uri, TRUE, 307);
  exit;
}

/**
 * Set cache value to FALSE.
 *
 * @param $bundle_md5
 *   Bundle's machine name.
 */
function advagg_missing_remove_cache($bundle_md5) {
  $files = array();
  $results = db_query("SELECT filename, filetype FROM {advagg_files} AS af INNER JOIN {advagg_bundles} AS ab USING ( filename_md5 ) WHERE bundle_md5 = '%s' ORDER BY porder ASC", $bundle_md5);
  while($row = db_fetch_array($results)) {
    $files[] = $row['filename'];
    $type = $row['filetype'];
  }

  list($css_path, $js_path) = advagg_get_root_files_dir();
  if ($type == 'js') {
    $file_type_path = $js_path;
  }
  if ($type == 'css') {
    $file_type_path = $css_path;
  }

  $filenames = advagg_get_filename($files, $type, '', $bundle_md5);
  foreach ($filenames as $key => $info) {
    $filename = $info['filename'];
    $filepath = $file_type_path .'/'. $filename;

    cache_set($filepath, FALSE, 'cache_advagg', TRUE);
  }
}
