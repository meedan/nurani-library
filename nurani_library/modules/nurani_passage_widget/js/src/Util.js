/**
 * Util library.
 */
function Util() {
}

// Globally available Util
var util = new Util();

// paulirish.com/2009/log-a-lightweight-wrapper-for-consolelog/
var log = function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};
