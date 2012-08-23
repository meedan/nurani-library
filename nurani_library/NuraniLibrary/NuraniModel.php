<?php

/**
 * NuraniModel
 */
abstract class NuraniModel {


  public function __construct() {

  }


  abstract public function search($work, $book, $chapter = NULL, $verse = NULL,
                                  $language = NULL, $offset = 0, $limit = 250);

  abstract public function import($work, $document);

  abstract public function deleteWork($work_id);


}
