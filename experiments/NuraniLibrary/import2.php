<?php

require_once 'NuraniLibrary.php';

$import = array();

// Tanzil Quran Text
$import['Quran'] = array(
  'path'          => dirname(__FILE__) . '/data/quran',
  'documentType'  => 'TanzilXML',
  'workType'      => 'quran',
  'language'      => 'ar',
  'textDirection' => 'rtl',
  'stripMarkup'   => TRUE,
  'stripChars'    => '',
  'books' => array(
    'quran' => 'quran-simple-enhanced.xml'
  ),
);


$library = new NuraniLibrary(array('backend' => 'Drupal'));
$library->import($import);
