<?php

require_once 'NuraniLibrary.php';

$import = array();

// Westminster Leningrad Codex (WLC).
$import['WLC'] = array(
  'path'          => dirname(__FILE__) . '/data/wlc',
  'documentType'  => 'OSIS',
  'language'      => 'he',
  'textDirection' => 'rtl',
  'stripMarkup'   => TRUE,
  'stripChars'    => '/',
  'books' => array(
    'Gen.xml',   'Exod.xml',  'Lev.xml',
    'Num.xml',   'Deut.xml',  'Josh.xml',
    'Judg.xml',  'Ruth.xml',  '1Sam.xml',
    '2Sam.xml',  '1Kgs.xml',  '2Kgs.xml',
    '1Chr.xml',  '2Chr.xml',  'Ezra.xml',
    'Neh.xml',   'Esth.xml',  'Job.xml',
    'Ps.xml',    'Prov.xml',  'Eccl.xml',
    'Song.xml',  'Isa.xml',   'Jer.xml',
    'Lam.xml',   'Ezek.xml',  'Dan.xml',
    'Hos.xml',   'Joel.xml',  'Amos.xml',
    'Obad.xml',  'Jonah.xml', 'Mic.xml',
    'Nah.xml',   'Hab.xml',   'Zeph.xml',
    'Hag.xml',   'Zech.xml',  'Mal.xml',
  ),
);


$library = new NuraniLibrary(array('backend' => 'Drupal'));
$library->import($import);
