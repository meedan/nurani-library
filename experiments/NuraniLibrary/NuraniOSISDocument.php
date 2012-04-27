<?php

require_once 'NuraniDocument.php';

/**
 * NuraniOSISDocument
 * 
 * 
 */
class NuraniOSISDocument extends NuraniDocument {

  public $xml;


  function __construct($path, $file) {
    parent::__construct($path, $file);
  }


  function load() {
    parent::load();

    $this->xml = new SimpleXMLElement($this->contents);
  }

}