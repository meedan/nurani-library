/**
 * Corebox util library.
 */
function Util() {
}

// Globally available CB.Util
var util = new Util();

// paulirish.com/2009/log-a-lightweight-wrapper-for-consolelog/
var log = function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};


/**
 * Capitalizes the first letter of a string.
 * @see: http://stackoverflow.com/a/1026087/806988
 */
Util.prototype.capitalize = function (string) {
  return string.charAt(0).toUpperCase() + string.slice(1);
};

Util.prototype.findByName = function (array, name) {
  var search, key;
  search = $.grep(array, function (o, i) { return o.name == name; });
  return search.length > 0 ? $.extend({ _key: array.indexOf(search[0]) }, search[0]) : false;
};