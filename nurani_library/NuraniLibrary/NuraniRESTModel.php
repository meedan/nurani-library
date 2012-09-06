<?php

require_once 'NuraniModel.php';


/**
 * NuraniRESTModel
 */
class NuraniRESTModel extends NuraniModel {

  public function __construct($connection) {
    parent::__construct($connection);
    $this->establishConnection();
  }


  /**
   * Performs a search against a remote Nurani Library Provider instance.
   */
  public function search($work_name, $book = NULL, $chapter = NULL, $verse = NULL, $page = 0, $pagesize = 100) {
    $this->resetErrorState();

    $query = array();

    foreach (array('work_name', 'book', 'chapter', 'verse', 'page', 'pagesize') as $variable) {
      // Beware the $$ notation..
      if (!is_null($$variable)) {
        $query[$variable] = $$variable;
      }
    }

    $passages = $this->restRequest('GET', 'passage' . $this->queryString($query));
    if (is_array($passages)) {
      foreach ($passages as &$passage) {
        $passage = (object) $passage;
      }
    }

    return $passages;
  }


  /**
   * Import exists to fulfil the abstract class spec; however, importing into
   * remote Nurani Library Provider instances is not allowed at this time.
   */
  public function import($work, $document) {
    $this->resetErrorState();
    return $this->error("Importing into remote Nurani Library Provider instances is not allowed at this time.", 0);
  }


  /**
   * Fetches all works in a remote Nurani Library Provider instance.
   */
  public function getWorks() {
    $this->resetErrorState();

    $works = $this->restRequest('GET', 'work');
    if (is_array($works)) {
      foreach ($works as &$work) {
        $work = (object) $work;
      }
    }

    return $works;
  }


  /**
   * Fetches a specific work from a remote Nurani Library Provider instance.
   */
  public function getWork($work_name) {
    $this->resetErrorState();

    $work = $this->restRequest('GET', 'work/' . urlencode($work_name));
    if (is_array($work)) {
      $work = (object) $work;
    }

    return $work;
  }


  public function deleteWork($work_id) {
    $this->resetErrorState();
    return $this->error("Deleting from remote Nurani Library Provider instances is not allowed at this time.", 0);
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
   * @param (int) $max_redirects
   *  Optional, passed to drupal_http_request() as $max_redirects param
   * @param (float) $timeout
   *  Optional, passed to drupal_http_request() as $timeout param
   */
  private function restRequest($method, $resource, $form_data = NULL, $headers = array(), $max_redirects = 3, $timeout = 30.0) {
    $path = $this->connection['path'] ? $this->connection['path'] . '/' : '';

    $url  = $this->connection['scheme'] . '://';
    $url .= ($this->connection['auth_user']) ? $this->connection['auth_user'] . ($this->connection['auth_pass'] ? ':' . $this->connection['auth_pass'] . '' : '') . '@' : '';
    $url .= $this->connection['host'];
    $url .= ':' . $this->connection['port'];
    $url .= '/' . $path . $resource;

    $headers['accept'] = 'application/json';
    if (in_array($method, array('POST', 'PUT'))) {
      $headers['content-type'] = 'application/x-www-form-urlencoded';
    }

    // Use the session, if one is set
    if (isset($this->session['id']) && isset($this->session['name'])) {
      $headers['cookie'] = $this->session['name'] . '=' . $this->session['id'];
    }

    $data = NULL;
    if (is_array($form_data)) {
      $data = http_build_query($form_data, '', '&');
    }
    else if (is_string($form_data) && strlen($form_data) > 0) {
      $data = $form_data;
    }

    if ($this->connection['debug']) {
      watchdog('NuraniRESTModel', "URL, headers, method, data, max_redirects timeout: <pre>!info</pre>", array('!info' => var_export(array($url, $headers, $method, $data, $max_redirects, $timeout), 1)));
    }

    $response = drupal_http_request($url, array(
      'headers' => $headers,
      'method' => $method,
      'data' => $data,
      'max_redirects' => $max_redirects,
      'timeout' => $timeout,
    ));

    if ($this->connection['debug']) {
      watchdog('NuraniRESTModel', "Response: <pre>!response</pre>", array('!response' => var_export($response, 1)));
    }

    if ($response->code != 200) {
      $message = "Error: " . $response->status_message;
      if ($response->data) {
        $message .= "; Data: " . $response->data;
      }
      return $this->error($message, $response->code);
    }

    $data = drupal_json_decode($response->data);
    if (is_null($data)) {
      return $this->error("Null response or unable to decode JSON", 1, array('data' => $response->data));
    }
    else {
      return $data;
    }
  }


  /**
   * Builds a query string from an array. Includes the leading question mark '?'.
   */
  private function queryString($query) {
    return (is_array($query) && !empty($query))
           ? '?' . http_build_query($query, '', '&')
           : '';
  }


  /**
   * Makes a connection with the 
   */
  private function establishConnection() {
    // Attempt to bind to the existing session
    if (isset($_SESSION['nurani_rest_session']) && isset($_SESSION['nurani_rest_session']['id']) && isset($_SESSION['nurani_rest_session']['name'])) {
      $this->session = $_SESSION['nurani_rest_session'];
      return TRUE;
    }

    // No session, attempt to get a new one. Ensure any partial connections are
    // cleaned up first.
    $this->destroyConnection();

    $login = $this->restRequest('POST', 'user/login', array('username' => $this->connection['user'], 'password' => $this->connection['pass']));
    if ($this->getError() || !$login['sessid'] || !$login['session_name']) {
      $this->destroyConnection();
      return FALSE;
    }

    // Store the session so the next $this->restRequest() can use it
    $this->session = array(
      'id' => $login['sessid'],
      'name' => $login['session_name'],
    );
    $_SESSION['nurani_rest_session'] = $this->session;

    return TRUE;
  }


  /**
   * Removes variables associated with a NuraniRESTModel session.
   */
  private function destroyConnection() {
    unset($_SESSION['nurani_rest_session'], $this->session);
  }

}
