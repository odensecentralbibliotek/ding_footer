<?php
/**
 * @file
 * Admin related backend.
 */

/**
 * Admin page menu callback.
 *
 * @return
 *   Admin form.
 */
function ding_footer_admin_page() {
  $path = drupal_get_path('module','ding_footer');
  drupal_add_js($path . '/js/ding_footer.admin.js');
  drupal_add_css($path . '/css/ding_footer.css');
  variable_set('ding_footer_blocks_hidden', array());

  return drupal_get_form('ding_footer_admin_form');
}

/**
 * Admin settings form for footer.
 *
 */
function ding_footer_admin_form($form, &$form_state) {
  $blocks_num = !empty($form_state['values']['num_blocks']) ? $form_state['values']['num_blocks'] : 0;
  $clicked_btn = '';
  $remove_btn = '';
  $hidden = variable_get('ding_footer_blocks_hidden', array());

  if (isset($form_state['clicked_button'])) {
    $clicked_btn = $form_state['clicked_button']['#name'];

    //Get a clicked 'remove' button
    foreach ($form_state['buttons'] as $k => $v) {
      if ($v['#name'] == $clicked_btn) {
	if (preg_match('/edit-remove/', $v['#id'])) {
	  $remove_btn = $k;
	  $hidden[] = $remove_btn;
	  variable_set('ding_footer_blocks_hidden', $hidden);
	  break;
	}
      }
    }
  }

  $form['ding_footer'] = array(
    '#type' => 'fieldset',
    '#title' => '',
  );

  // Get current saved blocks.
  $blocks = variable_get('ding_footer_blocks', array());

  $i = 0;
  $c = 1;

  // Display saved blocks.
  if (isset($blocks[0]['content'])) {
    $c = count($blocks);
    for (; $i < $c; $i++) {
      $form['ding_footer']['blocks']['block_' . $i] =
	ding_footer_block_form($blocks[$i], $i);
    }
  }

  //Whether add a new block field.
  if ($clicked_btn == 'add_footer_block') {
    $blocks_num++;
  }

  for ($j = 0; $i < $blocks_num + $c; $i++,$j++) {
    $unsubmitted = array();
    // Fill previous field data if available.
    if (isset($form_state['input']['save'][$i])) {
      $unsubmitted = array(
        'content' => $form_state['input']['content'][$i * 2],
	'save' => $form_state['input']['save'][$i]
      );
    }
    $form['ding_footer']['blocks']['block_' . $i] = ding_footer_block_form($unsubmitted, $i);
  }

  // Hide removed fields
  for ($i = 0; $i < $blocks_num + $c; $i++) {
    if (in_array($i, $hidden)) {
      $form['ding_footer']['blocks']['block_' . $i]['save[]']['#value'] = '';
      $form['ding_footer']['blocks']['block_' . $i]['#prefix'] = '<div class="ding-footer-block-hidden">';
      $form['ding_footer']['blocks']['block_' . $i]['#suffix'] = '</div>';
    }
  }

  // Keep track of block fields count.
  $form['ding_footer']['num_blocks'] = array(
    '#type' => 'hidden',
    '#value' => $blocks_num,
  );

  // Ajaxified button for new fields.
  $form['ding_footer']['new_block_button'] = array(
    '#type' => 'button',
    '#value' => 'Add another',
    '#name' => 'add_footer_block',
    '#ajax' => array(
      'callback' => 'ding_footer_admin_form_ajaxify',
      'wrapper' => 'ding-footer-blocks',
      'method' => 'replace',
      'effect' => 'fade'
    ),
  );

  // Wrap the form, due to draggable items renewal.
  $form['#prefix'] = '<div id="ding-footer-blocks">';
  $form['#suffix'] = '</div>';
  // Custom themer, mainly used for dragable table creation.
  $form['#theme'] = array('ding_footer_admin_form');
  // Custom form submit handler.
  $form['#submit'] = array('ding_footer_block_submit');

  return system_settings_form($form);
}

/**
 * Form part, describing block fields.
 *
 * @param $item
 *   Values for existing block.
 *
 * @return
 *   Fields structure.
 */
function ding_footer_block_form($item = array(), $index = 0) {
  $form = array();

  $form['sort[]'] = array(
    '#type' => 'weight',
    '#delta' => 5,
    '#default_value' => 0,
    '#value' => 0,
    '#attributes' => array(
      'class' => array(
        'sort-hidden'
      )
    ),
  );

  $form['content[]'] = array(
    '#type' => 'text_format',
    '#title' => t('Content'),
    '#description' => t('Contents for this footer column'),
    '#default_value' => isset($item['content']['value']) ? $item['content']['value'] : '',
    '#prefix' => '<div class="ding-footer-block">',
  );

  $form['save[]'] = array(
    '#type' => 'hidden',
    '#title' => t('Saved'),
    '#value' => isset($item['save']) ? $item['save'] : t('MAYBE'),
  );

  $form['remove[]'] = array(
    '#type' => 'button',
    '#value' => t('Remove'),
    '#name' => 'remove_' . $index,
    '#attributes' => array(
      'class' => array(
        'remove'
      )
    ),
    '#suffix' => '</div><div class="clearfix"></div>',
    '#ajax' => array(
      'callback' => 'ding_footer_admin_form_ajaxify',
      'wrapper' => 'ding-footer-blocks',
      'method' => 'replace',
      'effect' => 'fade'
    ),
  );

  return $form;
}

/**
 * Submit handler for admin backend blocks form.
 *
 * @param $form
 *   Form.
 * @param $form_state
 *   Form state.
 */
function ding_footer_block_submit($form, &$form_state) {
  $c = count($form_state['input']['save']);
  $blocks = array();

  for ($i = 0; $i < $c; $i++) {
    // Make an array of the data to save, ignoring blocks not marked for saving.
    if (!empty($form_state['input']['save'][$i])) {
      $content = $form_state['input']['content'][$i * 2];
      if (empty($content['value']))
	continue;

      $blocks[] = array(
	'content' => $content,
	'save' => 'YES'
      );
    }
  }

  // Save the blocks as a persistent variable.
  variable_set('ding_footer_blocks', $blocks);
}

/**
 * AJAX responder for field addition/removal fields.
 *
 * @param $form
 *   Form.
 *
 * @return
 *   Changed field to be updated.
 */
function ding_footer_admin_form_ajaxify($form, &$form_state) {
  return $form;
}

/**
 * Theme the blocks form, mainly adding a table drag-n-drop feature.
 *
 * @param $variables
 *   Theme related variables.
 *
 * @return
 *   Themed form output.
 */
function theme_ding_footer_admin_form($variables) {
  $form = $variables['form'];
  //drupal_add_tabledrag('ding-footer-admin-sort', 'order', 'sibling', 'sort');

  $header = array('Content Columns', '');
  $rows = array();
  foreach (element_children($form['ding_footer']['blocks']) as $key) {
    // Add class to group weight fields for drag and drop.
    $form['ding_footer']['blocks'][$key]['sort[]']['#attributes']['class'][] = 'sort';

    // Get sort element.
    $sort = $form['ding_footer']['blocks'][$key]['sort[]'];
    unset($form['ding_footer']['blocks'][$key]['sort[]']);

    // Build rows.
    $classes = array('draggable');
    if (isset($form['ding_footer']['blocks'][$key]['#prefix'])) {
      $classes[] = 'block-hidden';
    }

    $rows[] = array(
      'data' => array(
        drupal_render($form['ding_footer']['blocks'][$key]),
        drupal_render($sort)
      ),
      'class' => $classes
    );
  }

  $table_vars = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'ding-footer-admin-sort'),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => 'EMPTY'
  );
  $output = theme_table($table_vars);

  // Render remaining elements.
  $output .= drupal_render_children($form);

  return $output;
}
