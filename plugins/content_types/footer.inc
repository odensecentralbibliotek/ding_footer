<?php

/**
 * @file
 * Provides footer as a Panel content type.
 */

$plugin = array(
  'title' => t('Ding Footer'),
  'single' => TRUE,
  'description' => t('Provides ding footer content'),
  'category' => t('Ding'),
);

/**
 * Return the pane contents.
 */
function ding_footer_footer_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->content = theme('ding_footer', array('blocks' => variable_get('ding_footer_blocks', array())));
  return $block;
}

/**
 * Return the pane titles.
 */
function ding_footer_footer_content_type_admin_title($subtype, $conf, $context) {
  return t('Ding Footer');
}

/**
 * Return an edit form for the content type.
 */
function ding_footer_footer_content_type_edit_form($form, &$form_state) {
  return $form;
}
