<?php

/**
 * 
 */
function nurani_library_ui() {
  $content = array();
  $content['tests'] = array(
    '#theme' => 'item_list',
    '#prefix' => '<strong>' . t("Nurani Library UI Tests") . '</strong>',
    '#items' => array(
      l(t('Picker UI Test'), 'nurani-library/picker-ui'),
    )
  );
  return $content;
}

/**
 * Menu callback for main Nurani Library Picker user interface.
 */
function nurani_library_ui_picker_ui() {
  // Add the custom bundle UI and connections to the nurani library UI for the
  // passage picker
  $path = drupal_get_path('module', 'nurani_library_ui');
  $libraries_path = 'sites/all/libraries';

  $initializer = 'jQuery(function () {'
               .   'var nuraniLibraryPickerUI = new NL.PickerUI({'
               .     'osisIDWork: null,'
               .     'osisID: null'
               .   '});'
               .   'jQuery("#nurani-library").append(nuraniLibraryPickerUI.$element);'
               .   'jQuery("#nurani-library").find(".passage-picker-ui").css({'
               .     'height: 600,'
               .     'padding: "0.5em 1.0em",'
               .     'overflow: "auto",'
               .     'border: 0,'
               .     'position: "relative",'
               .   '});'
               . '});';

  $content = array();
  $content['#attached'] = array(
    'css' => array(
      $path . '/css/nurani_library_ui.css',
    ),
    'js'  => array(
      $libraries_path . '/handlebars/handlebars-1.0.rc.1.js',
      $libraries_path . '/Bible-Passage-Reference-Parser/js/en_bcv_parser.min.js',
      $libraries_path . '/jquery-color/jquery.color-2.1.1.min.js',
      $path . '/js/nurani_library_ui.js',
      $initializer => array(
        'type' => 'inline'
      ),
    ),
  );
  $content['container'] = array(
    '#type' => 'markup',
    // FIXME: Un-hardcode this.
    '#markup' => '<div id="nurani-library" style="width: 100%; height: auto; position: absolute; z-index: 10002; overflow: hidden; padding: 0.2em;">'
               .    '<div class="titlebar" style="padding: .5em 1em .3em; position: relative; height: 24px; background: #e1e2dc; text-align: center;">(Filler space to compensate for missing titlebar)</div>'
                    // UI will be injected here.
               . '</div>',
  );

  return $content;
}
