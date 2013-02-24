/**
 * Util library.
 */
function Util() {
}

// Globally available Util
var util = new Util();

/**
 * Capitalizes the first letter of a string.
 * @see: http://stackoverflow.com/a/1026087/806988
 */
Util.prototype.capitalize = function (string) {
  return string.charAt(0).toUpperCase() + string.slice(1);
};

Util.prototype.findByName = function (array, name) {
  var search, key;
  // The `o.name == name` match is important here, if `o.name === name` were
  // used there are some cases where chapters may not be matched because of
  // integer vs string mismatch.
  search = $.grep(array, function (o, i) { return o.name == name; });
  return search.length > 0 ? $.extend({ _key: array.indexOf(search[0]) }, search[0]) : false;
};

/**
 * Helper method, set informational messages which disappear after a set amount
 * of time.
 */
Util.prototype.setMessage = function (prepend_to, message, type, hide_after) {
  var classes, $message;

  type       = type || 'ok';
  hide_after = hide_after || 4000;

  classes = ['messages'];
  if (type) {
    classes.push(type);
  }

  $message = $('<div class="' + classes.join(' ') + '" style="display: none;">' + message + '</div>');
  prepend_to.prepend($message);
  $message.slideDown();

  setTimeout(function () {
    $message.slideUp(function () {
      $(this).remove();
    });
  }, hide_after);
};
