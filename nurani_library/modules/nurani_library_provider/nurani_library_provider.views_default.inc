<?php

/**
 * Implements hook_views_default_views().
 */
function nurani_library_provider_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'nurani_verses';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'nurani_library';
  $view->human_name = 'Nurani verses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Nurani verses';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Nurani library: Work short name */
  $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
  $handler->display->display_options['fields']['name_2']['table'] = 'nurani_library_works';
  $handler->display->display_options['fields']['name_2']['field'] = 'name';
  /* Field: Nurani library: Work full name */
  $handler->display->display_options['fields']['full_name_2']['id'] = 'full_name_2';
  $handler->display->display_options['fields']['full_name_2']['table'] = 'nurani_library_works';
  $handler->display->display_options['fields']['full_name_2']['field'] = 'full_name';
  /* Field: Nurani library: Work language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'nurani_library_works';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  /* Field: Nurani library: Book short name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'nurani_library_books';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Nurani library: Book full name */
  $handler->display->display_options['fields']['full_name']['id'] = 'full_name';
  $handler->display->display_options['fields']['full_name']['table'] = 'nurani_library_books';
  $handler->display->display_options['fields']['full_name']['field'] = 'full_name';
  /* Field: Nurani library: Chapter short name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'nurani_library_chapters';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  /* Field: Nurani library: Verse number */
  $handler->display->display_options['fields']['verse']['id'] = 'verse';
  $handler->display->display_options['fields']['verse']['table'] = 'nurani_library';
  $handler->display->display_options['fields']['verse']['field'] = 'verse';
  /* Field: Nurani library: Verse text */
  $handler->display->display_options['fields']['text']['id'] = 'text';
  $handler->display->display_options['fields']['text']['table'] = 'nurani_library';
  $handler->display->display_options['fields']['text']['field'] = 'text';
  /* Field: COUNT(Nurani library: Annotation id) */
  $handler->display->display_options['fields']['id_2']['id'] = 'id_2';
  $handler->display->display_options['fields']['id_2']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['fields']['id_2']['field'] = 'id';
  $handler->display->display_options['fields']['id_2']['group_type'] = 'count';
  $handler->display->display_options['fields']['id_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['id_2']['alter']['text'] = 'View [id_2] annotations';
  $handler->display->display_options['fields']['id_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['id_2']['alter']['path'] = 'nurani-library/verses/[verse]';
  $handler->display->display_options['fields']['id_2']['empty'] = 'N/A';
  $handler->display->display_options['fields']['id_2']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['id_2']['separator'] = '';
  /* Sort criterion: Nurani library: Work id */
  $handler->display->display_options['sorts']['work_id']['id'] = 'work_id';
  $handler->display->display_options['sorts']['work_id']['table'] = 'nurani_library';
  $handler->display->display_options['sorts']['work_id']['field'] = 'work_id';
  /* Sort criterion: Nurani library: Book order */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'nurani_library_books';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Sort criterion: Nurani library: Chapter order */
  $handler->display->display_options['sorts']['weight_1']['id'] = 'weight_1';
  $handler->display->display_options['sorts']['weight_1']['table'] = 'nurani_library_chapters';
  $handler->display->display_options['sorts']['weight_1']['field'] = 'weight';
  /* Sort criterion: Nurani library: Verse number */
  $handler->display->display_options['sorts']['verse']['id'] = 'verse';
  $handler->display->display_options['sorts']['verse']['table'] = 'nurani_library';
  $handler->display->display_options['sorts']['verse']['field'] = 'verse';
  /* Filter criterion: Nurani library: Book full name */
  $handler->display->display_options['filters']['full_name']['id'] = 'full_name';
  $handler->display->display_options['filters']['full_name']['table'] = 'nurani_library_books';
  $handler->display->display_options['filters']['full_name']['field'] = 'full_name';
  $handler->display->display_options['filters']['full_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['full_name']['expose']['operator_id'] = 'full_name_op';
  $handler->display->display_options['filters']['full_name']['expose']['label'] = 'Book full name';
  $handler->display->display_options['filters']['full_name']['expose']['operator'] = 'full_name_op';
  $handler->display->display_options['filters']['full_name']['expose']['identifier'] = 'full_name';
  /* Filter criterion: Nurani library: Book short name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'nurani_library_books';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Book machine name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  /* Filter criterion: Nurani library: Chapter full name */
  $handler->display->display_options['filters']['full_name_1']['id'] = 'full_name_1';
  $handler->display->display_options['filters']['full_name_1']['table'] = 'nurani_library_chapters';
  $handler->display->display_options['filters']['full_name_1']['field'] = 'full_name';
  $handler->display->display_options['filters']['full_name_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['full_name_1']['expose']['operator_id'] = 'full_name_1_op';
  $handler->display->display_options['filters']['full_name_1']['expose']['label'] = 'Chapter full name';
  $handler->display->display_options['filters']['full_name_1']['expose']['operator'] = 'full_name_1_op';
  $handler->display->display_options['filters']['full_name_1']['expose']['identifier'] = 'full_name_1';
  /* Filter criterion: Nurani library: Chapter short name */
  $handler->display->display_options['filters']['name_1']['id'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['table'] = 'nurani_library_chapters';
  $handler->display->display_options['filters']['name_1']['field'] = 'name';
  $handler->display->display_options['filters']['name_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_1']['expose']['operator_id'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['label'] = 'Chapter machine name';
  $handler->display->display_options['filters']['name_1']['expose']['operator'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['identifier'] = 'name_1';
  /* Filter criterion: Nurani library: Work full name */
  $handler->display->display_options['filters']['full_name_2']['id'] = 'full_name_2';
  $handler->display->display_options['filters']['full_name_2']['table'] = 'nurani_library_works';
  $handler->display->display_options['filters']['full_name_2']['field'] = 'full_name';
  $handler->display->display_options['filters']['full_name_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['full_name_2']['expose']['operator_id'] = 'full_name_2_op';
  $handler->display->display_options['filters']['full_name_2']['expose']['label'] = 'Work full name';
  $handler->display->display_options['filters']['full_name_2']['expose']['operator'] = 'full_name_2_op';
  $handler->display->display_options['filters']['full_name_2']['expose']['identifier'] = 'full_name_2';
  /* Filter criterion: Nurani library: Work language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'nurani_library_works';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['exposed'] = TRUE;
  $handler->display->display_options['filters']['language']['expose']['operator_id'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['label'] = 'Work language';
  $handler->display->display_options['filters']['language']['expose']['operator'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['identifier'] = 'language';
  /* Filter criterion: Nurani library: Work short name */
  $handler->display->display_options['filters']['name_2']['id'] = 'name_2';
  $handler->display->display_options['filters']['name_2']['table'] = 'nurani_library_works';
  $handler->display->display_options['filters']['name_2']['field'] = 'name';
  $handler->display->display_options['filters']['name_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_2']['expose']['operator_id'] = 'name_2_op';
  $handler->display->display_options['filters']['name_2']['expose']['label'] = 'Work machine name';
  $handler->display->display_options['filters']['name_2']['expose']['operator'] = 'name_2_op';
  $handler->display->display_options['filters']['name_2']['expose']['identifier'] = 'name_2';
  /* Filter criterion: Nurani library: Verse number */
  $handler->display->display_options['filters']['verse']['id'] = 'verse';
  $handler->display->display_options['filters']['verse']['table'] = 'nurani_library';
  $handler->display->display_options['filters']['verse']['field'] = 'verse';
  $handler->display->display_options['filters']['verse']['exposed'] = TRUE;
  $handler->display->display_options['filters']['verse']['expose']['operator_id'] = 'verse_op';
  $handler->display->display_options['filters']['verse']['expose']['label'] = 'Verse number';
  $handler->display->display_options['filters']['verse']['expose']['operator'] = 'verse_op';
  $handler->display->display_options['filters']['verse']['expose']['identifier'] = 'verse';
  /* Filter criterion: Nurani library: Verse text */
  $handler->display->display_options['filters']['text']['id'] = 'text';
  $handler->display->display_options['filters']['text']['table'] = 'nurani_library';
  $handler->display->display_options['filters']['text']['field'] = 'text';
  $handler->display->display_options['filters']['text']['operator'] = 'contains';
  $handler->display->display_options['filters']['text']['exposed'] = TRUE;
  $handler->display->display_options['filters']['text']['expose']['operator_id'] = 'text_op';
  $handler->display->display_options['filters']['text']['expose']['label'] = 'Verse text contains';
  $handler->display->display_options['filters']['text']['expose']['operator'] = 'text_op';
  $handler->display->display_options['filters']['text']['expose']['identifier'] = 'text';
  /* Filter criterion: Nurani library: Book order */
  $handler->display->display_options['filters']['weight']['id'] = 'weight';
  $handler->display->display_options['filters']['weight']['table'] = 'nurani_library_books';
  $handler->display->display_options['filters']['weight']['field'] = 'weight';
  $handler->display->display_options['filters']['weight']['exposed'] = TRUE;
  $handler->display->display_options['filters']['weight']['expose']['operator_id'] = 'weight_op';
  $handler->display->display_options['filters']['weight']['expose']['label'] = 'Book order';
  $handler->display->display_options['filters']['weight']['expose']['operator'] = 'weight_op';
  $handler->display->display_options['filters']['weight']['expose']['identifier'] = 'weight';
  /* Filter criterion: Nurani library: Chapter order */
  $handler->display->display_options['filters']['weight_1']['id'] = 'weight_1';
  $handler->display->display_options['filters']['weight_1']['table'] = 'nurani_library_chapters';
  $handler->display->display_options['filters']['weight_1']['field'] = 'weight';
  $handler->display->display_options['filters']['weight_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['weight_1']['expose']['operator_id'] = 'weight_1_op';
  $handler->display->display_options['filters']['weight_1']['expose']['label'] = 'Chapter order';
  $handler->display->display_options['filters']['weight_1']['expose']['operator'] = 'weight_1_op';
  $handler->display->display_options['filters']['weight_1']['expose']['identifier'] = 'weight_1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'nurani-library/verses';

  /* Display: Annotations for verse */
  $handler = $view->new_display('page', 'Annotations for verse', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Unfiltered text */
  $handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['header']['area_text_custom']['content'] = '<a href="/nurani-library/verses">&lsaquo; Back to all verses</a>';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Nurani library: Work full name */
  $handler->display->display_options['fields']['full_name_2']['id'] = 'full_name_2';
  $handler->display->display_options['fields']['full_name_2']['table'] = 'nurani_library_works';
  $handler->display->display_options['fields']['full_name_2']['field'] = 'full_name';
  /* Field: Nurani library: Book full name */
  $handler->display->display_options['fields']['full_name']['id'] = 'full_name';
  $handler->display->display_options['fields']['full_name']['table'] = 'nurani_library_books';
  $handler->display->display_options['fields']['full_name']['field'] = 'full_name';
  /* Field: Nurani library: Chapter full name */
  $handler->display->display_options['fields']['full_name_1']['id'] = 'full_name_1';
  $handler->display->display_options['fields']['full_name_1']['table'] = 'nurani_library_chapters';
  $handler->display->display_options['fields']['full_name_1']['field'] = 'full_name';
  /* Field: Nurani library: Verse number */
  $handler->display->display_options['fields']['verse']['id'] = 'verse';
  $handler->display->display_options['fields']['verse']['table'] = 'nurani_library';
  $handler->display->display_options['fields']['verse']['field'] = 'verse';
  /* Field: Nurani library: Annotation id */
  $handler->display->display_options['fields']['id_2']['id'] = 'id_2';
  $handler->display->display_options['fields']['id_2']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['fields']['id_2']['field'] = 'id';
  $handler->display->display_options['fields']['id_2']['separator'] = '';
  /* Field: Nurani library: Annotation type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Nurani library: Author UUID */
  $handler->display->display_options['fields']['author_uuid']['id'] = 'author_uuid';
  $handler->display->display_options['fields']['author_uuid']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['fields']['author_uuid']['field'] = 'author_uuid';
  /* Field: Nurani library: Annotation position */
  $handler->display->display_options['fields']['position']['id'] = 'position';
  $handler->display->display_options['fields']['position']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['fields']['position']['field'] = 'position';
  /* Field: Nurani library: Annotation length */
  $handler->display->display_options['fields']['length']['id'] = 'length';
  $handler->display->display_options['fields']['length']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['fields']['length']['field'] = 'length';
  /* Field: Nurani library: Annotation value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Nurani library: Verse id */
  $handler->display->display_options['arguments']['passage_id']['id'] = 'passage_id';
  $handler->display->display_options['arguments']['passage_id']['table'] = 'nurani_library_annotations';
  $handler->display->display_options['arguments']['passage_id']['field'] = 'passage_id';
  $handler->display->display_options['arguments']['passage_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['passage_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['passage_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['passage_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['passage_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'nurani-library/verses/%';
  $translatables['nurani_verses'] = array(
    t('Master'),
    t('Nurani verses'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Work short name'),
    t('Work full name'),
    t('Work language'),
    t('Book short name'),
    t('Book full name'),
    t('Chapter short name'),
    t('Verse number'),
    t('.'),
    t(','),
    t('Verse text'),
    t('Annotation id'),
    t('View [id_2] annotations'),
    t('N/A'),
    t('Book machine name'),
    t('Chapter full name'),
    t('Chapter machine name'),
    t('Work machine name'),
    t('Verse text contains'),
    t('Book order'),
    t('Chapter order'),
    t('Page'),
    t('Annotations for verse'),
    t('<a href="/nurani-library/verses">&lsaquo; Back to all verses</a>'),
    t('Annotation type'),
    t('Author UUID'),
    t('Annotation position'),
    t('Annotation length'),
    t('Annotation value'),
    t('All'),
  );

  $views['nurani_verses'] = $view;

  return $views;
}
