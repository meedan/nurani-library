<?php

function nurani_library() {
  return new NuraniLibrary(array('backend' => 'Drupal'));
}

function nurani_library_works_path() {
  return variable_get('nurani_library_works_path', 'sites/all/works');
}

/**
 * Searches in $path for XML files which may be valid works.
 */
function nurani_library_find_works($path) {
  $files = glob_recursive($path . '/*.xml');
  asort($files);

  $path_regexp = '/^' . preg_quote($path . '/', '/') . '/';

  $works = array();
  $directories = array();
  foreach ($files as $filepath) {
    // Skip anything that is not a valid format
    if (($format = nurnai_library_discover_work_format($filepath)) === FALSE) {
      continue;
    }

    $relativepath = preg_replace($path_regexp, '', $filepath);
    $depth = 0;

    $parts = explode('/', $relativepath);
    if ($parts > 1) {
      $depth = count($parts);

      // Remove the actual filename, only want the path components
      array_pop($parts);

      $stack = array();
      foreach ($parts as $part) {
        $stack[] = $part;
        $current_directory = implode('/', $stack);

        if (!in_array($current_directory, $directories)) {
          $directories[] = $current_directory;
          $current_directory_fullpath = $path . '/' . $current_directory;

          if (is_dir($current_directory_fullpath)) {
            $works[] = (object) array(
              'filepath' => $current_directory_fullpath,
              'relativepath' => $current_directory,
              'filename' => basename($current_directory_fullpath),
              'is_dir' => TRUE,
              'format' => FALSE,
              'depth' => count($stack)
            );
          }
        }
      }
    }

    $works[] = (object) array(
      'filepath' => $filepath,
      'relativepath' => $relativepath,
      'filename' => basename($relativepath),
      'is_dir' => FALSE,
      'format' => $format,
      'depth' => $depth,
    );
  }

  return $works;
}

/**
 * Determines if a file is a valid OSIS or Tanzil formatted work.  If it is
 * the format is returned.
 * 
 * TODO: This function is fairly quick but not very tolerant of subtle differences.  Perhaps use a full XML parser here?
 */
function nurnai_library_discover_work_format($filepath) {
  // Crude search tree
  $format_tree = array(
    '<?xml version="1.0" encoding="utf-8"' => array(
      'xsi:schemaLocation="http://www.bibletechnologies.net/2003/OSIS/namespace http://www.bibletechnologies.net/osisCore.2.1.1.xsd"' => array(
        'osis',
      ),
      '<quran>' => array(
        '<sura' => array(
          '<aya' => array(
            'tanzil'
          )
        )
      )
    )
  );

  $format = FALSE; // The matched format
  $lines = 512; // All of these patterns should be matchable in the first 500 lines

  $fp = fopen($filepath, 'r');
  while (($buffer = fgets($fp, 4096)) !== FALSE && !$format && $lines > 0) {
    foreach ($format_tree as $needle => $branch) {
      if (mb_strpos($buffer, $needle) !== FALSE) {
        // Matched the final needle in this tree, format discovered!
        if (count($branch) == 1 && isset($branch[0]) && is_string($branch[0])) {
          $format = $branch[0];
        }

        // Found a match, replace the tree with this branch.  This is an
        // okay optimization in this case because we never have need for
        // the remainder of the tree again.
        $format_tree = $branch;
        break;
      }
    }

    $lines--;
  }

  return $format;
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

function nurani_library_work_format_link($format) {
  switch ($format) {
    case 'osis':   return l('OSIS', 'http://www.crosswire.org/osis/');
    case 'tanzil': return l('Tanzil XML', 'http://tanzil.net/download/');
    default:       return '';
  }
}

function nurani_library_work_languages() {
  return array(
    'ar' => t("Arabic (ar)"),
    'en' => t("English (en)"),
    'fr' => t("French (fr)"),
    'he' => t("Hebrew (he)"),
  );
}

/**
 * Returns the list of works known to the system.
 */
function nurani_library_works() {
  $result = db_query("SELECT * FROM {nurani_library_works} ORDER BY name, language");
  $works = array();
  foreach ($result as $work) {
    $works[] = $work;
  }
  return $works;
}

function nurani_library_work_options() {
  $options = array();
  foreach (nurani_library_works() as $work) {
    $options[$work->name] = $work->full_name;
  }
  return $options;
}

/**
 * Returns the list of works known to the system.
 */
function nurani_library_work_load($work_name) {
  return db_query("SELECT * FROM {nurani_library_works} WHERE name = :name", array(':name' => $work_name))->fetchObject();
}

function nurani_library_work_types() {
  $work = new NuraniWork();
  return $work->workTypes;
}

function nurani_library_work_type_options() {
  $options = array();
  foreach (nurani_library_work_types() as $machine_name => $type) {
    $options[$machine_name] = $type->name;
  }
  return $options;
}
