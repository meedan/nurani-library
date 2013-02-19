function PassageWidget(widget) {
  if (widget) {
    this.init(widget);
  }
}

/**
 * Static method for JSONP driven object instantiation.
 */
PassageWidget.JSONP = function (widget) {
  return new PassageWidget(widget);
};

/**
 * Static method for building an oEmbed URL.
 */
PassageWidget.oEmbedURL = function (osisIDWork, osisID, hash, format, callback) {
  hash     = hash ? '#' + hash : '';
  format   = format ? format : 'jsonp';
  callback = callback ? callback : 'PassageWidget.JSONP';

  var nl    = Drupal.settings.nuraniLibrary,
      query = [
        'url=' + encodeURIComponent(nl.baseUrl + '/passages/' + osisIDWork + '/' + osisID + hash),
        'format=' + encodeURIComponent(format)
      ];

  if (format === 'jsonp') {
    query.push('callback=' + callback);
  }

  return nl.baseUrl + '/oembed/endpoint' + '?' + query.join('&');
};

PassageWidget.prototype.init = function (widget) {
  var matches, id;

  // Annotation the page anchor fragment marker '#' is abused to also be the
  // ID marker for jQuery
  matches = widget.original_url.match(/#passage-(widget|citation)-[a-z0-9]{8,32}$/);

  this.$element = matches[0] ? $(matches[0]) : false;

  if (this.$element) {
    if (widget.html) {
      this.addWidget(widget.html)
    }
    else if (widget.scalar) {
      this.addWidget(widget.scalar)
    }
    this.addWidgetTabBar();
    this.initAnnotations();
  }
};

PassageWidget.prototype.addWidget = function(html) {
  this.$element.html(html);
};

PassageWidget.prototype.initAnnotations = function () {
  var that = this;

  $('span.note', this.$element).each(function () {
    var $note  = $(this),
        $nm    = $('<sup class="note-marker">*</sup>'),
        pos;

    $note.before($nm);

    $nm.hover(function () { that.tooltipDisplayAction($nm, $note); },
              function () { that.tooltipHideAction($nm, $note); });
  });
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

    if (first) {
      $tab.addClass('active');
    } else {
      $this.hide();
    }

    $tab.data('passage-widget', $this);
    $tab.click(function () {
      that.tabSwitchAction(this);
      return false;
    });

    that.$tabBar.append($tab);

    first = false;
  });

};

PassageWidget.prototype.tabSwitchAction = function (tab) {
  $('.passage-widget', this.$element).hide();
  $(tab).data('passage-widget').show();
  $('.button', this.$tabBar).removeClass('active');
  $(tab).addClass('active');
};

PassageWidget.prototype.tooltipDisplayAction = function ($el, $tooltip) {
  var pos = $el.position();
 
  $tooltip.css({
    top:  pos.top - $tooltip.outerHeight() - 10,
    left: pos.left
  })

  $tooltip.show();
};

PassageWidget.prototype.tooltipHideAction = function ($el, $tooltip) {
  $tooltip.hide();
};


/**
 * PassageCitation is a derivative widget type inherited from PassageWidget.
 */
function PassageCitation(widget) {
  // Invoke parent constructor
  PassageWidget.call(this, widget);
}

// Inheritance, PassageCitation < PassageWidget
PassageCitation.prototype = new PassageWidget();

/**
 * Static method for JSONP driven object instantiation.
 */
PassageCitation.JSONP = function (widget) {
  return new PassageCitation(widget);
};

PassageCitation.prototype.init = function (widget) {
  // Invoke parent initialization
  PassageWidget.prototype.init.call(this, widget);

  if (this.$element) {
    this.initTooltip(widget);
  }
};

// Override to disable tab-bar
PassageCitation.prototype.addWidgetTabBar = function () { };

// 
PassageCitation.prototype.initTooltip = function (widget) {
  var that = this,
      $link = $('<a href="#" class="citation-marker">' + widget.title + '</a>'),
      $tooltip = $('.passage-widget', this.$element);

  this.$element.append($link);

  $link.hover(function () { that.tooltipDisplayAction($link, $tooltip); },
              function () { that.tooltipHideAction($link, $tooltip); });
};

