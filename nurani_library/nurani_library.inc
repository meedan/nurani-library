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
  );
}
