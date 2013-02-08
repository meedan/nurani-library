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
  public function search($work_name, $book = NULL, $chapter = NULL, $verse = NULL, $authorUUID = NULL, $page = 0, $pagesize = 100) {
    $this->resetErrorState();

    $query = array('work_name' => $work_name);

    foreach (array('book', 'chapter', 'verse', 'authorUUID', 'page', 'pagesize') as $variable) {
      // Beware the $$ notation..
      if (!is_null($$variable)) {
        $query[$variable] = $$variable;
      }
    }

    $passages = $this->restRequest('GET', 'passage' . $this->queryString($query));
    if (is_array($passages)) {
      foreach ($passages as &$passage) {
        $passage = (object) $passage;

        if (isset($passage->notes)) {
          $this->addNoteAuthors($passage->notes);
        }
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
   * Retrieves a list of annotations for a passage from remote Nurani Library
   * Provider instance.
   */
  public function getAnnotations($passage_id, $authorUUID = NULL, $type = NULL, $page = 0, $pagesize = 100) {
    $this->resetErrorState();
    $query = array('authorUUID' => $authorUUID, 'type' => $type, 'page' => $page, 'pagesize' => $pagesize);
    $path = 'annotation';
    if (is_numeric($passage_id) && $passage_id > 0) {
      $path .= '/' . $passage_id;
    }
    $annotations = $this->restRequest('GET', $path . $this->queryString($query));
    $this->addNoteAuthors($annotations);
    return $annotations;
  }


  /**
   * Retrieves an annotation from remote Nurani Library Provider instance.
   */
  public function getAnnotation($id) {
    $this->resetErrorState();
    $annotation = $this->restRequest('GET', 'annotation/' . (int) $id);
    $this->addNoteAuthor($annotation);
    return $annotation;
  }


  /**
   * Create an annotation on a Nurani Library Provider instance.
   */
  public function createAnnotation($annotation) {
    $this->resetErrorState();
    $annotation = $this->restRequest('POST', 'annotation', (array) $annotation);
    $this->addNoteAuthor($annotation);
    return $annotation;
  }


  /**
   * Updates an annotation on a Nurani Library Provider instance.
   */
  public function updateAnnotation($id, $annotation) {
    $this->resetErrorState();
    $annotation = $this->restRequest('PUT', 'annotation/' . $id, (array) $annotation);
    $this->addNoteAuthor($annotation);
    return $annotation;
  }


  /**
   * Deletes an annotation from a Nurani Library Provider instance.
   */
  public function deleteAnnotation($id) {
    $this->resetErrorState();
    $result = $this->restRequest('DELETE', 'annotation/' . $id);
    return $result;
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


  // TODO: Maybe this should be moved into nurani_library_services? It's highly Drupal'y
  protected function addNoteAuthors(&$notes) {
    global $user;

    foreach ($notes as $i => $note) {
      $note = (array) $note;

      if (!$note['author_uuid']) {
        continue;
      }

      $author = db_select('users', 'u')
                  ->fields('u', array('uid', 'name'))
                  ->condition('u.uuid', $note['author_uuid'])
                  ->execute()
                  ->fetchAssoc();

      if (!$author) {
        continue;
      }

      $notes[$i]['author'] = array(
        'uid' => $author['uid'],
        'name' => $author['name'],
        'url' => url('user/' . $author['uid'], array('absolute' => TRUE)),
      );

      if ($note['author_uuid'] == $user->uuid || user_access('edit all nurani library annotations')) {
        $notes[$i]['editable'] = TRUE;
      }
    }
  }


  protected function addNoteAuthor(&$note) {
    $notes = array($note);
    $this->addNoteAuthors($notes);
    $note = $notes[0];
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
  private function restRequest($method, $resource, $form_data = NULL, $headers = array(), $max_redirects = 3, $timeout = 30.0, $max_retries = 1) {;
    $path = $this->connection['path'] ? $this->connection['path'] . '/' : '';

    $url  = $this->connection['scheme'] . '://';
    $url .= ($this->connection['auth_user']) ? $this->connection['auth_user'] . ($this->connection['auth_pass'] ? ':' . $this->connection['auth_pass'] . '' : '') . '@' : '';
    $url .= $this->connection['host'];
    $url .= $this->connection['port'] != 80 ? ':' . $this->connection['port'] : '';
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
      if ($max_retries > 0 && !$this->testConnection()) {
        $this->destroyConnection();

        if ($this->establishConnection(0)) {
          $this->resetErrorState();
          return $this->restRequest($method, $resource, $form_data, $headers, $max_redirects, $timeout, $max_retries - 1);
        }
      }

      $message = "Error: ";
      if (isset($response->status_message)) {
        $message .= $response->status_message;
      }
      else if (isset($response->error)) {
        $message .= $response->error;
      }
      if (isset($response->data)) {
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
  private function establishConnection($max_retries = 3) {
    // Attempt to bind to the existing session
    if (isset($_SESSION['nurani_rest_session']) && isset($_SESSION['nurani_rest_session']['id']) && isset($_SESSION['nurani_rest_session']['name'])) {
      $this->session = $_SESSION['nurani_rest_session'];
      return TRUE;
    }

    // No session, attempt to get a new one. Ensure any partial connections are
    // cleaned up first.
    $this->destroyConnection();

    $login = $this->restRequest('POST', 'user/login', array('username' => $this->connection['user'], 'password' => $this->connection['pass']), array(), 3, 30.0, $max_retries);
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
   * Checks the status of the connection by ensuring the session is valid.
   */
  private function testConnection() {
    $this->resetErrorState();

    // Connection request is only sent once, never retried to avoid getting
    // stuck in a loop.
    $response = $this->restRequest('POST', 'system/connect', NULL, array(), 3, 30.0, 0);
    if (is_array($response) && is_array($response['user'])) {
      return $response['user']['uid'] > 0;
    }
    return FALSE;
  }


  /**
   * Removes variables associated with a NuraniRESTModel session.
   */
  private function destroyConnection() {
    unset($_SESSION['nurani_rest_session'], $this->session);
  }

}
