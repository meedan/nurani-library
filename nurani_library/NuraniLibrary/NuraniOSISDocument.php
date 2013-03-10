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
    foreach ($this->xml->xpath('//verse[@osisID]') as $verseXML) {
      $osisID = $this->parseOsisID((string) $verseXML['osisID']);

      $verse = $this->createVerse($verseXML);

      $this->contents[$osisID->book][$osisID->chapter][$osisID->verse] = $verse;
    }
  }


  /**
   * Generates an OSIS ID, useful when searching through OSIS XML via XPath.
   */
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


  /**
   * Splits an OSIS ID into components.
   */
  public function parseOsisID($osisID) {
    list($book, $chapter, $verse) = explode('.', $osisID);

    return (object) array(
      'book'    => $book,
      'chapter' => isset($chapter) && is_numeric($chapter) ? $chapter : NULL,
      'verse'   => isset($verse) && is_numeric($verse) ? $verse : NULL,
    );
  }


  /**
   * Turns an OSIS <verse></verse> SimpleXML object into a PHP object.
   *
   * NOTE: Currently only the verse text is extracted.
   */
  public function createVerse($verseXML) {
    $verse = (object) array(
      'text' => '',
      // Room for future expansion, interpretation
    );

    // Remove all unparsables
    $verse->text = strip_tags($verseXML->asXML(), '<note>');
    // Clean up white space issues
    $verse->text = trim(preg_replace('/[\pZ\pC]+/mu', ' ', $verse->text));
    // Removes notes from the text and stores them in a separate array
    $matches = array();
    $notes = preg_match_all('@<note([^>]*)>([^<]*)</note>@U', $verse->text, $matches, PREG_OFFSET_CAPTURE);

    $newText = '';
    $splitIndex = 0;

    foreach ($matches[0] as $i => $match) {
      $newText .= substr($verse->text, $splitIndex, $match[1] - $splitIndex);
      $splitIndex = $match[1] + strlen($match[0]);

      $words = NuraniLibrary::passageTextWords($newText);

      $verse->notes[] = (object) array(
        'position' => count($words),
        'length' => 0,
        'value' => $matches[2][$i][0],
      );
    }

    $newText .= substr($verse->text, $splitIndex, strlen($verse->text) - $splitIndex);
    $verse->text = $newText;

    if ($this->conf['stripChars']) {
      $verse->text = str_replace($this->conf['stripChars'], '', $verse->text);
    }

    return $verse;
  }


  public function bookOrder($book) {
    $order = array_search($book, $this->bibleBooksKeys);
    if ($order === FALSE) {
      return 0;
    }
    else {
      return $order + 1;
    }
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

    // Apocrypha
    if (isset($this->conf['includeApocrypha']) && $this->conf['includeApocrypha']) {
      $this->bibleBooks = array_merge($this->bibleBooks, array(
        'Tob'     => 'Tobit',                      'Jdt'     => 'Judith',
        'AddEsth' => 'Additions to Esther',        'Wis'     => 'Wisdom of Solomon',
        'Sir'     => 'Sirach',                     'Bar'     => 'Baruch',
        'EpJer'   => 'Letter of Jeremiah',         'PrAzar'  => 'Prayer of Azariah',
        'PrAzar'  => 'Prayer of Azariah',          'Sus'     => 'Susanna',
        'Bel'     => 'Bel and the Dragon',         '1Macc'   => '1 Maccabees',
        '2Macc'   => '2 Maccabees',                '1Esd'    => '1 Esdras',
        'PrMan'   => 'Prayer of Manasses',         '3Macc'   => '3 Maccabees',
        '2Esd'    => '2 Esdras',                   '4Macc'   => '4 Maccabees',
      ));
    }

    $this->bibleBooksKeys = array_keys($this->bibleBooks);
  }

}