<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function razem_obin_org_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'spaces_dashboard-custom-2';
  $context->description = 'Dashboard';
  $context->tag = 'Dashboard';
  $context->conditions = array();
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-box-75a9df25' => array(
          'module' => 'boxes',
          'delta' => 'box-75a9df25',
          'region' => 'content',
          'weight' => '0',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard');

  $export['spaces_dashboard-custom-2'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'spaces_dashboard-custom-3';
  $context->description = 'Dashboard';
  $context->tag = 'Dashboard';
  $context->conditions = array();
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-box-4e738e92' => array(
          'module' => 'boxes',
          'delta' => 'box-4e738e92',
          'region' => 'left',
          'weight' => '0',
        ),
        'boxes-box-cdf8f2d3' => array(
          'module' => 'boxes',
          'delta' => 'box-cdf8f2d3',
          'region' => 'right',
          'weight' => '0',
        ),
      ),
      'layout' => 'columns',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard');

  $export['spaces_dashboard-custom-3'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'spaces_dashboard-custom-4';
  $context->description = 'Dashboard';
  $context->tag = 'Dashboard';
  $context->conditions = array();
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-blog_comments-block_1' => array(
          'module' => 'views',
          'delta' => 'blog_comments-block_1',
          'region' => 'right',
          'weight' => '0',
        ),
        'views-casetracker_cases-block_2' => array(
          'module' => 'views',
          'delta' => 'casetracker_cases-block_2',
          'region' => 'content',
          'weight' => '0',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard');

  $export['spaces_dashboard-custom-4'] = $context;
  return $export;
}
