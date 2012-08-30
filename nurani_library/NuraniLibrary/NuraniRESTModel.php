<?php

require_once 'NuraniModel.php';


/**
 * NuraniRESTModel
 */
class NuraniRESTModel extends NuraniModel {

  public function __construct($connection) {
    parent::__construct($connection);
  }


  public function search($work, $book, $chapter = NULL, $verse = NULL, $language = NULL, $offset = 0, $limit = 250) {
    return array();
  }


  public function import($work, $document) {
    return FALSE;
  }


  public function getWorks() {
    return 1;
  }


  public function getWork($work_name) {
    return 1;
  }


  public function deleteWork($work_id) {
    return FALSE;
  }


  public function numPassagesForWork($work_name) {
    return 1;
  }

}
