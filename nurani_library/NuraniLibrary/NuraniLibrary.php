<?php

/**
 * NuraniLibrary
 * 
 * 
 */
class NuraniLibrary {

  public $model;


  public function __construct($config = array()) {
    $model = 'Nurani' . $config['backend'] . 'Model';

    require_once $model . '.php';

    $connection = isset($config['connection']) ? $config['connection'] : NULL;
    $this->model = new $model($connection);
  }


  public function getWorks() {
    return $this->model->getWorks();
  }


  public function getWork($work_name) {
    return $this->model->getWork($work_name);
  }


  public function isDuplicateWork($test_work) {
    $works = $this->getWorks();
    foreach ($works as $work) {
      if ($work->name == $test_work->name && $work->id == $test_work->id) {
        return TRUE;
      }
    }
    return FALSE;
  }


  public function search($work_name, $book = NULL, $chapter = NULL, $verse = NULL, $page = 0, $pagesize = 100) {
    return $this->model->search($work_name, $book, $chapter, $verse, $page, $pagesize);
  }


  public function getNotes($passage_id, $page = 0, $pagesize = 100) {
    return $this->model->getNotes($passage_id, $page, $pagesize);
  }


  public function import($import) {
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


  /**
   * NuraniLibrary::passageWords()
   *
   * UTF8 safe word splitter.
   */
  static function passageTextWords($text) {
    return preg_split('/[\pZ\pC]+/u', $text);
  }

}
