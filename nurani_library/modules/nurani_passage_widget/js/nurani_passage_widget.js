/**
 * The PassageWidget class.
 */
var PassageWidget = (function ($) {

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
      id = matches[0];
      $(id).html(widget.html);
    }
  };

  return PassageWidget;

}(jQuery));
