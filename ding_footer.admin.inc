<?php

function ding_footer_admin_page() {
  $path = drupal_get_path('module','ding_footer');
  
  drupal_add_js($path . '/js/ckeditor.js');
  drupal_add_js($path . '/js/ckeditorjquery.js');
  drupal_add_js($path . '/js/ding_footer.admin.js');

  return drupal_get_form('ding_footer_admin_form');
}

function ding_footer_admin_form ($form, &$form_state) {
  $blocks = variable_get('ding_footer_blocks');
  
  $form['#tree'] = TRUE;
  $form['blocks_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blocks to display in the footer.'),
    '#prefix' => '<div id="blocks_fieldset_wrapper">',
    '#suffix' => '</div>',
  );
  
  if (empty($form_state['num_blocks'])) {
    $form_state['num_blocks'] = !empty($blocks) ? count($blocks) : 1;
  }
  
  for ($i = 0; $i < $form_state['num_blocks']; $i++) {   
     $form['blocks_fieldset']['name'][$i] = array(
      '#type' =>   'textfield',
      '#title' => 'Block #' . ($i + 1),
      '#default_value' => isset($blocks[$i]) ? $blocks[$i] : '',
      '#attributes' => array('class' => array('load_wysiwyg')),
    );
  }
  
  $form['blocks_fieldset']['add_block'] = array(
    '#type' => 'submit',
    '#value' => t('Add another block'),
    '#submit' => array('ding_footer_add_one'),
    '#ajax' => array(
      'callback' => 'ding_footer_callback',
      'wrapper' => 'blocks_fieldset_wrapper',
    ),
  );
  
  if ($form_state['num_blocks'] > 1) {
    $form['blocks_fieldset']['remove_block'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('ding_footer_remove_one'),
      '#ajax' => array(
        'callback' => 'ding_footer_callback',
        'wrapper' => 'blocks_fieldset_wrapper',
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

function ding_footer_add_one ($form, &$form_state) {
  $form_state['num_blocks']++;
  $form_state['rebuild'] = TRUE;
}

function ding_footer_remove_one ($form, &$form_state) {
  $form_state['num_blocks']--;
  $form_state['rebuild'] = TRUE;
}

function ding_footer_callback ($form, $form_state) {
  return $form['blocks_fieldset'];
}

function ding_footer_admin_submit ($form, &$form_state) {
  $blocks = array();
  
  for($i = 0; $i < $form_state['num_blocks']; $i++) {
    $blocks[] = $form_state['values']['blocks_fieldset']['name'][$i];
  }
  
  variable_set('ding_footer_blocks', $blocks);
}
