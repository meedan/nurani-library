<?php

require_once 'NuraniModel.php';


/**
 * NuraniRESTModel
 */
class NuraniRESTModel extends NuraniModel {

  public function __construct($connection) {
    parent::__construct($connection);

    if (!isset($this->connection['httpversion'])) {
      $this->connection['httpversion'] = '1.1';
    }
  }


  /**
   * Performs a search against a remote Nurani Library Provider instance.
   */
  public function search($work, $book = NULL, $chapter = NULL, $verse = NULL, $page = 0, $pagesize = 100) {
    $query = array();

    foreach (array('book', 'chapter', 'verse', 'page', 'pagesize') as $variable) {
      // Beware the $$ notation..
      if (!is_null($$variable)) {
        $query[$variable] = $$$variable;
      }
    }

    $response = $this->restRequest(array(
      'method' => 'GET',
      'resource' => 'passage/' . $work . $this->_queryString($query),
    ));
    return $response;
  }


  /**
   * Import exists to fulfil the abstract class spec; however, importing into
   * remote Nurani Library Provider instances is not allowed at this time.
   */
  public function import($work, $document) {
    return FALSE;
  }


  /**
   * Fetches all works in a remote Nurani Library Provider instance.
   */
  public function getWorks() {
    $response = $this->restRequest(array(
      'method' => 'GET',
      'resource' => 'work',
    ));
    return $response;
  }


  /**
   * Fetches a specific work from a remote Nurani Library Provider instance.
   */
  public function getWork($work_name) {
    return 1;
  }


  public function deleteWork($work_id) {
    return FALSE;
  }

  /**
   * Sends a REST request using information provided in $this->connection.
   * 
   * @param (array) $request
   *  The request, as per the spec in rest_client module's README.txt. At
   *  minimum the 'resource' and 'method' are required.
   * @param (array) $headers
   *  Optional, any extra HTTP headers.  The basic ones are there already.
   * @param ()
   */
  private function restRequest($request, $headers = array(), $form_data = NULL, $retry = 3) {
    $path = $this->connection['path'] ? $this->connection['path'] . '/' : '';

    $request['resource']    = '/' . $path . $request['resource'];
    $request['port']        = $this->connection['port'];
    $request['httpversion'] = $this->connection['httpversion'];
    $request['scheme']      = $this->connection['scheme'];

    $headers['host']   = $this->connection['host'];
    $headers['accept'] = 'application/json';
    if (in_array($request['method'], array('POST', 'PUT'))) {
      $headers['content-type'] = 'application/x-www-form-urlencoded';
    }

    $data = NULL;
    if (is_array($form_data)) {
      $data = array('type' => 'string', 'value' => http_build_query($form_data, '', '&'));
    }
    else if (is_string($form_data) && strlen($form_data) > 0) {
      $data = array('type' => 'string', 'value' => $form_data);
    }

    watchdog('NuraniRESTModel', "Request, headers, data, retry: <pre>!info</pre>", array('!info' => var_export(array($request, $headers, $data, $retry), 1)));

    $response = rest_client_request($request = array(), $headers = array(), $data = NULL, $retry = 3);

    watchdog('NuraniRESTModel', "Response: <pre>!response</pre>", array('!response' => var_export($response, 1)));

    return $response;
  }

  private function _queryString($query) {
    return (is_array($query) && !empty($query))
           ? '?' . http_build_query($query, '', '&')
           : '';
  }

}
