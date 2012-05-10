<?php

/**
 * NuraniDocument
 * 
 * Abstract parent class for a document container.
 */
abstract class NuraniDocument {

  public $contents;
  public $conf;

  protected $path;
  protected $filepath;
  protected $rawContents;


  public function __construct($path, $file, $conf) {
    $this->path     = $path;
    $this->filepath = $path . '/' . $file;
    $this->conf     = $conf;
    $this->contents = array();

    // Explode the stripChars into an array
    if ($this->conf['stripChars'] && strlen($this->conf['stripChars']) > 1) {
      $this->conf['stripChars'] = str_split($this->conf['stripChars']);
    }

    if (!is_file($this->filepath)) {
      throw new Exception("NuraniDocument: Could not load {$this->filepath}");
    }
  }


  public function load() {
    $this->rawContents = file_get_contents($this->filepath);
  }


}
