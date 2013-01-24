function PassageWidget(widget) {
  this.init(widget);
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

  // Note the page anchor fragment marker '#' is abused to also be the
  // ID marker for jQuery
  matches = widget.original_url.match(/#passage-widget-[a-z0-9]{8,32}$/);

  this.$element = matches[0] ? $(matches[0]) : false;

  if (this.$element) {
    if (widget.html) {
      this.addWidget(widget.html)
    }
    this.addWidgetTabBar();
  }
};

PassageWidget.prototype.addWidget = function(html) {
  this.$element.html(html);
  this.initNotes();
};

PassageWidget.prototype.initNotes = function () {
  var that = this;

  $('span.note', this.$element).each(function () {
    var $note  = $(this),
        $nm    = $('<sup class="note-marker">*</sup>'),
        pos    = $note.position();

    $note.before($nm);

    $note.css({
      top:      pos.top - $note.outerHeight(),
      left:     pos.left
    })
    $note.hide();

    $nm.hover(function () { that.noteDisplayAction($note); },
              function () { that.noteHideAction($note); });
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

PassageWidget.prototype.noteDisplayAction = function ($note) {
  $note.show();
};

PassageWidget.prototype.noteHideAction = function ($note) {
  $note.hide();
};

