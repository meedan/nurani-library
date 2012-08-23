<?php

/**
 * Nurani Library administrative settings form.
 */
function nurani_library_provider_admin_form($form, &$form_state) {
  $form['nurani_library_provider_works_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Nurani Library works path'),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => nurani_library_provider_works_path(),
    '#description' => t("Path to the directory which contains raw works. "
                      . "Currently supported formats are !osis_link and "
                      . "!tanzil_link.  Works may reside in subdirectories of "
                      . "this path.", array(
                          '!osis_link' => nurani_library_work_format_link('OSIS'),
                          '!tanzil_link' => nurani_library_work_format_link('TanzilXML'),
                        )),
    '#required' => FALSE,
  );
  $form = system_settings_form($form);
  return $form;
}

/**
 * Nurani Library administrative form for managing works.
 */
function nurani_library_provider_works_form($form, &$form_state) {
  $nw = new NuraniWork();
  $nl = nurani_library();

  $form['works'] = array(
    '#type' => 'fieldset',
    '#title' => t('Works'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    'table' => array(
      '#theme' => 'nurani_library_provider_works_form_table',
      '#parents' => array('works'),
    )
  );

  $select_col_options = array(
    // TODO: The list of languages should not be hard-coded.
    'language' => $nw->workLanguageOptions,
    'type'     => $nw->workTypeOptions,
  );
  $descriptions = array(
    'name'      => t('A unique machine-readable name. Can only contain lowercase letters, numbers, and underscores.'),
    'full_name' => t("The human readable title of the work."),
    'language'  => t("The language this version of the work is written in."),
  );
  $row = 0;
  $works = $nl->getWorks();
  $works[] = _nurani_library_provider_new_work($form_state);
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

    $info = $nl->getWorkStats($work->name);
    $form['works']['table'][$row]['info'] = array(
      '#type' => 'markup',
      '#markup' => theme('item_list', array(
        'items' => array(
            t("<strong>Passages: </strong> @passages", array('@passages' => (string) $info['num_passages'])),
          )
        )
      ),
    );

    $form['works']['table'][$row]['actions'] = array();
    if ($row === 'new') {
      $form['works']['table'][$row]['info'] = array();
      $form['works']['table'][$row]['actions']['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add new work'),
        '#validate' => array('nurani_library_provider_works_form_add_validate'),
        '#submit' => array('nurani_library_provider_works_form_add_submit'),
      );
    }
    else {
      $form['works']['table'][$row]['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#attributes' => array('onclick' => "return confirm('" . t("Are you sure? This will delete ALL passages associated with this work. Also, any references to those passages will break.") . "');"),
        '#validate' => array('nurani_library_provider_works_form_delete_validate'),
        '#submit' => array('nurani_library_provider_works_form_delete_submit'),
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
function theme_nurani_library_provider_works_form_table(&$variables) {
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
        'class' => array('nurani-library-provider-works-col-' . $col),
        'data' => drupal_render($element[$row][$col]),
      );
    }
    $first_row = FALSE;
  }

  drupal_add_css(drupal_get_path('module', 'nurani_library_provider') . '/nurani_library_provider.css');
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('nurani-library-provider-works-form-table'))));
}

/**
 * Validation handler for works management form
 */
function nurani_library_provider_works_form_validate($form, &$form_state) {
  $works = $form_state['values']['works'];
  if (trim($works['new']['name']) === '') {
    unset($works['new'], $form_state['values']['works']['new']);
  }
  foreach ($works as $row => $work) {
    if (!is_array($work) || !isset($work['name'])) {
      continue;
    }
    _nurani_library_provider_validate_work($work, $form['works']['table'][$row]);
  }
}

/**
 * Submission handler for works management form
 */
function nurani_library_provider_works_form_submit($form, &$form_state) {
  foreach ($form_state['values']['works'] as $row => $work) {
    if (!is_array($work) || !isset($work['name'])) {
      continue;
    }
    if ($row === 'new') {
      drupal_set_message(t("Saved new work record."));
    }
    _nurani_library_provider_save_work($work);
  }

  drupal_set_message(t("Updated existing work records."));
}

function nurani_library_provider_works_form_add_validate($form, &$form_state) {
  $work = $form_state['values']['works']['new'];
  _nurani_library_provider_validate_work($work, $form['works']['table']['new'], TRUE);
}

function nurani_library_provider_works_form_add_submit($form, &$form_state) {
  $work = $form_state['values']['works']['new'];
  _nurani_library_provider_save_work($work);
  drupal_set_message(t("Saved new work record."));
}

function nurani_library_provider_works_form_delete_validate($form, &$form_state) {
  // Nothing to do currently.
}

function nurani_library_provider_works_form_delete_submit($form, &$form_state) {
  $work = $form_state['clicked_button']['#work'];
  if (!$work) {
    drupal_set_message(t("There was an error deleting this work. Please try again."), 'error');
  }
  else {
    $nl = nurani_library_provider();
    $nl->model->deleteWork($work->id);
    drupal_set_message(t("The work %full_name (%name) and all of its passages were removed from the system.", array('%full_name' => $work->full_name, '%name' => $work->name)));
  }
}

function _nurani_library_provider_new_work($form_state) {
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

function _nurani_library_provider_validate_work($work, $element, $validate_empty = FALSE) {
  $nl = nurani_library();

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

  if ($nl->isDuplicateWork((object) $work)) {
    form_set_error($element['name'], t("The work %name already exists.", array('%name' => $work['name'])));
  }
  if (preg_match('@[^a-z0-9_]+@', $work['name'])) {
    form_error($element['name'], t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
  }
}

function _nurani_library_provider_save_work($work) {
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
function nurani_library_provider_import_form($form) {
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import works'),
    '#tree' => TRUE,
    'table' => array(
      '#theme' => 'nurani_library_provider_import_form_table',
      '#parents' => array('import'),
    )
  );

  $path = nurani_library_provider_works_path();
  $row = 0;
  foreach (nurani_library_provider_find_works($path) as $file) {
    $form['import']['table'][$row]['#file'] = $file;
    $form['import']['table'][$row]['file'] = array();
    $form['import']['table'][$row]['import'] = array();

    if ($file->is_dir) {
      // TODO: Finish whole directory selection / import code
    }
    else {
      $form['import']['table'][$row]['file'] = array(
        '#type' => 'value',
        '#value' => $file,
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
                       '!osis'   => nurani_library_work_format_link('OSIS'),
                       '!tanzil' => nurani_library_work_format_link('TanzilXML'),
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
      drupal_set_message(t("Please <a href=\"!url\">create a work record first</a>. It is not possible to import without a work record.", array('!url' => url('admin/structure/nurani-library/provider/works'))), 'error');
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
function theme_nurani_library_provider_import_form_table(&$variables) {
  $output = '';

  $rows = array();
  $header = array(t("File"), t("Format"), t("Import status"));
  $element = &$variables['element'];

  foreach (element_children($element) as $row) {
    $file = $element[$row]['#file'];
    unset($element[$row]['import']['#title']);

    $row_classes = array();
    $row_classes[] = 'nurani-library-provider-row-' . ($file->is_dir ? 'directory' : 'file');
    if ($file->format) {
      $row_classes[] = 'nurani-library-provider-row-' . $file->format;
    }
    $row_classes[] = 'nurani-library-provider-row-depth-' . $file->depth;

    $rows[] = array(
      'class' => $row_classes,
      'data' => array(
        array(
          'class' => array('nurani-library-provider-col-import'),
          'data' => drupal_render($element[$row]['import']) . ' <span>' . check_plain($file->filepath) . '</span>',
        ),
        nurani_library_work_format_link($file->format),
        '',
      )
    );
  }

  drupal_add_css(drupal_get_path('module', 'nurani_library_provider') . '/nurani_library_provider.css');
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('nurani-library-provider-import-form-table'))));
}

/**
 * Validation handler for import form
 */
function nurani_library_provider_import_form_validate($form, &$form_state) {
  $import = $form_state['values']['import'];

  $files = array();
  $format = NULL;
  foreach ($import as $row => $data) {
    if (!is_array($data) || $data['import'] != 1) {
      continue;
    }

    $format = is_null($format) ? $data['file']->format : $format;
    if ($data['file']->format != $format) {
      form_error($form['import']['table'][$row], t("!format1 and !format2 works cannot be imported at the same time.", array('!format1' => nurani_library_work_format_link($format), '!format2' => nurani_library_work_format_link($data['file']->format))));
      break;
    }

    $files[] = $data['file'];
  }

  $form_state['storage']['files'] = $files;
  $form_state['storage']['format'] = $format;
}

/**
 * Submission handler for import form
 */
function nurani_library_provider_import_form_submit($form, &$form_state) {
  $nl = nurani_library();
  $work = $nl->getWork($form_state['values']['import']['work']);
  $files = $form_state['storage']['files'];
  $format = $form_state['storage']['format'];

  $import[$work->name] = array(
    'path'          => nurani_library_provider_works_path(),
    'format'  => $format,
    'language'      => $work->language,
    // TODO: textDirection should not be hard-coded.
    'textDirection' => in_array($work->language, array('he', 'ar')) ? 'rtl' : 'ltr',
    'stripMarkup'   => TRUE,
    // NOTE: Character stripping should not be hard-coded.
    'stripChars'    => $work->language == 'he' ? '/' : '',
    'files' => array(),
  );

  foreach ($files as $file) {
    $import[$work->name]['files'][] = $file->relativepath;
  }

  $library = new NuraniLibrary(array('backend' => 'Drupal'));
  $library->import($import);
}
