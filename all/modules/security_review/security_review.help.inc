<?php
// $Id: security_review.help.inc,v 1.1.2.2 2009/12/08 22:16:13 coltrane Exp $

/**
 * @file
 * Main help definition.
 */

function _security_review_help() {
  drupal_add_js(drupal_get_path('module', 'security_review') . '/security_review.js', 'module', 'header');
  $output = '';
  $output .= '<p>';
  $output .= t('You should take the security of your site very seriously.
    Fortunately, Drupal is fairly secure by default.
    The Security Review module automates many of the easy-to-make mistakes that render your site insecure, however it does not automatically make your site impenetrable.
    You should give care to what modules you install and how you configure your site and server.
    Be mindful of who visits your site and what features you expose for their use.');
  $output .= '</p>';
  $output .= '<p>';
  $output .= t('You can read more about securing your site in the <a href="!do">drupal.org handbooks</a> and on <a href="!cd">CrackingDrupal.com</a>.
    There are also additional modules you can install to secure or protect your site. Be aware though that the more modules you have running on your site the greater (usually) attack area you expose.',
    array('!do' => 'http://drupal.org/security/secure-configuration', '!cd' => 'http://crackingdrupal.com'));
  $output .= '</p>';
  $output .= '<p>' . l(t('Drupal.org Handbook: Introduction to security-related contrib modules'), 'http://drupal.org/node/382752') . '</p>';
  /*$output .= '<h3>' . t('Contrib modules for additional security and strength') . '</h3>';
  $output .= '<p>' . t('There are a wealth of modules on drupal.org.') . '</p>';
  $items[] = _security_review_help_ssl();
  $items[] = _security_review_help_spam();
  $output .= theme('item_list', $items);*/
  return $output;
}

function _security_review_help_ssl() {
  $description = t('The transfer of data between the visitor on your site and you Drupal installation can be secured through encryption.');
  $element = array(
    'problem' => t("Secure and private communication"),
    'type' => 'ssl',
    'description' => $description,
    'options' => array(
      array(
        'name' => 'Secure Pages',
        'href' => 'http://drupal.org/project/securepages',
      ),
    ),
  );
  return theme('security_review_help_options', $element);
}

function _security_review_help_spam() {
  $element = array(
    'problem' => t("Spammers and spam content"),
    'type' => 'spam',
    'description' => t('Spammers use automated tools and systems to bombarge your site with unsollicited content. You can use modules to prevent the submission of such content.'),
    'options' => array(
      array(
        'name' => 'CAPTCHA',
        'href' => 'http://drupal.org/project/captcha',
      ),
    ),
  );
  return theme('security_review_help_options', $element);
}

function theme_security_review_help_options($element) {
  $output .= '<div class="sec-rev-help-option">';
  $output .= l($element['problem'], 'admin/reports/security-review/help', array('fragment' => $element['type'], 'attributes' => array('class' => 'sec-rev-help-dyn')));
  $output .= '<div class="sec-rev-help-content">';
  $output .= '<p>' . $element['description'] . '</p>';
  foreach ($element['options'] as $option) {
    $items[] = l($option['name'], $option['href']);
  }
  $output .= theme('item_list', $items);
  $output .= '</div>';
  $output .= '</div>';
  return $output;
}
