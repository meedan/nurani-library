<?php

/**
 * Nurani Library administrative settings form.
 */
function nurani_library_admin_form($form, &$form_state) {
  $form['connection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nurani Library data connection'),
  );
  $form['connection']['backend'] = array(
    '#type' => 'select',
    '#title' => t('Backend type'),
    '#default_value' => variable_get('nurani_library_backend', 'Drupal'),
    '#options' => nurani_library_backend_options(),
    '#required' => TRUE,
  );

  foreach (nurani_library_backends() as $backend) {
    if (isset($backend['settings'])) {
      if (isset($backend['settings']['file'])) {
        require_once $backend['settings']['file'];
      }

      $form[$backend['name']] = array(
        '#type' => 'fieldset',
        '#title' => $backend['name'],

        'settings' => $backend['settings']['callback']($form_state),
      );
    }
  }

  return system_settings_form($form);
}

/**
 * Settings callback form for hook_nurani_library_backend()
 */
function nurani_library_rest_settings_form(&$form_state) {
  $form = array();
  $settings = nurani_library_rest_settings();

  $form['nurani_library_rest_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Protocol'),
    '#default_value' => $settings['scheme'],
    '#options' => array('http' => t("HTTP"), 'https' => t("HTTPS")),
    '#required' => TRUE,
  );
  $form['nurani_library_rest_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#size' => 60,
    '#maxlength' => 1024,
    '#default_value' => $settings['host'],
    '#required' => TRUE,
  );
  $form['nurani_library_rest_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#size' => 4,
    '#maxlength' => 8,
    '#default_value' => $settings['port'],
    '#required' => TRUE,
  );
  $form['nurani_library_rest_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('Path prefix to add to all REST requests, eg: http://example.com/<strong style="color: #f00;">api-prefix</strong>/passage/wlc'),
    '#size' => 4,
    '#maxlength' => 8,
    '#default_value' => $settings['path'],
    '#required' => TRUE,
  );
  $form['tests'] = array(
    '#type' => 'item',
    '#title' => t("REST connection status"),
    '#markup' => nurani_library_rest_tests(),
  );

  return $form;
}

/**
 * Returns an HTML table of REST tests.
 */
function nurani_library_rest_tests() {
  $nl = nurani_library();

  $header = array(t("Test"), t("Status"));
  $rows = array();

  $rows[] = array(
    t('Test fetching all works'),
    var_export($nl->getWorks(), 1),
  );

  return theme('table', array('header' => $header, 'rows' => $rows));
}
