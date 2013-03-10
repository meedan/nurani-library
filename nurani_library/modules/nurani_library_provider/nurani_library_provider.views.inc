<?php
/**
 * Implements hook_views_data().
 */
function nurani_library_provider_views_data() {
  //
  // {nurani_library} table, contains the verses
  //
  $data['nurani_library']['table']['group'] = t('Nurani library');
  $data['nurani_library']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Nurani verses'),
    'help' => t('Nurani library of scriptural passages.'),
  );
  $data['nurani_library']['id'] = array(
    'title' => t('Verse id'),
    'help' => t('The primary identifier for a verse.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library']['work_id'] = array(
    'title' => t('Work id'),
    'help' => t('The work identifier for a verse.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library']['book_id'] = array(
    'title' => t('Book id'),
    'help' => t('The book identifier for a verse.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library']['chapter_id'] = array(
    'title' => t('Chapter id'),
    'help' => t('The chapter identifier for a verse.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library']['verse'] = array(
    'title' => t('Verse number'),
    'help' => t('The verse number within its chapter.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library']['text'] = array(
    'title' => t('Verse text'),
    'help' => t('The verse\'s text.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  //
  // {nurani_library_works} table
  //
  $data['nurani_library_works']['table']['group']  = t('Nurani library');
  $data['nurani_library_works']['table']['join'] = array(
    'nurani_library' => array(
      'left_field' => 'work_id',
      'field' => 'id',
      'type' => 'INNER',
    ),
  );
  $data['nurani_library_works']['name'] = array(
    'title' => t('Work short name'),
    'help' => t('The short name of the work.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['nurani_library_works']['full_name'] = array(
    'title' => t('Work full name'),
    'help' => t('The full name of the work.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['nurani_library_works']['language'] = array(
    'title' => t('Work language'),
    'help' => t('The language of the work.'),
    'field' => array(
      'handler' => 'views_handler_field_locale_language',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_locale_language',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_locale_language',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  //
  // {nurani_library_books} table
  //
  $data['nurani_library_books']['table']['group']  = t('Nurani library');
  $data['nurani_library_books']['table']['join'] = array(
    'nurani_library' => array(
      'left_field' => 'book_id',
      'field' => 'id',
      'type' => 'INNER',
    ),
  );
  $data['nurani_library_books']['weight'] = array(
    'title' => t('Book order'),
    'help' => t('The order of the book within the work.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library_books']['name'] = array(
    'title' => t('Book short name'),
    'help' => t('The short name of the book.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['nurani_library_books']['full_name'] = array(
    'title' => t('Book full name'),
    'help' => t('The full name of the book.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  //
  // {nurani_library_chapters} table
  //
  $data['nurani_library_chapters']['table']['group']  = t('Nurani library');
  $data['nurani_library_chapters']['table']['join'] = array(
    'nurani_library' => array(
      'left_field' => 'chapter_id',
      'field' => 'id',
      'type' => 'INNER',
    ),
  );
  $data['nurani_library_chapters']['weight'] = array(
    'title' => t('Chapter order'),
    'help' => t('The order of the chapter within the book.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library_chapters']['name'] = array(
    'title' => t('Chapter short name'),
    'help' => t('The short name of the chapter.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['nurani_library_chapters']['full_name'] = array(
    'title' => t('Chapter full name'),
    'help' => t('The full name of the chapter.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  //
  // {nurani_library_annotations} table
  //
  $data['nurani_library_annotations']['table']['group']  = t('Nurani library');
  $data['nurani_library_annotations']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Nurani annotations'),
    'help' => t('Annotations of Nurani Library passages.'),
  );
  $data['nurani_library_annotations']['table']['join'] = array(
    'nurani_library' => array(
      'left_field' => 'id',
      'field' => 'passage_id',
      'type' => 'LEFT',
    ),
  );
  $data['nurani_library_annotations']['id'] = array(
    'title' => t('Annotation id'),
    'help' => t('The primary identifier for an annotation.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library_annotations']['passage_id'] = array(
    'title' => t('Verse id'),
    'help' => t('The verse identifier for an annotation.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library_annotations']['author_uuid'] = array(
    'title' => t('Author UUID'),
    'help' => t('The UUID of the user who created this annotation.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['nurani_library_annotations']['type'] = array(
    'title' => t('Annotation type'),
    'help' => t('The type of annotation (eg: "note" or "annotation").'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['nurani_library_annotations']['position'] = array(
    'title' => t('Annotation position'),
    'help' => t('The word in the verse this annotation is referencing.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library_annotations']['length'] = array(
    'title' => t('Annotation length'),
    'help' => t('The number of words, starting at "postion", being referenced.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['nurani_library_annotations']['value'] = array(
    'title' => t('Annotation value'),
    'help' => t('The contents of the annotation.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
