<?php

/**
 * Theme the embeddable passages widget.
 */
function theme_nurani_library_provider_passage_widget(&$variables) {
  $osisIDWork = $variables['osisIDWork'];
  $osisID     = $variables['osisID'];
  $passages   = $variables['passages'];

  foreach ($passages as $passage) {
    $passage_osisID = "$passage->book_name.$passage->chapter_name.$passage->verse";

    $passage_output .= '<p>';
    $passage_output .=   '<span class="verse">[' . l($passage->verse, "passages/$osisIDWork/$passage_osisID") . ']</span>';
    $passage_output .=   $passage->text;
    $passage_output .= '</p>';
  }

  $output  = '';
  $output .= '<div class="passage-widget">';
  $output .=   $passage_output;
  $output .= '</div>';

  return $output;
}
