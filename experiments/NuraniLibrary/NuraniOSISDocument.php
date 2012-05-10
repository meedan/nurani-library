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
  public $bibleBooksKeys;


  public function __construct($path, $file, $conf) {
    parent::__construct($path, $file, $conf);
    $this->populateBooks();
    $this->load();
  }


  public function load() {
    parent::load();
    $this->xml = new SimpleXMLElement(str_replace('xmlns=', 'ns=', $this->rawContents));

    // Build the chapter and verse list
    foreach ($this->xml->xpath('/osis/osisText/div[@type="book"]/chapter/verse') as $verseXML) {
      $osisID = $this->parseOsisID((string) $verseXML['osisID']);

      $verse = $this->createVerse($verseXML);

      $this->contents[$osisID->book][$osisID->chapter][$osisID->verse] = $verse;
    }
  }


  public function osisID($book, $chapter = NULL, $verse = NULL) {
    if (array_key_exists($book, $this->contents)) {
      $osisBook = $book;
    }
    else {
      $osisBook = array_search($book, $this->contents);
    }

    if (!$osisBook) {
      return FALSE;
    }

    // Simple way to make a valid number
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


  public function bookOrder($book) {
    if (!array_key_exists($book, $this->bibleBooksKeys)) {
      return 0;
    }
    return array_search($book, $this->bibleBooksKeys) + 1;
  }


  public function bookFullName($book) {
    return array_key_exists($book, $this->bibleBooks) ? $this->bibleBooks[$book] : $book;
  }


  public function chapterFullName($chapter) {
    return $chapter;
  }


  protected function populateBooks() {
    $this->bibleBooks = array(
      // Old Testament
      'Gen'    => 'Genesis',         'Exod'   => 'Exodus',          'Lev'    => 'Leviticus',
      'Num'    => 'Numbers',         'Deut'   => 'Deuteronomy',     'Josh'   => 'Joshua',
      'Judg'   => 'Judges',          'Ruth'   => 'Ruth',            '1Sam'   => '1 Samuel',
      '2Sam'   => '2 Samuel',        '1Kgs'   => '1 Kings',         '2Kgs'   => '2 Kings',
      '1Chr'   => '1 Chronicles',    '2Chr'   => '2 Chronicles',    'Ezra'   => 'Ezra',
      'Neh'    => 'Nehemiah',        'Esth'   => 'Esther',          'Job'    => 'Job',
      'Ps'     => 'Psalms',          'Prov'   => 'Proverbs',        'Eccl'   => 'Ecclesiastes',
      'Song'   => 'Song of Solomon', 'Isa'    => 'Isaiah',          'Jer'    => 'Jeremiah',
      'Lam'    => 'Lamentations',    'Ezek'   => 'Ezekiel',         'Dan'    => 'Daniel',
      'Hos'    => 'Hosea',           'Joel'   => 'Joel',            'Amos'   => 'Amos',
      'Obad'   => 'Obadiah',         'Jonah'  => 'Jonah',           'Mic'    => 'Micah',
      'Nah'    => 'Nahum',           'Hab'    => 'Habakkuk',        'Zeph'   => 'Zephaniah',
      'Hag'    => 'Haggai',          'Zech'   => 'Zechariah',       'Mal'    => 'Malachi',

      // New Testament
      'Matt'   => 'Matthew',         'Mark'   => 'Mark',            'Luke'   => 'Luke',
      'John'   => 'John',            'Acts'   => 'Acts',            'Rom'    => 'Romans',
      '1Cor'   => '1 Corinthians',   '2Cor'   => '2 Corinthians',   'Gal'    => 'Galatians',
      'Eph'    => 'Ephesians',       'Phil'   => 'Philippians',     'Col'    => 'Colossians',
      '1Thess' => '1 Thessalonians', '2Thess' => '2 Thessalonians', '1Tim'   => '1 Timothy',
      '2Tim'   => '2 Timothy',       'Titus'  => 'Titus',           'Phlm'   => 'Philemon',
      'Heb'    => 'Hebrews',         'Jas'    => 'James',           '1Pet'   => '1 Peter',
      '2Pet'   => '2 Peter',         '1John'  => '1 John',          '2John'  => '2 John',
      '3John'  => '3 John',          'Jude'   => 'Jude',            'Rev'    => 'Revelation',
    );

    // TODO: Verify if the list of Apocrypha is correct
    if ($this->conf['includeApocrypha']) {
      $this->bibleBooks = array_merge($this->bibleBooks, array(
        'Tob'     => 'Tobit',                      'Jdt'     => 'Judith',
        'GkEsth'  => 'Greek Esther',               'Wis'     => 'Wisdom of Solomon',
        'Sir'     => 'Sirach',                     'Bar'     => 'Baruch',
        'PrAzar'  => 'The Prayer of Azarias',      'Sus'     => 'Susanna',
        'Bel'     => 'Bel and the Dragon',         'SgThree' => 'Song of the Three Children',
        'EpJer'   => 'Epistle of Jeremy',          '1Macc'   => '1 Maccabees',
        '2Macc'   => '2 Maccabees',                '3Macc'   => '3 Maccabees',
        '4Macc'   => '4 Maccabees',                '1Esd'    => '1 Esdras',
        '2Esd'    => '2 Esdras',                   'PrMan'   => 'Prayer of Manasses',
      ));
    }

    $this->bibleBooksKeys = array_keys($this->bibleBooks);
  }

}