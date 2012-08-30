<?php

/**
 * 
 */
function _nurani_library_provider_api_passage_retrieve($osisID) {
  $return = (object) array('retrieve' => $osisID);
  return $return;
}

/**
 * 
 */
function _nurani_library_provider_api_passage_index($page, $parameters, $pagesize) {
  $return = array($page, $parameters, $pagesize);
  return $return;
}

/**
 * 
 */
function _nurani_library_provider_api_passage_validate($osisID) {
  $return = (object) array('validate' => $osisID);
  return $return;
}

/**
 * 
 */
function _nurani_library_provider_api_work_retrieve($name) {
  $return = (object) array('retrieve' => $name);
  return $return;
}

/**
 * 
 */
function _nurani_library_provider_api_work_index() {
  $return = array($page, $parameters, $pagesize);
  return $return;
}

