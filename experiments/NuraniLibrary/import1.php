<?php

require_once 'NuraniLibrary.php';

$import = array();

// Westminster Leningrad Codex (WLC).
$import['WLC'] = array(
  'documentType' => 'OSIS',
  'path' => dirname(__FILE__) . '/data/wlc',
  'textDirection' => 'rtl',
  'stripMarkup' => TRUE,
  'stripChars' => '/',
  'chapters' => array(
    'Gen'   => 'Gen.xml',   'Exod'  => 'Exod.xml',  'Lev'   => 'Lev.xml',
    'Num'   => 'Num.xml',   'Deut'  => 'Deut.xml',  'Josh'  => 'Josh.xml',
    'Judg'  => 'Judg.xml',  'Ruth'  => 'Ruth.xml',  '1Sam'  => '1Sam.xml',
    '2Sam'  => '2Sam.xml',  '1Kgs'  => '1Kgs.xml',  '2Kgs'  => '2Kgs.xml',
    '1Chr'  => '1Chr.xml',  '2Chr'  => '2Chr.xml',  'Ezra'  => 'Ezra.xml',
    'Neh'   => 'Neh.xml',   'Esth'  => 'Esth.xml',  'Job'   => 'Job.xml',
    'Ps'    => 'Ps.xml',    'Prov'  => 'Prov.xml',  'Eccl'  => 'Eccl.xml',
    'Song'  => 'Song.xml',  'Isa'   => 'Isa.xml',   'Jer'   => 'Jer.xml',
    'Lam'   => 'Lam.xml',   'Ezek'  => 'Ezek.xml',  'Dan'   => 'Dan.xml',
    'Hos'   => 'Hos.xml',   'Joel'  => 'Joel.xml',  'Amos'  => 'Amos.xml',
    'Obad'  => 'Obad.xml',  'Jonah' => 'Jonah.xml', 'Mic'   => 'Mic.xml',
    'Nah'   => 'Nah.xml',   'Hab'   => 'Hab.xml',   'Zeph'  => 'Zeph.xml',
    'Hag'   => 'Hag.xml',   'Zech'  => 'Zech.xml',  'Mal'   => 'Mal.xml',
  ),
);


$library = new NuraniLibrary();
$library->import($import);
var_export($library);
