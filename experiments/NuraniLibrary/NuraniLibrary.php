<?php

/**
 * NuraniLibrary
 * 
 * 
 */
class NuraniLibrary {

  public $model;


  function __construct($config = array()) {
    // Adds $library into the namespace
    require 'NuraniConfig.php';

    if (is_array($config)) {
      extract($config);
    }

    // FIXME: Potential class name injection here.
    require_once $model . '.php';
    $this->model = new $model($database);
  }


  function search($corpus, $book, $chapter = NULL, $verse = NULL) {
    $chapter = is_numeric($chapter) ? $chapter : 1;
    $verse   = is_numeric($verse) ? $verse : 1;

    return $this->model->search($corpus, $book, $chapter, $verse);
  }


  function import($import) {
    foreach ($import as $corpus => $info) {
      $class = 'Nurani' . $info['documentType'] . 'Document';
      require_once $class . '.php';

      foreach ($info['chapters'] as $file) {
        $document = new $class($info['path'], $file, $info);

        $this->model->import($corpus, $document);

        unset($document); // Ensure the memory is freed, these can be big
      }
    }
  }


}
