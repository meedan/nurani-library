<?php

/**
 * Implements hook_views_data().
 */
function nurani_library_views_data() {
  $data['nurani_library']['table']['group']  = t('Nurani library');

  $data['nurani_library']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Nurani verses'),
    'help' => t('Nurani library of scriptural texts.'),
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

  $data['nurani_library']['corpus_id'] = array(
    'title' => t('Corpus id'),
    'help' => t('The corpus identifier for a verse.'),
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
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['nurani_library_corpora']['table']['group']  = t('Nurani library');

  $data['nurani_library_corpora']['table']['join'] = array(
    'nurani_library' => array(
      'left_field' => 'corpus_id',
      'field' => 'id',
      'type' => 'INNER',
    ),
  );

  $data['nurani_library_corpora']['name'] = array(
    'title' => t('Corpus short name'),
    'help' => t('The short name of the corpus.'),
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

  $data['nurani_library_corpora']['full_name'] = array(
    'title' => t('Corpus full name'),
    'help' => t('The full name of the corpus.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['nurani_library_corpora']['language'] = array(
    'title' => t('Corpus language'),
    'help' => t('The language of the corpus.'),
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
    'help' => t('The order of the book within the corpus.'),
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
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

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
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}

