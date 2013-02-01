<?php

/**
 * Menu callback, acts as a passthru for create / delete operations on the
 * Nurani Library server. This enables the Javscript client to send
 * POST / PUT / DELETE requests without needing a special mechanism (ie: OAUTH)
 * to verify the user on the Nurani Library Provider end of things.
 */
function nurani_library_annotations($id) {
  global $user;

  $nl = nurani_library();
  $response = array();
  $params = nurani_library_passthru_parse_params();

  switch ($_SERVER['REQUEST_METHOD']) {
    case 'POST':
      $annotation = array(
        'passage_id'  => $params['passage_id'],
        'author_uuid' => $user->uuid,
        'type'        => 'annotation',
        'position'    => $params['position'],
        'length'      => $params['length'],
        'value'       => $params['value'],
      );
      $response = $nl->createAnnotation($annotation);
      break;
    case 'PUT':
      $annotation = array(
        'passage_id'  => $params['passage_id'],
        'author_uuid' => $user->uuid,
        'type'        => 'annotation',
        'position'    => $params['position'],
        'length'      => $params['length'],
        'value'       => $params['value'],
      );
      $response = $nl->updateAnnotation($id, $annotation);
      break;
    case 'DELETE':
      $response = $nl->deleteAnnotation($id);
      break;
    default:
      return nurani_library_passthru_abort('Bad request method.');
  }

  drupal_json_output($response);
}

function nurani_library_passthru_parse_params() {
  $params = array();

  switch ($_SERVER['REQUEST_METHOD']) {
    case 'POST':
      $params = $_POST;
      break;
    case 'PUT':
    case 'DELETE':
      parse_str(file_get_contents('php://input'), $params);
      break;
    case 'GET':
      $params = $_GET;
      break;
  }

  return $params;
}

function nurani_library_passthru_abort($message) {
  drupal_add_http_header('Status', '403 Forbidden');
  drupal_json_output(array('error' => array('message' => $message)));
  return NULL; // Making it clear, this page should not be rendered
}
