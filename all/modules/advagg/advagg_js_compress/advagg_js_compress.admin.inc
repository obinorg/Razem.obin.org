<?php

/**
 * @file
 * Admin page callbacks for the advagg JS compression module.
 */

/**
 * Page generation function for admin/settings/js-compress
 */
function advagg_js_compress_admin_page() {
  $output = '';
  return $output . drupal_get_form('advagg_js_compress_admin_settings_form');
}

/**
 * Form builder; Configure advagg settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function advagg_js_compress_admin_settings_form() {
  $form = array();

  $form['advagg_js_compress_packer_enable'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Enable Packer'),
    '#default_value'  => variable_get('advagg_js_compress_packer_enable', ADVAGG_JS_COMPRESS_PACKER_ENABLE),
    '#description'    => t('If enabled the non gzip version of JS files will be compressed using the JS Packer. WARNING: This has a very HIGH chance of breaking your JS. Only Enable after testing the non gzipped version.'),
  );

  return system_settings_form($form);
}
