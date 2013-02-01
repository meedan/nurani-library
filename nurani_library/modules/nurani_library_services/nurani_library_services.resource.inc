<?php

/**
 * Deferred implementation of hook_services_resources().
 */
function _nurani_library_services_services_resources() {
  return array(

    'passage' => array(
      'actions' => array(
        'validate' => array(
          'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
          'callback' => '_nurani_library_services_api_passage_validate',
          'args' => array(
            array(
              'name' => 'work_name',
              'optional' => FALSE,
              'type' => 'string',
              'description' => 'The machine name of the work containing the passage to validate',
              'source' => array('data' => 'work_name'),
            ),
            array(
              'name' => 'osisID',
              'optional' => FALSE,
              'type' => 'string',
              'description' => 'The OSIS identifier of the passage to validate',
              'source' => array('data' => 'osisID'),
            ),
          ),
          'access arguments' => array('access nurani library'),
        ),
      ),
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_passage_retrieve',
        'args' => array(
          array(
            'name' => 'work_name',
            'optional' => FALSE,
            'type' => 'string',
            'description' => 'The machine name of the work containing the passage to get',
            'source' => array('path' => 0),
          ),
          array(
            'name' => 'osisID',
            'optional' => FALSE,
            'type' => 'string',
            'description' => 'The OSIS identifier of the passage to get',
            'source' => array('path' => 1),
          ),
        ),
        'access arguments' => array('access nurani library'),
      ),
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_passage_index',
        'args' => array(
          array(
            'name' => 'work_name',
            'optional' => FALSE,
            'type' => 'string',
            'description' => 'The machine name of the work containing the index to get',
            'source' => array('param' => 'work_name'),
          ),
          array(
            'name' => 'book',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The OSIS name of the book to filter this work by (eg: John, 1Chr, Quran)',
            'source' => array('param' => 'book'),
          ),
          array(
            'name' => 'chapter',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The number of the chapter to filter this work by.',
            'source' => array('param' => 'chapter'),
          ),
          array(
            'name' => 'verse',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The number of the verse to filter this work by, ranges are okay.',
            'source' => array('param' => 'verse'),
          ),
          array(
            'name' => 'page',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The zero-based index of the page to get, defaults to 0.',
            'default value' => 0,
            'source' => array('param' => 'page'),
          ),
          array(
            'name' => 'pagesize',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'Number of records to get per page.',
            'default value' => variable_get('nurani_library_services_api_pagesize', 20),
            'source' => array('param' => 'pagesize'),
          ),
        ),
        'access arguments' => array('access nurani library'),
      ),
    ),

    'annotation' => array(
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_annotation_retrieve',
        'args' => array(
          array(
            'name' => 'id',
            'optional' => FALSE,
            'type' => 'int',
            'description' => 'The id of a Nurani Library annotation.',
            'source' => array('path' => 0),
          ),
        ),
        'access arguments' => array('access nurani library'), // TODO: Take private annotations into account
      ),
      'create' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_annotation_create',
        'args' => array(
          array(
            'name' => 'annotation',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The data used to create a Nurani Library annotation.',
            'type' => 'array',
          ),
        ),
        'access arguments' => array('create nurani library annotations'), // TODO: Take private annotations into account
      ),
      'update' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_annotation_update',
        'args' => array(
          array(
            'name' => 'id',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The id of a Nurani Library annotation.',
          ),
          array(
            'name' => 'annotation',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The Nurani Library annotation data to update.',
            'type' => 'array',
          ),
        ),
        'access arguments' => array('update nurani library annotations'), // TODO: Take private annotations into account
      ),
      'delete' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_annotation_delete',
        'args' => array(
          array(
            'name' => 'id',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The id of a Nurani Library annotation.',
          ),
        ),
        'access arguments' => array('delete nurani library annotations'), // TODO: Take private annotations into account
      ),
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_annotation_index',
        'args' => array(
          array(
            'name' => 'passage_id',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The id of a Nurani Library passage.',
            'source' => array('path' => 0),
          ),
          array(
            'name' => 'page',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The zero-based index of the page to get, defaults to 0.',
            'default value' => 0,
            'source' => array('param' => 'page'),
          ),
          array(
            'name' => 'pagesize',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'Number of records to get per page.',
            'default value' => variable_get('nurani_library_services_api_pagesize', 20),
            'source' => array('param' => 'pagesize'),
          ),
        ),
        'access arguments' => array('access nurani library'), // TODO: Take private annotations into account
      ),
    ),

    'work' => array(
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_work_retrieve',
        'args' => array(
          array(
            'name' => 'work_name',
            'optional' => FALSE,
            'type' => 'string',
            'description' => 'The machine name of the work to get',
            'source' => array('path' => 0),
          ),
        ),
        'access arguments' => array('access nurani library'),
      ),
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'nurani_library_services', 'name' => 'nurani_library_services.resource'),
        'callback' => '_nurani_library_services_api_work_index',
        'access arguments' => array('access nurani library'),
      ),
    ),

  );
}

/**
 * Retrieves a passage from a work.
 *
 * @return (array) $verses
 *  A list of verses that comprise the passage or an empty array no passage
 *  was located.
 */
function _nurani_library_services_api_passage_retrieve($work_name, $osisID) {
  $parts = explode('.', $osisID);

  $nl = nurani_library();
  $verses = $nl->search($work_name, $parts[0], $parts[1], $parts[2]);
  // TODO: Handle errors.

  return $verses;
}

/**
 * Returns a list of verses in a work.  Optionally, the list can be filtered
 * by book / chapter.
 *
 * @return (array) $verses
 *  A list of verses corresponding to the query or an empty array if no verses
 *  match that query.
 */
function _nurani_library_services_api_passage_index($work_name, $book, $chapter, $verse, $page, $pagesize) {
  $nl = nurani_library();
  return $nl->search($work_name, $book, $chapter, $verse, $page, $pagesize);
}

/**
 * Returns TRUE or FALSE if a passage is found within a work.
 *
 * @return (boolean) $is_passage_valid
 */
function _nurani_library_services_api_passage_validate($work_name, $osisID) {
  $verses = _nurani_library_services_api_passage_retrieve($work_name, $osisID);
  // TODO: Handle errors.

  return (count($verses) > 0);
}

/**
 *
 */
function _nurani_library_services_api_annotation_retrieve($id) {
  $nl = nurani_library();
  return $nl->getAnnotation($id);
}

/**
 *
 */
function _nurani_library_services_api_annotation_create($annotation) {
  $nl = nurani_library();
  $annotation['author_uuid'] = $GLOBALS['user']->uuid;
  return $nl->createAnnotation($annotation);
}

/**
 *
 */
function _nurani_library_services_api_annotation_update($id, $annotation) {
  $nl = nurani_library();
  return $nl->updateAnnotation($id, $annotation);
}

/**
 *
 */
function _nurani_library_services_api_annotation_delete($id) {
  $nl = nurani_library();
  return $nl->deleteAnnotation($id);
}

/**
 *
 */
function _nurani_library_services_api_annotation_index($passage_id, $page, $pagesize) {
  $nl = nurani_library();
  return $nl->getAnnotations($passage_id, $page, $pagesize);
}

/**
 * Returns a work given its name.
 *
 * @return (object) $work
 */
function _nurani_library_services_api_work_retrieve($work_name) {
  $nl = nurani_library();
  $work = $nl->getWork($work_name);
  return $work;
}

/**
 * Returns an array of all works.
 *
 * @return (array) $works
 */
function _nurani_library_services_api_work_index() {
  $nl = nurani_library();
  $works = $nl->getWorks();
  return $works;
}

