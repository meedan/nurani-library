<?php

/**
 * Nurani Library administrative settings form.
 */
function nurani_library_admin_form($form, &$form_state) {
  $form['connection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nurani Library data connection'),
  );
  $form['connection']['nurani_library_backend'] = array(
    '#type' => 'select',
    '#title' => t('Backend type'),
    '#default_value' => variable_get('nurani_library_backend', 'REST'),
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

  $form['tests'] = array(
    '#type' => 'item',
    '#title' => t("Connection status"),
    '#markup' => nurani_library_tests(),
  );

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
  $form['nurani_library_rest_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug enabled?'),
    '#default_value' => $settings['debug'],
  );

  return $form;
}

/**
 * Returns an HTML table of REST tests.
 */
function nurani_library_tests() {
  $nl = nurani_library();

  $header = array(t("Test"), t("Status"));
  $rows = array();

  $rows[] = array(
    t('Test fetching all works'),
    var_export($nl->getWorks(), 1),
  );
  $rows[] = array(
    t('Test fetching the work "wlc_he"'),
    var_export($nl->getWork('wlc_he'), 1),
  );
  $rows[] = array(
    t('Test fetching passages from Genesis in work "wlc_he"'),
    var_export($nl->search('wlc_he', 'Gen', 1, '1-5'), 1),
  );

  return theme('table', array('header' => $header, 'rows' => $rows));
}
