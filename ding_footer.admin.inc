<?php

/**
 * Admin page menu callback.
 *
 * @return
 *   Admin form.
 */
function ding_footer_admin_page() {
  return drupal_get_form('ding_footer_admin_form');
}

function ding_footer_admin_form ($form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['blocks_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blocks to display in the footer.'),
    '#prefix' => '<div id="blocks_fieldset_wrapper',
    '#suffix' => '</div>',
  );
  
  if (empty($form_state['num_blocks'])) {
    $form_state['num_blocks'] = 1;
  }
  
  for ($i = 0; $i < $form_state['num_blocks']; $i++) {
    $form['blocks_fieldset']['name'][$i] = array(
      '#type' =>   'text_format',
      '#title' => 'Block #' . $i,
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
  
  $form['#submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  
  return $form;
}

