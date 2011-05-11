<?php

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _release_log_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'spaces-feature-rslog';
  $context->description = '';
  $context->tag = 'Release Logs';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'releaselogs' => 'releaselogs',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-atrium_casetracker-block_2' => array(
          'module' => 'views',
          'delta' => 'atrium_casetracker-block_2',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
    'menu' => 'release-logs',
  );
  $context->condition_mode = 0;

  $translatables['spaces-feature-rslog'] = array(
    t('Release Logs'),
  );

  $export['spaces-feature-rslog'] = $context;
  return $export;
}

/**
 * Helper to implementation of hook_ctools_plugin_api().
 */
function _release_log_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
}

/**
 * Helper to implementation of hook_menu_default_items().
 */
function _release_log_menu_default_items() {
  $items = array();

  $items[] = array(
    'title' => 'Release Logs',
    'path' => 'release-logs',
    'weight' => '0',
  );
  // Translatables
  array(
    t('Release Logs'),
  );


  return $items;
}
