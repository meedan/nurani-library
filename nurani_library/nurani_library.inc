<?php

function nurani_library() {
  // TODO: Need to pass information regarding whether this is a local or remote connection.
  return new NuraniLibrary(array('backend' => 'Drupal'));
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

function nurani_library_work_info($work_name) {
}
