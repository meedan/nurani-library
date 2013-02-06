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
  $form['tests'] = array(
    '#type' => 'item',
    '#title' => t("%backend Connection status", array('%backend' => variable_get('nurani_library_backend', 'REST'))),
    '#markup' => nurani_library_tests(),
  );

  return system_settings_form($form);
}

/**
 * Returns an HTML table of REST tests.
 */
function nurani_library_tests() {
  $nl = nurani_library();

  $header = array(t("Test"), t("Data"), t("Status"));
  $rows = array();

  $result = $nl->getWorks();
  $error  = $nl->model->getError();
  $rows[] = array(
    'data' => array(
      t('Test fetching all works'),
      '<pre>' . var_export($result, 1) . '</pre>',
      is_null($error) ? t('Pass') : '<pre>' . var_export($error, 1) . '</pre>',
    ),
    'class' => array(is_null($error) ? 'test-pass' : 'test-fail'),
  );

  $result = $nl->getWork('wlc_he');
  $error  = $nl->model->getError();
  $rows[] = array(
    'data' => array(
      t('Test fetching the work "wlc_he"'),
      '<pre>' . var_export($result, 1) . '</pre>',
      is_null($error) ? t('Pass') : '<pre>' . var_export($error, 1) . '</pre>',
    ),
    'class' => array(is_null($error) ? 'test-pass' : 'test-fail'),
  );

  $result = $nl->search('wlc_he', 'Gen', 1, '10-15');
  $error  = $nl->model->getError();
  $rows[] = array(
    'data' => array(
      t('Test fetching passages wlc_he:Gen.1.10-15'),
      '<pre>' . var_export($result, 1) . '</pre>',
      is_null($error) ? t('Pass') : '<pre>' . var_export($error, 1) . '</pre>',
    ),
    'class' => array(is_null($error) ? 'test-pass' : 'test-fail'),
  );

  $result = $nl->getAnnotations(NULL, NULL, NULL, 0, 5);
  $error  = $nl->model->getError();
  $rows[] = array(
    'data' => array(
      t('Test fetching notes 0-5'),
      '<pre>' . var_export($result, 1) . '</pre>',
      is_null($error) ? t('Pass') : '<pre>' . var_export($error, 1) . '</pre>',
    ),
    'class' => array(is_null($error) ? 'test-pass' : 'test-fail'),
  );

  drupal_add_css(drupal_get_path('module', 'nurani_library') . '/nurani_library.css');
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('nurani-library-tests'))));
}

/**
 * Settings callback form for hook_nurani_library_backend()
 */
function nurani_library_rest_settings_form($form, &$form_state) {
  $settings = nurani_library_rest_settings();
  $form['nurani_library_rest_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Protocol'),
    '#default_value' => $settings['scheme'],
    '#options' => array('http' => t("HTTP"), 'https' => t("HTTPS")),
    '#required' => TRUE,
  );
  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTTP authentication'),
  );
  $form['authentication']['nurani_library_rest_auth_user'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP authentication user'),
    '#size' => 10,
    '#maxlength' => 128,
    '#default_value' => $settings['auth_user'],
    '#required' => FALSE,
  );
  $form['authentication']['nurani_library_rest_auth_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP authentication password'),
    '#size' => 10,
    '#maxlength' => 128,
    '#default_value' => $settings['auth_pass'],
    '#required' => FALSE,
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
    '#description' => t('All REST communication will be logged using <a href="!url">watchdog</a>.', array('!url' => 'http://api.drupal.org/api/drupal/includes%21bootstrap.inc/function/watchdog/7')),
    '#default_value' => $settings['debug'],
  );
  $form['nurani_library_rest_user'] = array(
    '#type' => 'textfield',
    '#title' => t('API User @ @host', array('@host' => $settings['host'])),
    '#size' => 8,
    '#maxlength' => 60,
    '#default_value' => $settings['user'],
    '#required' => TRUE,
  );
  $form['nurani_library_rest_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('API Password @ @host', array('@host' => $settings['host'])),
    '#size' => 16,
    '#maxlength' => 128,
    '#default_value' => $settings['pass'],
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Confirm form to create a missing Nurani Library API account.
 */
function nurani_library_create_api_account_form($form, &$form_state) {
  $account = user_load_by_name('API');

  if ($account && $account->uid > 0) {
    drupal_set_message(t('API account exists. If you wish to re-create it, please delete it first.'));
    drupal_goto();
    return;
  }

  return confirm_form(
    $form,
    t('Are you sure you want to create the Nurani Library API account?'),
    'admin/structure/nurani-library',
    '',
    t('Create API account'),
    t('Cancel')
  );
}

/**
 * Submission callback for Nurani Library API account creation confirm form.
 */
function nurani_library_create_api_account_form_submit($form, &$form_state) {
  module_load_install('nurani_library');
  nurani_library_create_API_account();
  $form_state['redirect'] = 'admin/structure/nurani-library';
}
