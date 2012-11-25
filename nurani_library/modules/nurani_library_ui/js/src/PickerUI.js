/**
 * Main controller object for the bundle UI.
 */
function PickerUI(opts) {
  this.defaults = {
    osisIDWork: '',
    osisID:     '',
  };

  this.opts = $.extend(this.defaults, opts);
  this.init();

  return this;
}

PickerUI.prototype.init = function () {
  // Contents of div.toolbar and div.passages is populated via handlebars.js templates
  this.$element = $('<div class="passage-picker-ui"><div class="toolbar"></div><div class="passages"></div></div>');

  // Render the initial templates, empty
  this.templates = {};
  this.viewData = { works: [], passages: [] };
  this.render();

  this.populateWorks(true);
};

PickerUI.prototype.render = function (to_render) {
  to_render = to_render || ['toolbar', 'passages'];

  for (var i = to_render.length - 1; i >= 0; i--) {
    // Compile handlebars.js templates as needed
    // @see: PickerUI.templates.js
    if (!this.templates[to_render[i]]) {
      this.templates[to_render[i]] = Handlebars.compile(PickerUI.templates[to_render[i]]);
    }
    this.$element.find('.' + to_render[i]).html(this.templates[to_render[i]](this.viewData));
  }
};

PickerUI.prototype.populateWorks = function (and_passages) {
  var that = this;

  $.ajax({
    url: Drupal.settings.nuraniLibrary.baseAPIUrl + '/work?format=jsonp&callback=?',
    dataType: 'jsonp',
    success: function (data) {
      that.viewData.works = data;
      that.render(['toolbar']);

      if (and_passages) {
        that.populatePassages();
      }
    }
  });
};

PickerUI.prototype.populatePassages = function () {
  var that = this;

  $.ajax({
    // TODO: Select the chosen work and pass it here.
    // TODO: Have filters for book / chapter.
    // TODO: Pagination??
    url: Drupal.settings.nuraniLibrary.baseAPIUrl + '/passage?work_name=wlc_he&book=&chapter=&page=&pagesize=100&format=jsonp&callback=?',
    dataType: 'jsonp',
    success: function (data) {
      var i = 0,
          len = data.length,
          passage;

      for (i = 0; i < len; i++) {
        passage = data[i];
        passage.osisID    = [passage.book_name, passage.chapter_name, passage.verse].join('.');
        passage.css_id    = 'edit-passage-' + passage.osisID.replace(/\./g, '-');
        passage.verse_url = Drupal.settings.nuraniLibrary.baseUrl + '/passages/' + passage.work_name + '/' + passage.osisID;

        data[i] = passage;
      }

      that.viewData.passages = data;
      that.render(['passages']);
    }
  });
};

PickerUI.prototype.getSelectionOSIS = function () {
  // FIXME: Errors should be managed by the PickerUI object.
  // for (var key in data.errors) {
  //   if (data.errors.hasOwnProperty(key)) {
  //     util.setMessage(that.$dialog, data.errors[key], 'error');
  //   }
  // }

  // TODO: Un-stub this.
  return {
    osisIDWork: 'wlc_he',
    osisID: 'Gen.1.1'
  };
};

PickerUI.prototype.didResize = function () {
  this.$element.find('.toolbar').css('width', this.$element.width());
};

