<?php

/**
 * Class to manage the work types.  Most useful for validation.
 */
class NuraniWork {

  public $workTypes;
  public $workLanguages;


  function __construct() {
    $this->workTypes = array(
      'biblical' => (object) array(
        'name' => t("Biblical"),
        'books' => array_merge(
          $this->booksBiblicalOldTestament(),
          $this->booksBiblicalNewTestament()
        ),
      ),
      'biblical_apocryhpa' => (object) array(
        'name' => t("Biblical (including Apocrypha)"),
        'books' => array_merge(
          $this->booksBiblicalOldTestament(),
          $this->booksBiblicalNewTestament(),
          $this->booksBiblicalApocrypha()
        ),
      ),
      'hebraic' => (object) array(
        'name' => t("Hebraic (Hebrew bible)"),
        'books' => $this->booksBiblicalOldTestament(),
      ),
      'quranic' => (object) array(
        'name' => t("Quranic"),
        'books' => $this->booksQuranicQuran(),
      ),
    );
    $this->workTypeOptions = array();
    foreach ($this->workTypes as $type_name => $type) {
      $this->workTypeOptions[$type_name] = $type->name;
    }

    $this->workLanguages = array(
      'ar' => t("Arabic (ar)"),
      'en' => t("English (en)"),
      'fr' => t("French (fr)"),
      'he' => t("Hebrew (he)"),
    );
    // TODO: Need to add attributes for RTL; probably use Drupal language objects
    $this->workLanguageOptions = $this->workLanguages;
  }


  public function booksBiblicalOldTestament() {
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


  public function booksBiblicalNewTestament() {
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


  public function booksBiblicalApocrypha() {
    return array(
      'Tob'     => 'Tobit',                      'Jdt'     => 'Judith',
      'GkEsth'  => 'Greek Esther',               'Wis'     => 'Wisdom of Solomon',
      'Sir'     => 'Sirach',                     'Bar'     => 'Baruch',
      'PrAzar'  => 'The Prayer of Azarias',      'Sus'     => 'Susanna',
      'Bel'     => 'Bel and the Dragon',         'SgThree' => 'Song of the Three Children',
      'EpJer'   => 'Epistle of Jeremy',          '1Macc'   => '1 Maccabees',
      '2Macc'   => '2 Maccabees',                '3Macc'   => '3 Maccabees',
      '4Macc'   => '4 Maccabees',                '1Esd'    => '1 Esdras',
      '2Esd'    => '2 Esdras',                   'PrMan'   => 'Prayer of Manasses',
    );
  }


  public function booksQuranicQuran() {
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


}
