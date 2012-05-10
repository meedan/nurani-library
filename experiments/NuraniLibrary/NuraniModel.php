<?php

/**
 * NuraniModel
 */
abstract class NuraniModel {


  public function __construct() {

  }


  abstract public function search($corpus, $book, $chapter = NULL, $verse = NULL,
                                  $language = NULL, $offset = 0, $limit = 250);

  abstract public function import($corpus, $document);


}
