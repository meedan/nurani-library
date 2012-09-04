<?php

/**
 * NuraniModel
 */
abstract class NuraniModel {

  public $connection;

  public function __construct($connection = array()) {
    $this->connection = $connection;
  }


  abstract public function search($work_name, $book, $chapter = NULL, $verse = NULL, $offset = 0, $limit = 250);

  abstract public function getWorks();
  abstract public function getWork($work_name);
  abstract public function deleteWork($work_id);

  abstract public function import($work, $document);

}
