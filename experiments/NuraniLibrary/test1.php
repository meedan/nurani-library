<?php

require_once 'NuraniLibrary.php';

$library = new NuraniLibrary(array('backend' => 'Drupal'));
$search = $library->search('WLC', 'Gen', '1');

var_export($search);