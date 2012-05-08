<?php

require_once 'NuraniModel.php';

/**
 * NuraniDrupal
 */
class NuraniDrupal extends NuraniModel {


  function __construct($database) {
    parent::__construct($database);
  }


  function search($corpus, $book, $chapter, $verse) {
    // Implement me.
  }


  function import($corpus, $document) {
    if (!$document->books) {
      return FALSE;
    }

    $corpus_id = $this->getCorpusID($corpus, TRUE);
    if ($corpus_id === FALSE) {
      return FALSE; // TODO: Log error for broken corpus ID.
    }

    foreach ($document->books as $bookKey => $book) {
      $book_id = $this->getBookID($bookKey, TRUE);
      if ($book_id === FALSE) {
        continue; // TODO: Log error for broken book ID.
      }

      foreach ($document->chapters as $chapterKey => $chapter) {
        $chapter_id = $this->getChapterID($chapterKey, TRUE);
        if ($chapter_id === FALSE) {
          continue; // TODO: Log error for broken chapter ID.
        }

        foreach ($chapter->verses as $verseKey => $verse) {
          // TODO: Batch these queries together for efficiency
          db_delete('nurani_library')
            ->condition('corpus_id',  $corpus_id)
            ->condition('book_id',    $book_id)
            ->condition('chapter_id', $chapter_id)
            ->condition('verse',      $verseKey)
            ->execute();

          db_insert('nurani_library')
            ->fields(array(
                'corpus_id'  => $corpus_id,
                'book_id'    => $book_id,
                'chapter_id' => $chapter_id,
                'verse'      => $verseKey,
                'value'      => $verse->text,
              ))
            ->execute();
        }
      }
    }
  }

  function getCorpusID($corpusName, $createIfMissing = FALSE) {
    $corpus_id = db_query("SELECT id FROM {nurani_library_corpora} WHERE name = '%s'", $corpusName)->fetchField();

    if (!$corpus_id && $createIfMissing) {
      $corpus_id = db_insert('nurani_library_corpora')
                     ->fields(array(
                         'name' => $corpusName
                       ))
                     ->execute();
    }

    return $corpus_id;
  }


  function getBookID($bookName, $createIfMissing = FALSE) {
    $book_id = db_query("SELECT id FROM {nurani_library_books} WHERE name = '%s'", $bookName)->fetchField();

    if (!$corpus_id && $createIfMissing) {
      $book_id = db_insert('nurani_library_books')
                   ->fields(array(
                       'name' => $bookName
                     ))
                   ->execute();
    }

    return $book_id;
  }


  function getChapterID($chapterName, $createIfMissing = FALSE) {
    $chapter_id = db_query("SELECT id FROM {nurani_library_chapters} WHERE name = '%s'", $chapterName)->fetchField();

    if (!$corpus_id && $createIfMissing) {
      $chapter_id = db_insert('nurani_library_chapters')
                      ->fields(array(
                          'name' => $chapterName
                        ))
                      ->execute();
    }

    return $chapter_id;
  }


}
