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
    '#description' => t("Path to the directory which contains raw texts. "
                      . "Currently supported formats are !osis_link and "
                      . "!tanzil_link.  Texts may reside in subdirectories of "
                      . "this path.", array(
                          '!osis_link' => nurani_library_text_format_link('osis'),
                          '!tanzil_link' => nurani_library_text_format_link('tanzil'),
                        )),
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

  $form['texts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Texts'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    'table' => array(
      '#theme' => 'nurani_library_texts_form_table',
      '#parents' => array('texts'),
    )
  );

  $select_col_options = array(
    // TODO: The list of languages should not be hard-coded.
    'language' => nurani_library_text_languages(),
    'type'     => nurani_library_text_type_options(),
  );
  $descriptions = array(
    'name' => t('The machine-readable name. Can only contain lowercase letters, numbers, and underscores.'),
    'full_name' => t("The human readable title of the text."),
    'language' => t("The language this version of the text is written in."),
  );
  $row = 0;
  $texts = nurani_library_texts();
  $texts[] = _nurani_library_new_text($form_state);
  foreach ($texts as $text) {
    $form['texts'][$row] = array(
      '#text' => $text,
    );

    // Ensure the new record is clearly separated from the existing records
    if ($text->id == 'new') {
      $row = 'new';
    }

    foreach ($text as $col => $value) {
      switch ($col) {
        case 'id':
          $form['texts']['table'][$row][$col] = array(
            '#type' => 'value',
            '#value' => $value,
          );
          break;

        case 'name':
          if ($row !== 'new') {
            $form['texts']['table'][$row][$col] = array(
              '#type' => 'markup',
              '#markup' => '<em>' . check_plain($value) . '</em>',
              'name' => array(
                '#type' => 'value',
                '#value' => $value,
                '#parents' => array('texts', $row, $col),
              ),
            );
          }
          else {
            $form['texts']['table'][$row][$col] = array(
              '#type' => 'textfield',
              '#size' => 16,
              '#maxlength' => 32,
            );
          }
          break;

        case 'type':
        case 'language':
          $form['texts']['table'][$row][$col] = array(
            '#type' => 'select',
            '#options' => $select_col_options[$col],
          );
          break;

        default: 
          $form['texts']['table'][$row][$col] = array(
            '#type' => 'textfield',
            '#size' => 16,
            '#maxlength' => 64,
          );
          break;
      }

      $form['texts']['table'][$row][$col]['#title'] = $col;
      $form['texts']['table'][$row][$col]['#description'] = isset($descriptions[$col]) ? $descriptions[$col] : NULL;
      $form['texts']['table'][$row][$col]['#default_value'] = $value;
      $form['texts']['table'][$row][$col]['#required'] = ($row !== 'new');
      $form['texts']['table'][$row][$col]['#row'] = $row;
    }

    $form['texts']['table'][$row]['actions'] = array();
    if ($row === 'new') {
      $form['texts']['table'][$row]['actions']['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add new text'),
        '#validate' => array('nurani_library_texts_form_add_validate'),
        '#submit' => array('nurani_library_texts_form_add_submit'),
      );
    }
    else {
      $form['texts']['table'][$row]['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array('onclick' => "return confirm('" . t("Are you sure? This will delete ALL passages associated with this text. Also, any references to those passages will break.") . "');"),
        '#validate' => array('nurani_library_texts_form_delete_validate'),
        '#submit' => array('nurani_library_texts_form_delete_submit'),
        '#text' => $text,
      );
    }

    $row += 1;
  }

  $form['texts']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
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
  $first_row = TRUE;

  foreach (element_children($element) as $row) {
    foreach (element_children($element[$row]) as $col) {
      if ($col == 'id') {
        continue;
      }
      if ($first_row) {
        if (isset($element[$row][$col]['#description'])) {
          $description = '<small>' . check_plain($element[$row][$col]['#description']) . '</small>';
        }
        else {
          $description = '';
        }
        $header[] = $col . $description;
      }

      unset($element[$row][$col]['#title'], $element[$row][$col]['#description']);

      $rows[$row][] = array(
        'class' => array('nurani-library-texts-col-' . $col),
        'data' => drupal_render($element[$row][$col]),
      );
    }
    $first_row = FALSE;
  }

  drupal_add_css(drupal_get_path('module', 'nurani_library') . '/nurani_library.css');
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('nurani-library-texts-form-table'))));
}

/**
 * Validation handler for texts management form
 */
function nurani_library_texts_form_validate($form, &$form_state) {
  $texts = $form_state['values']['texts'];
  if (trim($texts['new']['name']) === '') {
    unset($texts['new'], $form_state['values']['texts']['new']);
  }
  foreach ($texts as $row => $text) {
    if (!is_array($text) || !isset($text['name'])) {
      continue;
    }
    _nurani_library_validate_text($text, $form['texts']['table'][$row]);
  }
}

/**
 * Submission handler for texts management form
 */
function nurani_library_texts_form_submit($form, &$form_state) {
  foreach ($form_state['values']['texts'] as $row => $text) {
    if (!is_array($text) || !isset($text['name'])) {
      continue;
    }
    if ($row === 'new') {
      drupal_set_message(t("Saved new text record."));
    }
    _nurani_library_save_text($text);
  }

  drupal_set_message(t("Updated existing text records."));
}

function nurani_library_texts_form_add_validate($form, &$form_state) {
  $text = $form_state['values']['texts']['new'];
  _nurani_library_validate_text($text, $form['texts']['table']['new'], TRUE);
}

function nurani_library_texts_form_add_submit($form, &$form_state) {
  $text = $form_state['values']['texts']['new'];
  _nurani_library_save_text($text);
  drupal_set_message(t("Saved new text record."));
}

function nurani_library_texts_form_delete_validate($form, &$form_state) {
  // Nothing to do currently.
}

function nurani_library_texts_form_delete_submit($form, &$form_state) {
  $text = $form_state['clicked_button']['#text'];
  if (!$text) {
    drupal_set_message(t("There was an error deleting this text. Please try again."), 'error');
  }
  else {
    $nl = nurani_library();
    $nl->model->deleteCorpus($text->id);
    drupal_set_message(t("The text %full_name (%name) and all of its passages were removed from the system.", array('%full_name' => $text->full_name, '%name' => $text->name)));
  }
}

function _nurani_library_new_text($form_state) {
  $edit = isset($form_state['values']['texts']['new']) ? $form_state['values']['texts']['new'] : array();
  $text = array(
    'id'        => 'new',
    'name'      => '',
    'full_name' => '',
    'language'  => 'en',
    'type'      => 'biblical',
  );
  return (object) array_merge($text, $edit);
}

function _nurani_library_validate_text($text, $element, $validate_empty = FALSE) {
  if ($validate_empty) {
    if (trim($text['name']) === '') {
      form_error($element['name'], t('!name field is required.', array('!name' => 'name')));
    }
    if (trim($text['full_name']) === '') {
      form_error($element['full_name'], t('!name field is required.', array('!name' => 'full_name')));
    }
    return;
  }
  if ($text['name'] === '' && $text['full_name'] === '') {
    return;
  }

  $dupe = db_query("SELECT 1
                      FROM {nurani_library_corpora}
                     WHERE name = :name
                       AND language = :language
                       AND id <> :id",
                   array(
                     ':name' => $text['name'],
                     ':language' => $text['language'],
                     ':id' => $text['id'],
                   ))->fetchCol();

  if ($dupe) {
    form_set_error($element['name'], t("This <em>name + language</em> combination %name + %language already exists.", array('%name' => $text['name'], '%language' => $text['language'])));
  }
  if (preg_match('@[^a-z0-9_]+@', $text['name'])) {
    form_error($element['name'], t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
  }
}

function _nurani_library_save_text($text) {
  $record = (object) $text;
  $is_new = ($record->id === 'new');
  unset($record->actions);

  if ($is_new) {
    unset($record->id);
  }

  drupal_write_record('nurani_library_corpora', $record, ($is_new ? array() : array('id')));
}

/**
 * Nurani Library administrative form for importing texts.
 */
function nurani_library_import_form($form) {
  drupal_add_js(drupal_get_path('module', 'nurani_library') . '/nurani_library.js');

  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import texts'),
    '#tree' => TRUE,
    'table' => array(
      '#theme' => 'nurani_library_import_form_table',
      '#parents' => array('import'),
    )
  );

  $path = nurani_library_texts_path();
  $row = 0;
  foreach (nurani_library_find_texts($path) as $text) {
    $form['import']['table'][$row]['#text'] = $text;
    $form['import']['table'][$row]['text'] = array();
    $form['import']['table'][$row]['import'] = array();

    if ($text->is_dir) {
      // TODO: Finish whole directory selection / import code
    }
    else {
      $form['import']['table'][$row]['text'] = array(
        '#type' => 'value',
        '#value' => $text,
      );
      $form['import']['table'][$row]['import'] = array(
        '#type' => 'checkbox',
        '#title' => t('Import?'),
        '#default_value' => FALSE,
      );
    }
    $row += 1;
  }

  // No texts were found
  if ($row == 0) {
    $form['import']['no_texts'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="messages error">'
                 .   '<h2 class="element-invisible">Error message</h2>'
                 .   t("No !osis or !tanzil texts could be found in %path", array(
                       '!osis'   => nurani_library_text_format_link('osis'),
                       '!tanzil' => nurani_library_text_format_link('tanzil'),
                       '%path'   => $path
                     ))
                 . '</div>',
    );
  }
  else {
    $form['import']['text'] = array(
      '#type' => 'select',
      '#title' => t('Import into this text'),
      '#options' => nurani_library_text_options(),
      '#required' => TRUE,
    );

    $form['import']['import'] = array(
      '#type' => 'submit',
      '#value' => t('(Re)import selected texts'),
    );
  }

  return $form;
}

/**
 * Theme function to render the Import texts table form elements.
 */
function theme_nurani_library_import_form_table(&$variables) {
  $output = '';

  $rows = array();
  $header = array(t("File"), t("Format"), t("Import status"));
  $element = &$variables['element'];

  foreach (element_children($element) as $row) {
    $text = $element[$row]['#text'];
    unset($element[$row]['import']['#title']);

    $row_classes = array();
    $row_classes[] = 'nurani-library-row-' . ($text->is_dir ? 'directory' : 'file');
    if ($text->format) {
      $row_classes[] = 'nurani-library-row-' . $text->format;
    }
    $row_classes[] = 'nurani-library-row-depth-' . $text->depth;

    $rows[] = array(
      'class' => $row_classes,
      'data' => array(
        array(
          'class' => array('nurani-library-col-import'),
          'data' => drupal_render($element[$row]['import']) . ' <span>' . check_plain($text->filepath) . '</span>',
        ),
        nurani_library_text_format_link($text->format),
        '',
      )
    );
  }

  drupal_add_css(drupal_get_path('module', 'nurani_library') . '/nurani_library.css');
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('nurani-library-import-form-table'))));
}

/**
 * Validation handler for import form
 */
function nurani_library_import_form_validate($form, &$form_state) {
  $import = $form_state['values']['import'];

  $texts = array();
  $format = NULL;
  foreach ($import as $row => $data) {
    if (!is_array($data) || $data['import'] != 1) {
      continue;
    }

    $format = is_null($format) ? $data['text']->format : $format;
    if ($data['text']->format != $format) {
      form_error($form['import']['table'][$row], t("!format1 and !format2 texts cannot be imported at the same time.", array('!format1' => nurani_library_text_format_link($format), '!format2' => nurani_library_text_format_link($data['text']->format))));
      break;
    }

    $texts[] = $data['text'];
  }

  $form_state['storage']['texts'] = $texts;
}

/**
 * Submission handler for import form
 */
function nurani_library_import_form_submit($form, &$form_state) {
  $corpus = nurani_library_text_load($form_state['values']['import']['text']);
  $texts = $form_state['storage']['texts'];

  $import[$corpus->name] = array(
    'path'          => nurani_library_texts_path(),
    'documentType'  => 'OSIS',
    'language'      => $corpus->language,
    // TODO: textDirection should not be hard-coded.
    'textDirection' => in_array($corpus->language, array('he', 'ar')) ? 'rtl' : 'ltr',
    'stripMarkup'   => TRUE,
    // NOTE: Character stripping should not be hard-coded.
    'stripChars'    => $corpus->language == 'he' ? '/' : '',
    'files' => array(),
  );

  foreach ($texts as $text) {
    $import[$corpus->name]['files'][] = $text->relativepath;
  }

  $library = new NuraniLibrary(array('backend' => 'Drupal'));
  $library->import($import);
}
