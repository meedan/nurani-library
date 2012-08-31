<?php

require_once 'NuraniModel.php';


/**
 * NuraniDrupalModel
 */
class NuraniDrupalModel extends NuraniModel {


  public function __construct($connection) {
    parent::__construct($connection);
  }


  public function search($work_name, $book, $chapter = NULL, $verse = NULL, $page = 0, $limit = 250) {
    $select = db_select('nurani_library', 'l');

    // TODO: Simplify the NuraniDrupalModel::search() "SQL".  Using DBTNG makes a mess of it.
    $select->join('nurani_library_works',  'w', 'l.work_id = w.id');
    $select->join('nurani_library_books',    'b',  'l.book_id = b.id');
    $select->join('nurani_library_chapters', 'ch', 'l.chapter_id = ch.id');
    $select->fields('l',  array('id',   'verse',    'text'));
    $select->addField('w', 'name', 'work_name');
    $select->addField('w', 'full_name', 'work_full_name');
    $select->addField('w', 'language', 'work_language');
    $select->addField('b',  'name', 'book_name');
    $select->addField('b',  'full_name', 'book_full_name');
    $select->addField('ch', 'name', 'chapter_name');
    $select->addField('ch', 'full_name', 'chapter_full_name');
    $select->condition('w.name', $work_name);
    $select->condition('b.name', $book);
    $select->orderBy('w.name');
    $select->orderBy('w.language');
    $select->orderBy('b.name');
    $select->orderBy('ch.name');
    $select->orderBy('w.name');

    if (!is_null($chapter)) {
      $select->condition('ch.name', $chapter);
    }
    if (!is_null($verse)) {
      if (strstr($verse, '-') === FALSE) {
        $select->condition('l.verse', $verse);
      }
      // Handle verse range (eg: John.3.15-17)
      else {
        $parts = explode('-', $verse);
        $select->condition('l.verse', $parts[0], '>=');
        $select->condition('l.verse', $parts[1], '<=');
      }
    }

    if ($limit > 0) {
      $select->range($page * $limit, $limit);
    }

    $result = $select->execute();

    $search = count($result) ? array() : FALSE;
    foreach ($result as $row) {
      $search[] = $row;
    }

    return $search;
  }


  public function getWorks() {
    $result = db_query("SELECT w.*, COUNT(l.id) AS num_passages
                          FROM {nurani_library_works} w
                    INNER JOIN {nurani_library} l ON w.id = l.work_id
                      GROUP BY w.id
                      ORDER BY w.name, w.language");
    $works = array();
    foreach ($result as $work) {
      $works[] = $work;
    }
    return $works;
  }


  public function getWork($work_name) {
    return db_query("SELECT w.*, COUNT(l.id) AS num_passages
                          FROM {nurani_library_works} w
                    INNER JOIN {nurani_library} l ON w.id = l.work_id
                         WHERE w.name = :name
                      GROUP BY w.id
                      ORDER BY w.name, w.language", array(':name' => $work_name))->fetchObject();
  }


  public function deleteWork($work_id) {
    db_delete('nurani_library_works')
      ->condition('id', $work_id)
      ->execute();
    db_delete('nurani_library')
      ->condition('work_id', $work_id)
      ->execute();
    db_delete('nurani_library_books')
      ->condition('work_id', $work_id)
      ->execute();
    db_delete('nurani_library_chapters')
      ->condition('work_id', $work_id)
      ->execute();
  }


  public function import($work, $document) {
    if (!$document->contents) {
      return FALSE;
    }

    $work_id = $this->getWorkID($work, array('language' => $document->conf['language']));
    if ($work_id === FALSE) {
      return FALSE; // TODO: Log error for broken work ID.
    }

    foreach ($document->contents as $bookKey => $book) {
      $book_id = $this->getBookID($bookKey,
                                  array('work_id' => $work_id,
                                        'weight'    => $document->bookOrder($bookKey),
                                        'full_name' => $document->bookFullName($bookKey)));

      if ($book_id === FALSE) {
        continue; // TODO: Log error for broken book ID.
      }

      foreach ($book as $chapterKey => $chapter) {
        $chapter_id = $this->getChapterID($chapterKey,
                                          array('work_id' => $work_id,
                                                'weight'    => $chapterKey,
                                                'full_name' => $document->chapterFullName($chapterKey)));
        if ($chapter_id === FALSE) {
          continue; // TODO: Log error for broken chapter ID.
        }

        foreach ($chapter as $verseKey => $verse) {
          $record = db_query("SELECT *
                                FROM {nurani_library}
                               WHERE work_id  = :work_id
                                 AND book_id    = :book_id
                                 AND chapter_id = :chapter_id
                                 AND verse      = :verse",
                             array(
                               ":work_id"  => $work_id,
                               ":book_id"    => $book_id,
                               ":chapter_id" => $chapter_id,
                               ":verse"      => $verseKey
                             ))
                             ->fetchObject();

          if (!$record) {
            $record = (object) array();
          }

          $record->work_id  = $work_id;
          $record->book_id    = $book_id;
          $record->chapter_id = $chapter_id;
          $record->verse      = $verseKey;
          $record->text       = $verse->text;

          drupal_write_record('nurani_library', $record, isset($record->id) ? array('id') : array());
        }
      }
    }
  }


  public function getWorkID($name, $createIfMissing = FALSE) {
    if ($createIfMissing !== FALSE) {
      $fields = array_merge(array('name' => $name), $createIfMissing);
    }
    return $this->getOrCreateObject('nurani_library_works', $fields, array('name'));
  }


  public function getBookID($name, $createIfMissing = FALSE) {
    if ($createIfMissing !== FALSE) {
      $fields = array_merge(array('name' => $name), $createIfMissing);
    }
    return $this->getOrCreateObject('nurani_library_books', $fields, array('work_id', 'name'));
  }


  public function getChapterID($name, $createIfMissing = FALSE) {
    if ($createIfMissing !== FALSE) {
      $fields = array_merge(array('name' => $name), $createIfMissing);
    }
    return $this->getOrCreateObject('nurani_library_chapters', $fields, array('work_id', 'name'));
  }


  public function getOrCreateObject($table, $fields, $pk_fields) {
    // FIXME: this is not taking work_id into account for books and chapters
    //        and hence fails for quaranic stuff.
    $select = db_select($table, 't');
    $select->addField('t', 'id');
    foreach ($pk_fields as $field) {
      $select->condition('t.' . $field, $fields[$field]);
    }
    $id = $select->execute()->fetchField();

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
