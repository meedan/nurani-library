<?php

/**
 * NuraniDocument
 * 
 * Abstract parent class for a document container.
 */
class NuraniDocument {

  protected $path;
  protected $filepath;
  protected $contents;


  function __construct($path, $file) {
    $this->path = $path;
    $this->filepath = $path . '/' . $file;

    if (!is_file($this->filepath)) {
      throw new Exception("NuraniDocument: Could not load {$this->filepath}");
    }
  }


  function load() {
    $this->contents = file_get_contents($this->filepath);
  }

}
