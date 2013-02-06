<?php

/**
 * NuraniModel
 */
abstract class NuraniModel {

  public $connection;
  public $errorMessage;
  public $errorCode;
  public $errorData;


  public function __construct($connection = array()) {
    $this->connection = $connection;
    $this->resetErrorState();
  }


  public function resetErrorState() {
    $this->errorMessage = NULL;
    $this->errorCode    = NULL;
    $this->errorData    = NULL;
  }

  public function getError() {
    if (!is_null($this->errorMessage) || !is_null($this->errorCode)) {
      return array('message' => $this->errorMessage, 'code' => (int) $this->errorCode);
    }
    else {
      return NULL;
    }
  }

  public function error($message, $code, $data = NULL) {
    $this->errorMessage = $message;
    $this->errorCode    = $code;
    $this->errorData    = $data;
    return FALSE;
  }


  abstract public function search($work_name, $book = NULL, $chapter = NULL, $verse = NULL, $authorUUID = NULL, $page = 0, $pagesize = 100);
  abstract public function getAnnotations($passage_id, $authorUUID = NULL, $type = NULL, $page = 0, $pagesize = 100);
  abstract public function getAnnotation($id);
  abstract public function createAnnotation($annotation);
  abstract public function updateAnnotation($id, $annotation);
  abstract public function deleteAnnotation($id);
  abstract public function getWorks();
  abstract public function getWork($work_name);
  abstract public function deleteWork($work_id);
  abstract public function import($work, $document);

}
