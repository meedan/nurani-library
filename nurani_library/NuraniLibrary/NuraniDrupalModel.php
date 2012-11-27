<?php

require_once 'NuraniModel.php';


/**
 * NuraniDrupalModel
 */
class NuraniDrupalModel extends NuraniModel {

  private $connected;

  public function __construct($connection) {
    parent::__construct($connection);

    $this->connected = (
         db_table_exists('nurani_library')
      && db_table_exists('nurani_library_works')
      && db_table_exists('nurani_library_books')
      && db_table_exists('nurani_library_chapters')
    );
  }


  public function search($work_name, $book = NULL, $chapter = NULL, $verse = NULL, $page = 0, $pagesize = 100) {
    if (!$this->connected) {
      return $this->error(t("Could not establish connection to {nurani_library} database tables."), 0);
    }

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
    $select->orderBy('w.name');
    $select->orderBy('w.language');
    $select->orderBy('b.name');
    $select->orderBy('ch.name');
    $select->orderBy('w.name');

    if ($book) {
      $select->condition('b.name', $book);
    }
    if ($chapter) {
      $select->condition('ch.name', $chapter);
    }
    if ($verse) {
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

    if ($pagesize > 0) {
      $select->range($page * $pagesize, $pagesize);
    }

    $result = $select->execute();

    $search = count($result) ? array() : FALSE;
    foreach ($result as $row) {
      $search[] = $row;
    }

    return $search;
  }


  public function getWorks() {
    if (!$this->connected) {
      return $this->error(t("Could not establish connection to {nurani_library} database tables."), 0);
    }

    $result = db_query("SELECT name FROM {nurani_library_works} ORDER BY id");
    $works = array();

    foreach ($result as $work) {
      $works[] = $this->getWork($work->name);
    }

    return $works;
  }


  public function getWork($work_name) {
    if (!$this->connected) {
      return $this->error(t("Could not establish connection to {nurani_library} database tables."), 0);
    }

    $result = db_query("SELECT w.name, w.full_name, w.language,
                               b.name AS book_name, b.full_name AS book_full_name,
                               c.name AS chapter_name, c.full_name AS chapter_full_name
                          FROM {nurani_library} nl
                    LEFT JOIN {nurani_library_chapters} c ON nl.chapter_id = c.id
                    LEFT JOIN {nurani_library_books} b ON nl.book_id = b.id
                    LEFT JOIN {nurani_library_works} w ON nl.work_id = w.id
                         WHERE w.name = :work_name
                      GROUP BY nl.work_id, nl.book_id, nl.chapter_id
                      ORDER BY nl.work_id, b.weight, c.weight", array(':work_name' => $work_name));

    $maps = array('books' => array(), 'chapters' => array());

    foreach ($result as $row) {
      $this->autoCastTypes($row);

      $book_key    = $this->keyForValueInUniqueMap($row->book_name, $maps['books']);
      $chapter_key = $this->keyForValueInUniqueMap($row->chapter_name, $maps['chapters']);

      // $work is created on processing the first record
      if (!isset($work)) {
        $work = (object) array(
          'name'      => $row->name,
          'full_name' => $row->full_name,
          'language'  => $row->language,
          'books'     => array(),
        );
      }

      if (!isset($work->books[$book_key])) {
        $work->books[$book_key] = (object) array(
          'name'      => $row->book_name,
          'full_name' => $row->book_full_name,
          'chapters'  => array(),
        );
      }

      if ($row->chapter_name == $row->chapter_full_name) {
        $work->books[$book_key]->chapters[$chapter_key] = $row->chapter_name;
      }
      else {
        $work->books[$book_key]->chapters[$chapter_key] = array(
          $row->chapter_name,
          $row->chapter_full_name,
        );
      }
    }

    // Optimization, when possible flatten chapters into a range representation
    foreach ($work->books as $book_key => $book) {
      $found_non_numeric = FALSE;

      foreach ($book->chapters as $chapter) {
        if (!is_numeric($chapter)) {
          $found_non_numeric = TRUE;
          break;
        }
      }

      if (!$found_non_numeric) {
        $first = reset($book->chapters);
        $last  = end($book->chapters);
        $work->books[$book_key]->chapters = "{$first}-{$last}";
      }
    }

    return $work;
  }


  public function deleteWork($work_id) {
    if (!$this->connected) {
      return $this->error(t("Could not establish connection to {nurani_library} database tables."), 0);
    }

    db_delete('nurani_library_works')
      ->condition('id', $work_id)
      ->execute();
    db_delete('nurani_library')
      ->condition('work_id', $work_id)
      ->execute();

    // Cleaning up orphaned books and chapters
    db_query("DELETE FROM {nurani_library_books}
                    WHERE NOT EXISTS (SELECT *
                                        FROM {nurani_library} nl
                                       WHERE nl.book_id = {nurani_library_books}.id)");
    db_query("DELETE FROM {nurani_library_chapters}
                    WHERE NOT EXISTS (SELECT *
                                        FROM {nurani_library} nl
                                       WHERE nl.chapter_id = {nurani_library_chapters}.id)");
  }


  public function import($work, $document) {
    if (!$this->connected) {
      return $this->error(t("Could not establish connection to {nurani_library} database tables."), 0);
    }

    if (!$document->contents) {
      return FALSE;
    }

    $work_id = $this->getWorkID($work, array('language' => $document->conf['language']));
    if ($work_id === FALSE) {
      return FALSE; // TODO: Log error for broken work ID.
    }

    foreach ($document->contents as $bookKey => $book) {
      $book_id = $this->getBookID($bookKey,
                                  array('weight'    => $document->bookOrder($bookKey),
                                        'full_name' => $document->bookFullName($bookKey)));

      if ($book_id === FALSE) {
        continue; // TODO: Log error for broken book ID.
      }

      foreach ($book as $chapterKey => $chapter) {
        $chapter_id = $this->getChapterID($chapterKey,
                                          array('weight'    => $chapterKey,
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

          $record->work_id    = $work_id;
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
    return $this->getOrCreateObject('nurani_library_books', $fields, array('name'));
  }


  public function getChapterID($name, $createIfMissing = FALSE) {
    if ($createIfMissing !== FALSE) {
      $fields = array_merge(array('name' => $name), $createIfMissing);
    }
    return $this->getOrCreateObject('nurani_library_chapters', $fields, array('name'));
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

  private function autoCastTypes(&$object) {
    foreach ($object as &$value) {
      if (is_numeric($value)) {
        $value = strpos($value, '.') === FALSE
               ? (int) $value
               : (float) $value;
      }
    }
  }

  /**
   * 
   */
  private function keyForValueInUniqueMap($value, &$map) {
    $key = array_search($value, $map);
    if ($key === FALSE) {
      $map[] = $value;
      $key = count($map) - 1;
    }
    return (int) $key;
  }

}
