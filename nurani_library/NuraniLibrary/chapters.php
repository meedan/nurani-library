<?php

$name = $argv[1];
$file = $argv[2];

$contents = file_get_contents($file);
$xml = new SimpleXMLElement(str_replace('xmlns=', 'ns=', $contents));

foreach ($xml->xpath('//div[@type="book"]') as $book) {
  $name = (string) $book['osisID'];

  $chapters = array();
  foreach ($book->xpath('chapter[@osisID]') as $chapter) {
    $id = (string) $chapter['osisID'];

    if ($id) {
      $id_parts = explode('.', $id);
      $chapters[] = $id_parts[1];
    }
  }

  // Biblical ones are in order
  $last_chapter = end($chapters);
  echo "  '$name' => range(1, $last_chapter),\n";
}

