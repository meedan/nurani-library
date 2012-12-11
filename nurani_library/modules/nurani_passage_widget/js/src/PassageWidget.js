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

  this.$element = matches[0] ? $(matches[0]) : false;

  if (this.$element) {
    this.addWidget(widget.html)
    this.addWidgetTabBar();
  }
};

PassageWidget.prototype.addWidget = function(html) {
  this.$element.html(html);
};

PassageWidget.prototype.addWidgetTabBar = function() {
  var that = this,
      $passageWidgets = this.$element.find('> .passage-widget'),
      first = true;

  if ($passageWidgets.length <= 1) {
    return;
  }

  this.$tabBar = $('<div class="tab-bar"></div>');

  this.$element
    .parent()
    .append(this.$tabBar);

  $passageWidgets.each(function () {
    var $this = $(this),
        title = $this.find('h4').text(),
        $tab  = $('<a href="#" class="button">' + title + '</a>');

    if (!first) {
      $this.hide();
    }

    $tab.data('passage-widget', $this);
    $tab.click(function () { that.tabSwitchAction(this); });

    that.$tabBar.append($tab);

    first = false;
  });

};

PassageWidget.prototype.tabSwitchAction = function (tab) {
  $('.passage-widget', this.$element).hide();
  $(tab).data('passage-widget').show();
};
