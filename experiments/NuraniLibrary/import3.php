<?php

require_once 'NuraniLibrary.php';

$import = array();

// King James Version: 
$import['KJV'] = array(
  'path'          => dirname(__FILE__) . '/data/KJV',
  'documentType'  => 'OSIS',
  'language'      => 'en',
  'textDirection' => 'ltr',
  'stripMarkup'   => TRUE,
  'stripChars'    => '',
  'files' => array(
    'kjv_osis.xml'
  ),
);


$library = new NuraniLibrary(array('backend' => 'Drupal'));
$library->import($import);
