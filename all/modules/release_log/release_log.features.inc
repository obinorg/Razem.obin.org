<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function release_log_context_default_contexts() {
  module_load_include('inc', 'release_log', 'release_log.defaults');
  $args = func_get_args();
  return call_user_func_array('_release_log_context_default_contexts', $args);
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function release_log_ctools_plugin_api() {
  module_load_include('inc', 'release_log', 'release_log.defaults');
  $args = func_get_args();
  return call_user_func_array('_release_log_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_menu_default_items().
 */
function release_log_menu_default_items() {
  module_load_include('inc', 'release_log', 'release_log.defaults');
  $args = func_get_args();
  return call_user_func_array('_release_log_menu_default_items', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function release_log_views_default_views() {
  module_load_include('inc', 'release_log', 'release_log.features.views');
  $args = func_get_args();
  return call_user_func_array('_release_log_views_default_views', $args);
}
