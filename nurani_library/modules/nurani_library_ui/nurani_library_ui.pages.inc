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
  // Add the custom bundle UI and connections to the nurani library UI for the
  // passage picker
  $content['#attached'] = nurani_library_ui_attached();
  $content['#attached']['js'][$initializer] = array('type' => 'inline');
  $content['container'] = array(
    '#type' => 'markup',
    // FIXME: Un-hardcode this.
    '#markup' => '<div id="nurani-library" style="width: 100%; height: auto; position: relative; z-index: 100; overflow: hidden; padding: 0.2em;">'
               .    '<div class="titlebar" style="padding: .5em 1em .3em; position: relative; height: 24px; background: #e1e2dc; text-align: center;">(Filler space to compensate for missing titlebar)</div>'
                    // UI will be injected here.
               . '</div>',
  );

  return $content;
}
