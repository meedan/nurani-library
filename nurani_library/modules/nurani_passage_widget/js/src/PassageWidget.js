function PassageWidget(widget) {
  this.init(widget);
}

/**
 * Static method for JSONP driven object instantiation.
 */
PassageWidget.JSONP = function (widget) {
  return new PassageWidget(widget);
};

PassageWidget.prototype.init = function (widget) {
  var matches, id;

  // Note the page anchor fragment marker '#' is abused to also be the
  // ID marker for jQuery
  matches = widget.original_url.match(/#passage-widget-[a-z0-9]{8,32}$/);
  if (matches[0]) {
    this.addWidget(matches[0], widget.html)
  }
};

PassageWidget.prototype.addWidget = function(selector, html) {
  $(selector).html(html);
};
