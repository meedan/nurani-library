<?php

require_once 'NuraniModel.php';


/**
 * NuraniRESTModel
 */
class NuraniRESTModel extends NuraniModel {

  public function __construct($connection) {
    parent::__construct($connection);
  }


  /**
   * Performs a search against a remote Nurani Library Provider instance.
   */
  public function search($work_name, $book = NULL, $chapter = NULL, $verse = NULL, $page = 0, $pagesize = 100) {
    $query = array();

    foreach (array('work_name', 'book', 'chapter', 'verse', 'page', 'pagesize') as $variable) {
      // Beware the $$ notation..
      if (!is_null($$variable)) {
        $query[$variable] = $$variable;
      }
    }

    $response = $this->restRequest('GET', 'passage' . $this->_queryString($query));
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
    $response = $this->restRequest('GET', 'work');
    return $response;
  }


  /**
   * Fetches a specific work from a remote Nurani Library Provider instance.
   */
  public function getWork($work_name) {
    $response = $this->restRequest('GET', 'work/' . urlencode($work_name));
    return $response;
  }


  public function deleteWork($work_id) {
    return FALSE;
  }


  /**
   * Sends a REST request using information provided in $this->connection.
   * 
   * @param (string) $method
   *  The request method, one of: GET, POST, PUT or DELETE
   * @param (string) $resource
   *  The resource to fetch, ie: 'passage/wlc'
   * @param (array) $headers
   *  Optional, any extra HTTP headers.  The basic ones are there already.
   * @param (mixed) $form_data
   *  Optional, the POST/PUT data payload. Array or string are fine.
   * @param (int) $retry
   *  Optional, passed to drupal_http_request() as $retry param
   * @param (float) $timeout
   *  Optional, passed to drupal_http_request() as $timeout param
   */
  private function restRequest($method, $resource, $headers = array(), $form_data = NULL, $retry = 3, $timeout = 30.0) {
    $path = $this->connection['path'] ? $this->connection['path'] . '/' : '';

    $url  = $this->connection['scheme'] . '://' . $this->connection['host'];
    $url .= ':' . $this->connection['port'];
    $url .= '/' . $path . $resource;

    $headers['accept'] = 'application/json';
    if (in_array($method, array('POST', 'PUT'))) {
      $headers['content-type'] = 'application/x-www-form-urlencoded';
    }

    // TODO: Need to authenticate with the server in some clean way, OAuth?
    // $headers['cookie'] = session_name() . '=' . session_id()

    $data = NULL;
    if (is_array($form_data)) {
      $data = http_build_query($form_data, '', '&');
    }
    else if (is_string($form_data) && strlen($form_data) > 0) {
      $data = $form_data;
    }

    if ($this->connection['debug']) {
      watchdog('NuraniRESTModel', "URL, headers, method, data, retry timeout: <pre>!info</pre>", array('!info' => var_export(array($url, $headers, $method, $data, $retry, $timeout), 1)));
    }

    $response = drupal_http_request($url, $headers, $method, $data, $retry, $timeout);

    if ($this->connection['debug']) {
      watchdog('NuraniRESTModel', "Response: <pre>!response</pre>", array('!response' => var_export($response, 1)));
    }

    return $response;
  }


  /**
   * Builds a query string from an array. Includes the leading question mark '?'.
   */
  private function _queryString($query) {
    return (is_array($query) && !empty($query))
           ? '?' . http_build_query($query, '', '&')
           : '';
  }

}
