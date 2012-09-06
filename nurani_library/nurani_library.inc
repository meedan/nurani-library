<?php

function nurani_library() {
  $settings = array();
  $settings['backend'] = variable_get('nurani_library_backend', 'REST');

  // HACK: FIXME: This should be gracefully determined with a callback in hook_nurani_library_backend()
  if ($settings['backend'] == 'REST') {
    $settings['connection'] = nurani_library_rest_settings();
  }

  return new NuraniLibrary($settings);
}

function nurani_library_work_format_link($format) {
  switch ($format) {
    case 'OSIS':   return l('OSIS', 'http://www.crosswire.org/osis/');
    case 'TanzilXML': return l('Tanzil XML', 'http://tanzil.net/download/');
    default:       return '';
  }
}

function nurani_library_work_options() {
  $nl = nurani_library();
  $options = array();
  foreach ($nl->getWorks() as $work) {
    $options[$work->name] = $work->full_name;
  }
  return $options;
}

function nurani_library_backends() {
  $backends = array();

  foreach (module_implements('nurani_library_backend') as $module) {
    $function = $module . '_nurani_library_backend';
    $backends = array_merge($backends, $function());
  }

  return $backends;
}

function nurani_library_backend_options() {
  $options = array();

  foreach (nurani_library_backends() as $backend) {
    $options[$backend['name']] = $backend['description'];
  }

  return $options;
}

function nurani_library_rest_settings() {
  return array(
    'scheme' => variable_get('nurani_library_rest_scheme', 'http'),
    'host'   => variable_get('nurani_library_rest_host', 'library.nurani.org'),
    'port'   => variable_get('nurani_library_rest_port', 80),
    'path'   => variable_get('nurani_library_rest_path', 'api'),
    'debug'  => variable_get('nurani_library_rest_debug', '0'),
    'user'   => variable_get('nurani_library_rest_user', 'API'),
    'pass'   => variable_get('nurani_library_rest_pass', ''),
  );
}

/**
 * Creates an account named 'API' for use with REST Services.
 */
function nurani_library_create_API_account() {
  // Create the API account with a random password, store the password in the
  // database temporarily.  It is up to the administrator to remove the
  // nurani_library_api_user_pass variable for security.
  $pass = user_password();
  $api_role = user_role_load_by_name('API');

  // The API role is stored in a feature, but when the site is installing the
  // order the features are installed can cause this to not exist yet.  Creating
  // it here causes no issues.
  if (!$api_role || $api_role->rid <= 0) {
    // Create a default role for site administrators, with all available permissions assigned.
    $api_role = new stdClass();
    $api_role->name = 'API';
    $api_role->weight = 3;
    user_role_save($api_role);
  }

  $new_user = array(
    'name' => 'API',
    'pass' => $pass,
    'mail' => 'api-no-reply@' . $_SERVER['SERVER_NAME'],
    'init' => 'api-no-reply@' . $_SERVER['SERVER_NAME'],
    'status' => 1,
    'access' => time(),
    'roles' => array(
      DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      $api_role->rid => $api_role->name,
    ),
  );

  $account = user_save(null, $new_user);
  if ($account && $account->uid > 0) {
    variable_set('nurani_library_api_user_pass', $pass);
    drupal_set_message(t('Nurani Library API account created successfully. Write down the account password <strong>!pass</strong> in a safe place then delete the <em>nurani_library_api_user_pass</em> variable.', array('!pass' => $pass)), 'warning');
  }
}
