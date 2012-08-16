<?php

function nurani_library_texts_path() {
  return variable_get('nurani_library_texts_path', 'sites/all/texts');
}

function nurani_library_find_texts($path) {
  $texts = glob_recursive($path . '/*.xml');
  dpm($texts);
}

/**
 * Recursive glob function.
 * @see http://us.php.net/manual/en/function.glob.php#106595
 */
function glob_recursive($pattern, $flags = 0) {
  $files = glob($pattern, $flags);

  foreach (glob(dirname($pattern).'/*', GLOB_ONLYDIR|GLOB_NOSORT) as $dir) {
    $files = array_merge($files, glob_recursive($dir.'/'.basename($pattern), $flags));
  }

  return $files;
}

// TODO: Un-stub nurani_library_texts().
function nurani_library_texts() {
  return array(
    (object) array('machine_name' => 'niv', 'name' => t("NIV"), 'type' => 'biblical'),
    (object) array('machine_name' => 'nkjv', 'name' => t("NKJV"), 'type' => 'biblical'),
    (object) array('machine_name' => 'wlc', 'name' => t("WLC"), 'type' => 'hebraic'),
  );
}

function nurani_library_text_options() {
  $options = array();
  foreach (nurani_library_texts() as $type) {
    $options[$type->machine_name] = $type->name;
  }
  return $options;
}

// TODO: Un-stub nurani_library_text_types().
function nurani_library_text_types() {
  return array(
    (object) array(
      'machine_name' => 'biblical',
      'name' => t("Biblical")
    ),
    (object) array(
      'machine_name' => 'biblical_apocryhpa',
      'name' => t("Biblical (including Apocrypha)"),
      'options' => array('volumes' => array('new_testament', 'old_testament', 'apocrypha')),
    ),
    (object) array(
      'machine_name' => 'hebraic',
      'name' => t("Hebraic (Hebrew bible)")
    ),
    (object) array(
      'machine_name' => 'quranic',
      'name' => t("Quranic")
    ),
  );
}

function nurani_library_text_type_options() {
  $options = array();
  foreach (nurani_library_text_types() as $type) {
    $options[$type->machine_name] = $type->name;
  }
  return $options;
}

/**
 * Returns a list of books for a given text type.
 * 
 * @param (string) $type
 *  One of 'biblical', 'hebraic' or 'quranic'
 * @param (array) $options
 *  A list of options, see $defaults.
 */
function nurani_library_books($type, $options = array()) {
  // Default options per-type
  $defaults = array(
    'biblical' => array(
      'volumes' => array('new_testament', 'old_testament'),
    ),
    'hebraic' => array(
      'volumes' => array('old_testament'),
    ),
    'quranic' => array(
      'volumes' => array('quran')
    ),
  );

  $options = array_merge($defaults[$type], (array) $options);

  $books = array();
  foreach ($options['volumes'] as $volume) {
    $function = "_nurani_library_books_" . $type . "_" . $volume;

    if (function_exists($function)) {
      $books = array_merge($books, call_user_func($function));
    }
  }

  return $books;
}

function _nurani_library_books_biblical_old_testament() {
  return array(
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
  );
}

function _nurani_library_books_biblical_new_testament() {
  return array(
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
}

function _nurani_library_books_hebraic_old_testament() {
  return _nurani_library_books_biblical_old_testament();
}

function _nurani_library_books_biblical_apocrypha() {
  return array(
    'Tob'     => 'Tobit',                 'Jdt'     => 'Judith',
    'GkEsth'  => 'Greek Esther',          'Wis'     => 'Wisdom of Solomon',
    'Sir'     => 'Sirach',                'Bar'     => 'Baruch',
    'PrAzar'  => 'The Prayer of Azarias', 'Sus'     => 'Susanna',
    'Bel'     => 'Bel and the Dragon',    'SgThree' => 'Song of the Three Children',
    'EpJer'   => 'Epistle of Jeremy',     '1Macc'   => '1 Maccabees',
    '2Macc'   => '2 Maccabees',           '3Macc'   => '3 Maccabees',
    '4Macc'   => '4 Maccabees',           '1Esd'    => '1 Esdras',
    '2Esd'    => '2 Esdras',              'PrMan'   => 'Prayer of Manasses',
  );
}

function _nurani_library_books_quranic_quran() {
  return array(
    '1' => 'الفاتحة',    '2' => 'البقرة',     '3' => 'آل عمران',
    '4' => 'النساء',     '5' => 'المائدة',    '6' => 'الأنعام',
    '7' => 'الأعراف',    '8' => 'الأنفال',    '9' => 'التوبة',
    '10' => 'يونس',      '11' => 'هود',       '12' => 'يوسف',
    '13' => 'الرعد',     '14' => 'ابراهيم',   '15' => 'الحجر',
    '16' => 'النحل',     '17' => 'الإسراء',   '18' => 'الكهف',
    '19' => 'مريم',      '20' => 'طه',        '21' => 'الأنبياء',
    '22' => 'الحج',      '23' => 'المؤمنون',  '24' => 'النور',
    '25' => 'الفرقان',   '26' => 'الشعراء',   '27' => 'النمل',
    '28' => 'القصص',     '29' => 'العنكبوت',  '30' => 'الروم',
    '31' => 'لقمان',     '32' => 'السجدة',    '33' => 'الأحزاب',
    '34' => 'سبإ',       '35' => 'فاطر',      '36' => 'يس',
    '37' => 'الصافات',   '38' => 'ص',         '39' => 'الزمر',
    '40' => 'غافر',      '41' => 'فصلت',      '42' => 'الشورى',
    '43' => 'الزخرف',    '44' => 'الدخان',    '45' => 'الجاثية',
    '46' => 'الأحقاف',   '47' => 'محمد',      '48' => 'الفتح',
    '49' => 'الحجرات',   '50' => 'ق',         '51' => 'الذاريات',
    '52' => 'الطور',     '53' => 'النجم',     '54' => 'القمر',
    '55' => 'الرحمن',    '56' => 'الواقعة',   '57' => 'الحديد',
    '58' => 'المجادلة',  '59' => 'الحشر',     '60' => 'الممتحنة',
    '61' => 'الصف',      '62' => 'الجمعة',    '63' => 'المنافقون',
    '64' => 'التغابن',   '65' => 'الطلاق',    '66' => 'التحريم',
    '67' => 'الملك',     '68' => 'القلم',     '69' => 'الحاقة',
    '70' => 'المعارج',   '71' => 'نوح',       '72' => 'الجن',
    '73' => 'المزمل',    '74' => 'المدثر',    '75' => 'القيامة',
    '76' => 'الانسان',   '77' => 'المرسلات',  '78' => 'النبإ',
    '79' => 'النازعات',  '80' => 'عبس',       '81' => 'التكوير',
    '82' => 'الإنفطار',  '83' => 'المطففين',  '84' => 'الإنشقاق',
    '85' => 'البروج',    '86' => 'الطارق',    '87' => 'الأعلى',
    '88' => 'الغاشية',   '89' => 'الفجر',     '90' => 'البلد',
    '91' => 'الشمس',     '92' => 'الليل',     '93' => 'الضحى',
    '94' => 'الشرح',     '95' => 'التين',     '96' => 'العلق',
    '97' => 'القدر',     '98' => 'البينة',    '99' => 'الزلزلة',
    '100' => 'العاديات', '101' => 'القارعة',  '102' => 'التكاثر',
    '103' => 'العصر',    '104' => 'الهمزة',   '105' => 'الفيل',
    '106' => 'قريش',     '107' => 'الماعون',  '108' => 'الكوثر',
    '109' => 'الكافرون', '110' => 'النصر',    '111' => 'المسد',
    '112' => 'الإخلاص',  '113' => 'الفلق',    '114' => 'الناس',
  );
}
