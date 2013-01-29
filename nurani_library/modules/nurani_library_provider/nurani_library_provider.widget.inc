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
    // $passage_output .=   '<span class="verse">' . nurani_library_l($passage->verse, "passages/$osisIDWork/$passage_osisID") . '</span>';
    $passage_output .=   '<span class="verse"><strong>' . $passage->verse . '</strong></span>';
    $passage_output .=   theme('nurani_library_provider_passage_text', array('passage' => $passage));
    $passage_output .= '</p>';
  }

  $output  = '';
  $output .= '<div class="passage-widget">';
  if ($passage_output) {
    $output .= '<h3>' . nurani_library_format_passage_string_from_passages($passages) . '</h3>';
    $output .= '<h4>' . $first->work_full_name . '</h4>';
    $output .= '<div class="passages">';
    $output .=   $passage_output;
    $output .= '</div>';
  }
  else {
    $output .= '<div class="message error">' . t('We are sorry. An error has occurred and this passage cannot be displayed. Please try again or contact the @site team.', array('@site' => variable_get('site_name', 'Nurani'))) . '</div>';
  }
  $output .= '</div>';

  return $output;
}

/**
 * Returns the passage value with notes embedded ready for display.
 */
function theme_nurani_library_provider_passage_text(&$variables) {
  $passage = $variables['passage'];

  if (!isset($passage->notes) || empty($passage->notes)) {
    return $passage->text;
  }

  $words = NuraniLibrary::passageTextWords($passage->text);

  foreach ($passage->notes as $note) {
    $index = $note->position + $note->length - 1;

    if (isset($words[$index])) {
      $classes = array('note');

      if ($note->type != 'note') {
        $classes[] = $note->type;
      }

      $words[$index] .= '<span class="' . implode(' ', $classes) . '">' . $note->value . '</span>';
    }
  }

  return implode(' ', $words);
}
