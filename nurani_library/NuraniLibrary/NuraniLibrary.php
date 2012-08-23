<?php

/**
 * NuraniLibrary
 * 
 * 
 */
class NuraniLibrary {

  public $model;


  function __construct($config = array()) {
    $model = 'Nurani' . $config['backend'] . 'Model';

    require_once $model . '.php';

    $database = isset($config['database']) ? $config['database'] : NULL;
    $this->model = new $model($database);
  }


  function search($work, $book, $chapter = NULL, $verse = NULL, $language = NULL, $offset = 0, $limit = 250) {
    return $this->model->search($work, $book, $chapter, $verse, $language, $offset, $limit);
  }


  function import($import) {
    foreach ($import as $work => $info) {
      if (!isset($info['documentType']) || !$info['documentType']) {
        continue;
      }

      $class = 'Nurani' . $info['documentType'] . 'Document';
      require_once $class . '.php';

      foreach ($info['files'] as $file) {
        $document = new $class($info['path'], $file, $info);

        $this->model->import($work, $document);

        unset($document); // Ensure the memory is freed, these can be big
      }
    }
  }


}
