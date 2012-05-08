<?php

require_once 'NuraniDocument.php';

/**
 * NuraniOSISDocument
 * 
 * 
 */
class NuraniOSISDocument extends NuraniDocument {

  public $xml;
  public $bibleBooks;


  public function __construct($path, $file, $conf) {
    parent::__construct($path, $file, $conf);
    $this->populateBooks();
    $this->load();
  }


  public function load() {
    parent::load();
    $this->xml = new SimpleXMLElement(str_replace('xmlns=', 'ns=', $this->contents));

    // Build the chapter and verse list
    foreach ($this->xml->xpath('/osis/osisText/div[@type="book"]/chapter/verse') as $verseXML) {
      $osisID = $this->parseOsisID((string) $verseXML['osisID']);

      $verse = $this->createVerse($verseXML);

      $this->books[$osisID->book][$osisID->chapter][$osisID->verse] = $verse;
    }
  }


  /**
   * Searches the currently loaded text.
   * 
   * @param $book
   *  Required, a book / volume partition of the text
   *  (eg: "Genesis", or in OSIS "Gen")
   * @param $chapter
   *  Required, the chapter in the book.
   * @param $verse
   *  Optional, the 
   */
  public function search($book, $chapter, $verse = NULL) {
    $chapterID = $this->osisID($book, $chapter);
    $this->xml->xpath('/osis/osisText/div/chapter[' . $chapterID . ']/verse');
  }


  public function osisID($book, $chapter = NULL, $verse = NULL) {
    if (array_key_exists($book, $this->books)) {
      $osisBook = $book;
    }
    else {
      $osisBook = array_search($book, $this->books);
    }

    if (!$osisBook) {
      return FALSE;
    }

    // Simple way to truncate leading zeros, etc.
    $chapter = (int) $chapter;
    $verse   = (int) $verse;

    $osisID = $osisBook;
    $osisID = $chapter ? '.' . $chapter : '';
    $osisID = $verse ? '.' . $verse : '';

    return $osisID;
  }


  public function parseOsisID($osisID) {
    list($book, $chapter, $verse) = explode('.', $osisID);

    return (object) array(
      'book'    => $book,
      'chapter' => isset($chapter) && is_numeric($chapter) ? $chapter : 1,
      'verse'   => isset($verse) && is_numeric($verse) ? $verse : 1,
    );
  }


  public function createVerse($verseXML) {
    $verse = (object) array(
      'text' => '',
      // Room for future expansion, interpretation
    );

    if (isset($verseXML->w) || isset($verseXML->seg)) {
      $words = array();
      foreach ($verseXML->xpath('w|seg') as $part) {
        $words[] = (string) $part;
      }
      if ($this->conf['textDirection'] == 'rtl') {
        $words = array_reverse($words);
      }
      $verse->text = implode(' ', $words);
    }
    else {
      $verse->text = (string) $verseXML;
    }

    if ($this->conf['stripMarkup']) {
      $verse->text = strip_tags($verse->text);
    }
    if ($this->conf['stripChars']) {
      $verse->text = str_replace($this->conf['stripChars'], '', $verse->text);
    }

    return $verse;
  }


  protected function populateBooks() {
    $this->bibleBooks = array(
      'Gen'   => 'Genesis',         'Exod'  => 'Exodus',          'Lev'   => 'Leviticus',       
      'Num'   => 'Numbers',         'Deut'  => 'Deuteronomy',     'Josh'  => 'Joshua',          
      'Judg'  => 'Judges',          'Ruth'  => 'Ruth',            '1Sam'  => '1 Samuel',        
      '2Sam'  => '2 Samuel',        '1Kgs'  => '1 Kings',         '2Kgs'  => '2 Kings',         
      '1Chr'  => '1 Chronicles',    '2Chr'  => '2 Chronicles',    'Ezra'  => 'Ezra',            
      'Neh'   => 'Nehemiah',        'Esth'  => 'Esther',          'Job'   => 'Job',             
      'Ps'    => 'Psalms',          'Prov'  => 'Proverbs',        'Eccl'  => 'Ecclesiastes',    
      'Song'  => 'Song of Solomon', 'Isa'   => 'Isaiah',          'Jer'   => 'Jeremiah',        
      'Lam'   => 'Lamentations',    'Ezek'  => 'Ezekiel',         'Dan'   => 'Daniel',          
      'Hos'   => 'Hosea',           'Joel'  => 'Joel',            'Amos'  => 'Amos',            
      'Obad'  => 'Obadiah',         'Jonah' => 'Jonah',           'Mic'   => 'Micah',           
      'Nah'   => 'Nahum',           'Hab'   => 'Habakkuk',        'Zeph'  => 'Zephaniah',       
      'Hag'   => 'Haggai',          'Zech'  => 'Zechariah',       'Mal'   => 'Malachi',         
    );
  }

}