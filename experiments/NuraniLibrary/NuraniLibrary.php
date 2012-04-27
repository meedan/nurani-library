<?php

require_once 'NuraniOSISDocument.php';

/**
 * NuraniLibrary
 * 
 * 
 */
class NuraniLibrary {

  public $library;
  public $text;
  public $chapter;
  public $document;


  function __construct() {
    require 'NuraniConfig.php'; // Adds $library into the namespace
    $this->library = $library;
  }


  /**
   * @param $text
   *  The textual corpus to connect to, eg: 'NIV' or 'WLC'.
   */
  function loadText($text, $chapter = NULL) {
    if (!array_key_exists($text, $this->library)) {
      return FALSE;
    }

    $info = $this->library[$text];
    if (!array_key_exists($chapter, $info['chapters'])) {
      return FALSE;
    }

    $this->text     = $text;
    $this->chapter  = $chapter;
    $this->document = NULL;

    switch ($info['documentType']) {
      case 'OSIS':
        $this->document = new NuraniOSISDocument($info['path'], $info['chapters'][$chapter]);
        break;
    }

    if (!$this->document) {
      return FALSE;
    }

    $this->document->load();
  }

}
