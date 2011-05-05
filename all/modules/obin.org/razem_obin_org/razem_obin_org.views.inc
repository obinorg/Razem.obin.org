<?php
// $Id: crayon.views.inc,v 1.1.2.1 2010/08/19 14:49:36 yhahn Exp $

/**
 * Implementation of hook_views_handlers().
 */
function razem_obin_org_views_handlers() {
  $handlers = array();
  $handlers['handlers'] = array();
  $handlers['info']['path'] = drupal_get_path('module', 'razem_obin_org') .'/views';
  // OG: Group.
  if (module_exists('og_views')) {
    if (module_exists('spaces_og')) {
      $handlers['handlers']['razem_obin_org_views_handler_field_spaces_og'] = array(
        'parent' => 'views_handler_field_spaces_og',
      );
    }
  }
  return $handlers;
}


/**
 * Implementation of hook_views_data_alter().
 */
function razem_obin_org_views_data_alter(&$data) {
  // OG: Group.
  if (module_exists('og_views') && isset($data['og_ancestry']['group_nid']['field']['handler'])) {
    if (module_exists('spaces_og')) {
      $data['og_ancestry']['group_nid']['field']['handler'] = 'crayon_views_handler_field_spaces_og';
    }
  }
  
}