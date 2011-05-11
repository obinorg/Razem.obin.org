<?php
/**
 * Configuration callback for this module.
*/
function uploadpath_admin_settings() {

  $form['uploadpath_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Default pattern for the file path prefix'),
    '#description' => t('Specify the pattern to prefix to file names uploaded with the upload module.  It will be appended after the site files directory (e.g., files) but before the file name itself.  Do not include a leading or trailing slash.  Spaces will be converted to underscores to avoid file system issues.'),
    '#default_value' => variable_get('uploadpath_prefix', '[type]/[yyyy]/[mm]'),
    '#weight' => -9
  );
  
  $form['misc'] = array(
    '#title' => t('Miscellaneous'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Other settings'),
    '#weight' => -6
  );
  
  $form['misc']['uploadpath_clean_filenames'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clean file upload filenames'),
    '#return_value' => 1,
    '#default_value' => variable_get('uploadpath_clean_filenames', true),
    '#description' => t('Rename uploaded files based on file desc node title. characters other than numbers and letters are replaced with underscore and a unique id is appended. maximum total length is 60 characters.'),
    '#weight' => 0,
  );
  
  $form['node_types_exclude'] = array(
    '#title' => t('Excluded node types'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Select the node types to exclude from upload path processing'),
    '#weight' => -8
  );
  
  $form['node_types'] = array(
    '#title' => t('Patterns for each node type'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Patterns for node types. If empty, the default pattern will be used.'),
    '#weight' => -7
  );
  
  $form['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Prefer raw-text replacements for text to avoid problems with HTML entities!'),
    '#weight' => -6
  );
  $form['token_help']['help'] = array(
    '#value' => theme('token_help', 'node'),
  );
  
  //node type settings
  $node_types = node_get_types();
  foreach($node_types as $type){
  	$types[$type->type] = $type->name;
  }
  //set excluded node types
  $form['node_types_exclude']['uploadpath_excluded_node_types'] = array(
    '#type' => 'select',
  	'#multiple' => true,
    '#title' => t('Excluded Node Types'),
    '#default_value' => variable_get('uploadpath_excluded_node_types', array()),
    '#description' => t('Select the node types to exclude from uploadpath processing.'),
  	'#options' => $types,
  );
  
  foreach($node_types as $type){
    //only show settings if not excluded
    if(!in_array($type->type, variable_get('uploadpath_excluded_node_types',array()))){
      $form['node_types']['uploadpath_prefix_'.$type->type] = array(
      '#type' => 'textfield',
      '#title' => t('Path pattern for '.$type->name),
      '#description' => t('Specify the path pattern to prefix for '.$type->name.' type nodes'),
      '#default_value' => variable_get('uploadpath_prefix_'.$type->type, ''),  	
      );
    }
  }
  return system_settings_form($form);
}
