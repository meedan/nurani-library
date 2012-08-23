<?php

/**
 * Nurani Library administrative settings form.
 */
function nurani_library_admin_form($form, &$form_state) {
  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => '<em>Free-form markup.</em>',
  );
  return $form;
}
