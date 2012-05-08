<?php

require_once 'NuraniModel.php';

/**
 * NuraniDrupal
 */
class NuraniDrupal extends NuraniModel {

  public $corpus_table_prefix = 'nurani_library';

  function __construct($database) {
    parent::__construct($database);
  }


  function corpusExists($corpus) {
    static $corpora = array();

    if (!array_key_exists($corpus, $corpora)) {
      $corpora[$corpus] = db_table_exists($this->corpusTableName($corpus));
    }

    return $corpora[$corpus];
  }


  function corpusTableName($corpus) {
    return $this->corpus_table_prefix . '_' . $corpus;
  }


  function search($corpus, $book, $chapter, $verse) {

  }


  function import($corpus, $document) {
    if (!$document->chapters) {
      return FALSE;
    }

    foreach ($document->chapters as $chapterKey => $chapter) {
      foreach ($chapter->verses as $verseKey => $verse) {
        // DELETE + INSERT
      }
    }
  }


}
