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

    $connection = isset($config['connection']) ? $config['connection'] : NULL;
    $this->model = new $model($connection);
  }


  function getWorks() {
    return $this->model->getWorks();
  }


  function getWork($work_name) {
    return $this->model->getWork($work_name);
  }


  function getWorkStats($work_name) {
    return array(
      'num_passages' => $this->model->numPassagesForWork($work_name),
    );
  }


  function isDuplicateWork($test_work) {
    $works = $this->getWorks();
    foreach ($works as $work) {
      if ($work->name == $test_work->name && $work->id == $test_work->id) {
        return TRUE;
      }
    }
    return FALSE;
  }


  function search($work_name, $book, $chapter = NULL, $verse = NULL, $language = NULL, $offset = 0, $limit = 250) {
    return $this->model->search($work_name, $book, $chapter, $verse, $language, $offset, $limit);
  }


  function import($import) {
    foreach ($import as $work => $info) {
      if (!isset($info['format']) || !$info['format']) {
        continue;
      }

      $class = 'Nurani' . $info['format'] . 'Document';
      require_once $class . '.php';

      foreach ($info['files'] as $file) {
        $document = new $class($info['path'], $file, $info);

        $this->model->import($work, $document);

        unset($document); // Ensure the memory is freed, these can be big
      }
    }
  }


}
