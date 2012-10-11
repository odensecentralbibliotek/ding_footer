<?php

/**
 * @file
 * Implementation of the administration interface that allows the user to add an
 * arbitrary number of blocks to the configuration.
 */

/**
 * Handles the administion menu callback and load the adminstration UI for the
 * module.
 *
 * @todo: The wysiwyg editor should be handle through the wysiwyg module so it's
 * not hardcode to ckeditor. Which means that the JS should be alter to.
 */
function ding_footer_admin_page() {
  // Add javascript to handle the the wysiwyg editor on ajax calls.
  drupal_add_js(drupal_get_path('module', 'ding_footer') . '/js/ding_footer.admin.js', array('weight' => 12));

  // Get the administion from and return it.
  return drupal_get_form('ding_footer_admin_form');
}

/**
 *
 */
function ding_footer_admin_form($form, &$form_state) {
  // Set the form to be empty.
  $form = array(
    '#tree' => TRUE,
  );

  // Load previous saved content.
  $blocks = variable_get('ding_footer_blocks');

  // Add fieldset around the form to have a wrapper to append "add on more"
  // elements to.
  $form['blocks_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content to display in the footer.'),
    '#prefix' => '<div id="blocks-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  // Set the number of blocks counter, if this is the first form build.
  if (empty($form_state['number_of_blocks'])) {
    $form_state['number_of_blocks'] = !empty($blocks) ? count($blocks) : 1;
  }

  // Add fields for each footer block.
  for ($i = 0; $i < $form_state['number_of_blocks']; $i++) {
     $form['blocks_fieldset']['name'][$i] = array(
      '#type' => 'text_format',
      '#title' => t('Footer @number', array('@number' => $i + 1)),
      '#default_value' => isset($blocks[$i]) ? $blocks[$i] : '',
      '#attributes' => array(
        'class' => array('wysiwyg-ajax'),
      ),
    );
  }

  // Add one more submit.
  $form['blocks_fieldset']['add_block'] = array(
    '#type' => 'submit',
    '#value' => t('Add another block'),
    '#submit' => array('ding_footer_add_one'),
    '#ajax' => array(
      'callback' => 'ding_footer_fallback',
      'wrapper' => 'blocks-fieldset-wrapper',
    ),
  );

  // Remove one submit, if there are more than on footer block.
  if ($form_state['number_of_blocks'] > 1) {
    $form['blocks_fieldset']['remove_block'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('ding_footer_remove_one'),
      '#ajax' => array(
        'callback' => 'ding_footer_fallback',
        'wrapper' => 'blocks-fieldset-wrapper',
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['#submit'][] = 'ding_footer_admin_submit';
  return $form;
}

/**
 * Add one more ajax callback.
 */
function ding_footer_add_one($form, &$form_state) {
  $form_state['number_of_blocks']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Remove one ajax callback.
 */
function ding_footer_remove_one($form, &$form_state) {
  $form_state['number_of_blocks']--;
  $form_state['rebuild'] = TRUE;
}

/**
 * Fallback submit function used, if the user does not have javascript support
 * enabled.
 */
function ding_footer_fallback($form, $form_state) {
  return $form['blocks_fieldset'];
}

/**
 * The administration UI submit callback.
 */
function ding_footer_admin_submit($form, &$form_state) {
  $content = array();

  for ($i = 0; $i < $form_state['number_of_blocks']; $i++) {
    $content[] = $form_state['values']['blocks_fieldset']['name'][$i]['value'];
  }

  variable_set('ding_footer_blocks', $content);
}
