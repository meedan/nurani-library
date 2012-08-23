<?php

/**
 * Nurani Library administrative settings form.
 */
function nurani_library_admin_form($form, &$form_state) {
  $form['nurani_library_works_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Nurani Library works path'),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => nurani_library_works_path(),
    '#description' => t("Path to the directory which contains raw works. "
                      . "Currently supported formats are !osis_link and "
                      . "!tanzil_link.  Works may reside in subdirectories of "
                      . "this path.", array(
                          '!osis_link' => nurani_library_work_format_link('osis'),
                          '!tanzil_link' => nurani_library_work_format_link('tanzil'),
                        )),
    '#required' => FALSE,
  );
  $form = system_settings_form($form);
  return $form;
}

/**
 * Nurani Library administrative form for managing works.
 */
function nurani_library_works_form($form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'nurani_library') . '/nurani_library.js');

  $form['works'] = array(
    '#type' => 'fieldset',
    '#title' => t('Works'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    'table' => array(
      '#theme' => 'nurani_library_works_form_table',
      '#parents' => array('works'),
    )
  );

  $select_col_options = array(
    // TODO: The list of languages should not be hard-coded.
    'language' => nurani_library_work_languages(),
    'type'     => nurani_library_work_type_options(),
  );
  $descriptions = array(
    'name' => t('The machine-readable name. Can only contain lowercase letters, numbers, and underscores.'),
    'full_name' => t("The human readable title of the work."),
    'language' => t("The language this version of the work is written in."),
  );
  $row = 0;
  $works = nurani_library_works();
  $works[] = _nurani_library_new_work($form_state);
  foreach ($works as $work) {
    $form['works'][$row] = array(
      '#work' => $work,
    );

    // Ensure the new record is clearly separated from the existing records
    if ($work->id == 'new') {
      $row = 'new';
    }

    foreach ($work as $col => $value) {
      switch ($col) {
        case 'id':
          $form['works']['table'][$row][$col] = array(
            '#type' => 'value',
            '#value' => $value,
          );
          break;

        case 'name':
          if ($row !== 'new') {
            $form['works']['table'][$row][$col] = array(
              '#type' => 'markup',
              '#markup' => '<em>' . check_plain($value) . '</em>',
              'name' => array(
                '#type' => 'value',
                '#value' => $value,
                '#parents' => array('works', $row, $col),
              ),
            );
          }
          else {
            $form['works']['table'][$row][$col] = array(
              '#type' => 'textfield',
              '#size' => 16,
              '#maxlength' => 32,
            );
          }
          break;

        case 'type':
        case 'language':
          $form['works']['table'][$row][$col] = array(
            '#type' => 'select',
            '#options' => $select_col_options[$col],
          );
          break;

        default: 
          $form['works']['table'][$row][$col] = array(
            '#type' => 'textfield',
            '#size' => 16,
            '#maxlength' => 64,
          );
          break;
      }

      $form['works']['table'][$row][$col]['#title'] = $col;
      $form['works']['table'][$row][$col]['#description'] = isset($descriptions[$col]) ? $descriptions[$col] : NULL;
      $form['works']['table'][$row][$col]['#default_value'] = $value;
      $form['works']['table'][$row][$col]['#required'] = ($row !== 'new');
      $form['works']['table'][$row][$col]['#row'] = $row;
    }

    $form['works']['table'][$row]['actions'] = array();
    if ($row === 'new') {
      $form['works']['table'][$row]['actions']['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add new work'),
        '#validate' => array('nurani_library_works_form_add_validate'),
        '#submit' => array('nurani_library_works_form_add_submit'),
      );
    }
    else {
      $form['works']['table'][$row]['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array('onclick' => "return confirm('" . t("Are you sure? This will delete ALL passages associated with this work. Also, any references to those passages will break.") . "');"),
        '#validate' => array('nurani_library_works_form_delete_validate'),
        '#submit' => array('nurani_library_works_form_delete_submit'),
        '#work' => $work,
      );
    }

    $row += 1;
  }

  $form['works']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Theme function to render the form elements for each Work as a table.
 */
function theme_nurani_library_works_form_table(&$variables) {
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
        'class' => array('nurani-library-works-col-' . $col),
        'data' => drupal_render($element[$row][$col]),
      );
    }
    $first_row = FALSE;
  }

  drupal_add_css(drupal_get_path('module', 'nurani_library') . '/nurani_library.css');
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('nurani-library-works-form-table'))));
}

/**
 * Validation handler for works management form
 */
function nurani_library_works_form_validate($form, &$form_state) {
  $works = $form_state['values']['works'];
  if (trim($works['new']['name']) === '') {
    unset($works['new'], $form_state['values']['works']['new']);
  }
  foreach ($works as $row => $work) {
    if (!is_array($work) || !isset($work['name'])) {
      continue;
    }
    _nurani_library_validate_work($work, $form['works']['table'][$row]);
  }
}

/**
 * Submission handler for works management form
 */
function nurani_library_works_form_submit($form, &$form_state) {
  foreach ($form_state['values']['works'] as $row => $work) {
    if (!is_array($work) || !isset($work['name'])) {
      continue;
    }
    if ($row === 'new') {
      drupal_set_message(t("Saved new work record."));
    }
    _nurani_library_save_work($work);
  }

  drupal_set_message(t("Updated existing work records."));
}

function nurani_library_works_form_add_validate($form, &$form_state) {
  $work = $form_state['values']['works']['new'];
  _nurani_library_validate_work($work, $form['works']['table']['new'], TRUE);
}

function nurani_library_works_form_add_submit($form, &$form_state) {
  $work = $form_state['values']['works']['new'];
  _nurani_library_save_work($work);
  drupal_set_message(t("Saved new work record."));
}

function nurani_library_works_form_delete_validate($form, &$form_state) {
  // Nothing to do currently.
}

function nurani_library_works_form_delete_submit($form, &$form_state) {
  $work = $form_state['clicked_button']['#work'];
  if (!$work) {
    drupal_set_message(t("There was an error deleting this work. Please try again."), 'error');
  }
  else {
    $nl = nurani_library();
    $nl->model->deleteWork($work->id);
    drupal_set_message(t("The work %full_name (%name) and all of its passages were removed from the system.", array('%full_name' => $work->full_name, '%name' => $work->name)));
  }
}

function _nurani_library_new_work($form_state) {
  $edit = isset($form_state['values']['works']['new']) ? $form_state['values']['works']['new'] : array();
  $work = array(
    'id'        => 'new',
    'name'      => '',
    'full_name' => '',
    'language'  => 'en',
    'type'      => 'biblical',
  );
  return (object) array_merge($work, $edit);
}

function _nurani_library_validate_work($work, $element, $validate_empty = FALSE) {
  if ($validate_empty) {
    if (trim($work['name']) === '') {
      form_error($element['name'], t('!name field is required.', array('!name' => 'name')));
    }
    if (trim($work['full_name']) === '') {
      form_error($element['full_name'], t('!name field is required.', array('!name' => 'full_name')));
    }
    return;
  }
  if ($work['name'] === '' && $work['full_name'] === '') {
    return;
  }

  $dupe = db_query("SELECT 1
                      FROM {nurani_library_works}
                     WHERE name = :name
                       AND language = :language
                       AND id <> :id",
                   array(
                     ':name' => $work['name'],
                     ':language' => $work['language'],
                     ':id' => $work['id'],
                   ))->fetchCol();

  if ($dupe) {
    form_set_error($element['name'], t("This <em>name + language</em> combination %name + %language already exists.", array('%name' => $work['name'], '%language' => $work['language'])));
  }
  if (preg_match('@[^a-z0-9_]+@', $work['name'])) {
    form_error($element['name'], t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
  }
}

function _nurani_library_save_work($work) {
  $record = (object) $work;
  $is_new = ($record->id === 'new');
  unset($record->actions);

  if ($is_new) {
    unset($record->id);
  }

  drupal_write_record('nurani_library_works', $record, ($is_new ? array() : array('id')));
}

/**
 * Nurani Library administrative form for importing works.
 */
function nurani_library_import_form($form) {
  drupal_add_js(drupal_get_path('module', 'nurani_library') . '/nurani_library.js');

  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import works'),
    '#tree' => TRUE,
    'table' => array(
      '#theme' => 'nurani_library_import_form_table',
      '#parents' => array('import'),
    )
  );

  $path = nurani_library_works_path();
  $row = 0;
  foreach (nurani_library_find_works($path) as $work) {
    $form['import']['table'][$row]['#work'] = $work;
    $form['import']['table'][$row]['work'] = array();
    $form['import']['table'][$row]['import'] = array();

    if ($work->is_dir) {
      // TODO: Finish whole directory selection / import code
    }
    else {
      $form['import']['table'][$row]['work'] = array(
        '#type' => 'value',
        '#value' => $work,
      );
      $form['import']['table'][$row]['import'] = array(
        '#type' => 'checkbox',
        '#title' => t('Import?'),
        '#default_value' => FALSE,
      );
    }
    $row += 1;
  }

  // No works were found
  if ($row == 0) {
    $form['import']['no_works'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="messages error">'
                 .   '<h2 class="element-invisible">Error message</h2>'
                 .   t("No !osis or !tanzil works could be found in %path", array(
                       '!osis'   => nurani_library_work_format_link('osis'),
                       '!tanzil' => nurani_library_work_format_link('tanzil'),
                       '%path'   => $path
                     ))
                 . '</div>',
    );
  }
  else {
    $form['import']['work'] = array(
      '#type' => 'select',
      '#title' => t('Import into this work'),
      '#options' => nurani_library_work_options(),
      '#required' => TRUE,
    );
    if (empty($form['import']['work']['#options'])) {
      drupal_set_message(t("Please <a href=\"!url\">create a work record first</a>. It is not possible to import without a work record.", array('!url' => url('admin/structure/nurani-library/works'))), 'error');
    }
    $form['import']['import'] = array(
      '#type' => 'submit',
      '#value' => t('(Re)import selected works'),
    );
  }

  return $form;
}

/**
 * Theme function to render the Import works table form elements.
 */
function theme_nurani_library_import_form_table(&$variables) {
  $output = '';

  $rows = array();
  $header = array(t("File"), t("Format"), t("Import status"));
  $element = &$variables['element'];

  foreach (element_children($element) as $row) {
    $work = $element[$row]['#work'];
    unset($element[$row]['import']['#title']);

    $row_classes = array();
    $row_classes[] = 'nurani-library-row-' . ($work->is_dir ? 'directory' : 'file');
    if ($work->format) {
      $row_classes[] = 'nurani-library-row-' . $work->format;
    }
    $row_classes[] = 'nurani-library-row-depth-' . $work->depth;

    $rows[] = array(
      'class' => $row_classes,
      'data' => array(
        array(
          'class' => array('nurani-library-col-import'),
          'data' => drupal_render($element[$row]['import']) . ' <span>' . check_plain($work->filepath) . '</span>',
        ),
        nurani_library_work_format_link($work->format),
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

  $works = array();
  $format = NULL;
  foreach ($import as $row => $data) {
    if (!is_array($data) || $data['import'] != 1) {
      continue;
    }

    $format = is_null($format) ? $data['work']->format : $format;
    if ($data['work']->format != $format) {
      form_error($form['import']['table'][$row], t("!format1 and !format2 works cannot be imported at the same time.", array('!format1' => nurani_library_work_format_link($format), '!format2' => nurani_library_work_format_link($data['work']->format))));
      break;
    }

    $works[] = $data['work'];
  }

  $form_state['storage']['works'] = $works;
}

/**
 * Submission handler for import form
 */
function nurani_library_import_form_submit($form, &$form_state) {
  $work = nurani_library_work_load($form_state['values']['import']['work']);
  $works = $form_state['storage']['works'];

  $import[$work->name] = array(
    'path'          => nurani_library_works_path(),
    'documentType'  => 'OSIS',
    'language'      => $work->language,
    // TODO: textDirection should not be hard-coded.
    'textDirection' => in_array($work->language, array('he', 'ar')) ? 'rtl' : 'ltr',
    'stripMarkup'   => TRUE,
    // NOTE: Character stripping should not be hard-coded.
    'stripChars'    => $work->language == 'he' ? '/' : '',
    'files' => array(),
  );

  foreach ($works as $work) {
    $import[$work->name]['files'][] = $work->relativepath;
  }

  $library = new NuraniLibrary(array('backend' => 'Drupal'));
  $library->import($import);
}
