<?php
ini_set('memory_limit', '512M');

$xmls = array();
$all_data = '';
foreach (explode("\n", `ls`) as $file) {
  echo $file . "\n";

  $data = file_get_contents($file);
  $all_data .= $data;
  $xmls[] = new SimpleXMLElement($data);

  echo "u: " . memory_get_usage() . " ru: " . memory_get_usage(TRUE) . "\n";
  sleep(1);
}

echo "DONE\n";
sleep(300);
