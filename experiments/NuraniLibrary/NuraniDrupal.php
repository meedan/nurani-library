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

    $corpus_id = $this->getCorpusID($corpus,
                                    array('language' => $document->conf['language']));
    if ($corpus_id === FALSE) {
      return FALSE; // TODO: Log error for broken corpus ID.
    }

    foreach ($document->books as $bookKey => $book) {
      // TODO: Handle non-bible texts for book weight ordering
      $weight    = array_search($bookKey, $document->bibleBooksKeys);
      $full_name = $document->bibleBooks[$bookKey];
      $book_id   = $this->getBookID($bookKey,
                                    array('corpus_id' => $corpus_id,
                                          'weight'    => ($weight + 1),
                                          'full_name' => $full_name));

      if ($book_id === FALSE) {
        continue; // TODO: Log error for broken book ID.
      }

      foreach ($book as $chapterKey => $chapter) {
        $chapter_id = $this->getChapterID($chapterKey,
                                          array('corpus_id' => $corpus_id,
                                                'weight'    => $chapterKey));
        if ($chapter_id === FALSE) {
          continue; // TODO: Log error for broken chapter ID.
        }

        foreach ($chapter as $verseKey => $verse) {
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
                'text'       => $verse->text,
              ))
            ->execute();
        }
      }
    }
  }


  function getCorpusID($name, $createIfMissing = FALSE) {
    if ($createIfMissing !== FALSE) {
      $fields = array_merge(array('name' => $name), $createIfMissing);
    }
    return $this->getOrCreateObject('nurani_library_corpora', $fields);
  }


  function getBookID($name, $createIfMissing = FALSE) {
    if ($createIfMissing !== FALSE) {
      $fields = array_merge(array('name' => $name), $createIfMissing);
    }
    return $this->getOrCreateObject('nurani_library_books', $fields);
  }


  function getChapterID($name, $createIfMissing = FALSE) {
    if ($createIfMissing !== FALSE) {
      $fields = array_merge(array('name' => $name), $createIfMissing);
    }
    return $this->getOrCreateObject('nurani_library_chapters', $fields);
  }


  function getOrCreateObject($table, $fields) {
    $id = db_query("SELECT id FROM {" . $table . "} WHERE name = :name", array(':name' => $fields['name']))->fetchField();

    if (!$id && $fields !== FALSE) {
      if (!isset($fields['full_name'])) {
        $fields['full_name'] = $fields['name'];
      }

      $id = db_insert($table)
              ->fields($fields)
              ->execute();
    }

    return $id;
  }


}
