<?php

/**
 * Nurani Library administrative settings form.
 */
function nurani_library_admin_form($form, &$form_state) {
  $form['nurani_library_texts_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Nurani Library texts path'),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => nurani_library_texts_path(),
    '#description' => t('Path to the directory which contains raw texts.  Currently supported formats are <a href="http://www.crosswire.org/osis/">OSIS</a> and <a href="http://tanzil.net/download/">Tanzil XML</a>.  Texts may reside in subdirectories of this path.'),
    '#required' => FALSE,
  );
  $form = system_settings_form($form);
  return $form;
}

/**
 * Nurani Library administrative form for managing texts.
 */
function nurani_library_texts_form($form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'nurani_library') . '/nurani_library.js');

  $texts = nurani_library_find_texts(nurani_library_texts_path());

  $form['texts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Texts'),
    '#collapsible' => FALSE,
    'table' => array(
      '#tree' => TRUE,
      '#theme' => 'nurani_library_texts_form_table',
    )
  );
  $row = 0;
  $type_options = nurani_library_text_type_options();
  foreach (nurani_library_texts() as $text) {
    $form['texts'][$row] = array(
      '#text' => $text,
    );

    foreach ($text as $col => $value) {
      switch ($col) {
        case 'type':
          $form['texts']['table'][$row][$col] = array(
            '#type' => 'select',
            '#title' => $col,
            '#default_value' => $value,
            '#options' => $type_options,
          );
          break;

        default: 
          $form['texts']['table'][$row][$col] = array(
            '#type' => 'textfield',
            '#title' => $col,
            '#size' => 16,
            '#maxlength' => 64,
            '#default_value' => $value,
            '#disabled' => ($col == 'machine_name'),
          );
          break;
      }

      $form['texts']['table'][$row][$col]['#required'] = TRUE;
    }

    $form['texts']['table'][$row]['actions'] = array(
      'delete' => array(
        '#type' => 'button',
        '#value' => t('Delete'),
        '#attributes' => array('data-machine_name' => $text->machine_name, 'class' => array('delete-action')),
      ),
    );

    $row += 1;
  }

  $form['texts']['add'] = array(
    '#type' => 'button',
    '#value' => t('New text'),
    '#attributes' => array('class' => array('add-action')),
  );

  return $form;
}

/**
 * Theme function to render the form elements for each Text as a table.
 */
function theme_nurani_library_texts_form_table(&$variables) {
  $output = '';

  $rows = array();
  $header = array();
  $element = &$variables['element'];

  foreach (element_children($element) as $row) {
    foreach (element_children($element[$row]) as $col) {
      if ($row == 0) {
        $header[] = $col;
      }

      unset($element[$row][$col]['#title']);

      $rows[$row][] = drupal_render($element[$row][$col]);
    }
  }

  drupal_add_css(drupal_get_path('module', 'nurani_library') . '/nurani_library.css');
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('nurani-library-texts-form-table'))));
}
