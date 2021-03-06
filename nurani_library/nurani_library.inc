<?php

function nurani_library() {
  $settings = array();
  $settings['backend'] = nurani_library_backend();

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

/**
 * Returns an array of passages and passage information using OSIS ID
 * information.
 */
function nurani_library_get_passages($osisIDWork, $osisID) {
  if (trim($osisIDWork) == '' || trim($osisID) == '') {
    return FALSE;
  }

  $osis = explode('.', $osisID);
  if (count($osis) != 3) {
    return FALSE;
  }

  $nl = nurani_library();
  $passages = $nl->search($osisIDWork, $osis[0], $osis[1], $osis[2]);

  if ($nl->model->getError()) {
    return FALSE;
  }

  return $passages;
}

/**
 * Returns extra information along with an array of passages using OSIS ID
 * information.
 */
function nurani_library_get_passages_info($osisIDWork, $osisID) {
  $passages = nurani_library_get_passages($osisIDWork, $osisID);

  if ($passages === FALSE || count($passages) <= 0) {
    return FALSE;
  }

  $last  = end($passages);
  $first = reset($passages);

  $verse = $first->verse . ($last->verse != $first->verse ? '-' . $last->verse : '');

  return array(
    'passage_human' => "$first->book_full_name $first->chapter_full_name:$verse",
    'work_full_name' => $first->work_full_name,
    'passages' => $passages,
  );
}

function nurani_library_backend() {
  return variable_get('nurani_library_backend', 'REST');
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
    'scheme'    => variable_get('nurani_library_rest_scheme', 'http'),
    'auth_user' => variable_get('nurani_library_rest_auth_user', ''),
    'auth_pass' => variable_get('nurani_library_rest_auth_pass', ''),
    'host'      => variable_get('nurani_library_rest_host', 'library.nurani.org'),
    'port'      => variable_get('nurani_library_rest_port', 80),
    'path'      => variable_get('nurani_library_rest_path', 'api'),
    'debug'     => variable_get('nurani_library_rest_debug', '0'),
    'user'      => variable_get('nurani_library_rest_user', 'API'),
    'pass'      => variable_get('nurani_library_rest_pass', ''),
  );
}

function nurani_library_l() {
  $args = func_get_args();
  $backend = nurani_library_backend();

  if ($backend == 'REST') {
    $args[0] = nurani_library_base_url() . '/' . $args[0];
  }
  $args[2]['absolute'] = TRUE;

  return call_user_func_array('l', $args);
}

function nurani_library_base_url($no_authentication = FALSE, $add_path = FALSE, $force_absolute = FALSE) {
  $backend = nurani_library_backend();

  if ($backend == 'REST') {
    $settings = nurani_library_rest_settings();

    $url  = $settings['scheme'] . '://';
    if (!$no_authentication) {
      $url .= ($settings['auth_user']) ? $settings['auth_user'] . ($settings['auth_pass'] ? ':' . $settings['auth_pass'] . '' : '') . '@' : '';
    }
    $url .= $settings['host'];
    $url .= $settings['port'] != 80 ? ':' . $settings['port'] : '';
  }
  else {
    $url = preg_replace('@[/]*$@', '', url('', array('absolute' => $force_absolute)));
  }

  if ($add_path) {
    $url .= '/' . (isset($settings) ? $settings['path'] : 'api');
  }

  return $url;
}

function nurani_library_format_passage_string_from_passages($passages) {
  $first = reset($passages);
  $last  = FALSE;

  if (count($passages) > 1) {
    $last = end($passages);
  }

  $title  = ucfirst($first->book_full_name); // HACK: This is really lazy, we need a human book full name for the Qur'an
  $title .= ' ';
  $title .= $first->chapter_name;
  $title .= ':';
  $title .= $first->verse;
  if ($last) {
    $title .= '-' . $last->verse;
  }

  return $title;
}
