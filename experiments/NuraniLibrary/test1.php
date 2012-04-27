<?php

require_once 'NuraniLibrary.php';

$library = new NuraniLibrary();
$library->loadText('WLC', 'Exodus');
var_export($library);
