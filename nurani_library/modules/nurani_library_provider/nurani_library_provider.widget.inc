<?php

/**
 * Theme the embeddable passages widget.
 */
function theme_nurani_library_provider_passage_widget(&$variables) {
  $osisIDWork = $variables['osisIDWork'];
  $osisID     = $variables['osisID'];
  $passages   = $variables['passages'];

  $first = current($passages);

  $passage_output = '';
  foreach ($passages as $passage) {
    $passage_osisID = "$passage->book_name.$passage->chapter_name.$passage->verse";

    $passage_output .= '<p class="' . $passage->work_language . '">';
    $passage_output .=   '<span class="verse">' . nurani_library_l($passage->verse, "passages/$osisIDWork/$passage_osisID") . '</span>';
    $passage_output .=   $passage->text;
    $passage_output .= '</p>';
  }

  $output  = '';
  $output .= '<div class="passage-widget">';
  $output .=   '<h3>' . nurani_library_format_passage_string_from_passages($passages) . '</h3>';
  $output .=   '<h4>' . $first->work_full_name . '</h4>';
  $output .=   $passage_output;
  $output .= '</div>';

  return $output;
}
