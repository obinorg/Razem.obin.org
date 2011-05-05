<?php

/**
 * Implementation of hook_default_box().
 */
function razem_obin_org_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-4e738e92';
  $box->plugin_key = 'simple';
  $box->title = 'Freehemozone news';
  $box->description = 'Free hemp zone news';
  $box->options = FALSE;

  $export['box-4e738e92'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-5ae91cb4';
  $box->plugin_key = 'simple';
  $box->title = 'ubuntu';
  $box->description = 'ubuntu 10.04';
  $box->options = FALSE;

  $export['box-5ae91cb4'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-5ca85855';
  $box->plugin_key = 'simple';
  $box->title = 'Komunikacja';
  $box->description = 'Komunikacja';
  $box->options = FALSE;

  $export['box-5ca85855'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-5e7483bd';
  $box->plugin_key = 'simple';
  $box->title = '';
  $box->description = 'mapa';
  $box->options = FALSE;

  $export['box-5e7483bd'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-75a9df25';
  $box->plugin_key = 'simple';
  $box->title = 'Dokumentacja';
  $box->description = 'Dokumentacja Razem';
  $box->options = FALSE;

  $export['box-75a9df25'] = $box;
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-cdf8f2d3';
  $box->plugin_key = 'simple';
  $box->title = 'Cuckarnia';
  $box->description = 'Cuckarnia';
  $box->options = FALSE;

  $export['box-cdf8f2d3'] = $box;
  return $export;
}
