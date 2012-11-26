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
  this.renderViews();

  this.populateWorks(true);
};

/**
 * Initialize the elements in the toolbar. This is called dynamically when the
 * toolbar view is loaded.
 */
PickerUI.prototype.initToolbar = function ($toolbar) {
  var that = this;

  $('#edit-search-submit', $toolbar)
    .click(function () {
      log($('#edit-search', $toolbar).val(), 'TODO: Search in passage box');
      return false;
    });

  $('#edit-work-filter', $toolbar)
    .change(function () {
      that.viewData.selected_work = util.findByName(that.viewData.works, $(this).val());
      that.setFilterOptions(['selected_book', 'selected_chapter']);
      that.renderViews(['toolbar']);
      that.populatePassages();
    });
  $('#edit-book-filter', $toolbar)
    .change(function () {
      var i = that.viewData.selected_work._key;
      that.viewData.selected_book = util.findByName(that.viewData.works[i].books, $(this).val());
      that.setFilterOptions(['selected_chapter']);
      that.renderViews(['toolbar']);
      that.populatePassages();
    });
  $('#edit-chapter-filter', $toolbar)
    .change(function () {
      var i = that.viewData.selected_work._key,
          j = that.viewData.selected_book._key;
      that.viewData.selected_chapter = util.findByName(that.viewData.works[i].books[j].chapters, $(this).val());
      that.populatePassages();
    });
};

/**
 * Initialize the elements in the passages list. This is called dynamically
 * when the passages view is loaded.
 */
PickerUI.prototype.initPassages = function ($passages) {

};

PickerUI.prototype.renderViews = function (to_render) {
  var func, $el;
  to_render = to_render || ['toolbar', 'passages'];

  for (var i = to_render.length - 1; i >= 0; i--) {
    $el = this.$element.find('.' + to_render[i]);
    // Compile handlebars.js templates as needed
    // @see: PickerUI.templates.js
    if (!this.templates[to_render[i]]) {
      this.templates[to_render[i]] = Handlebars.compile(PickerUI.templates[to_render[i]]);
    }
    $el.html(this.templates[to_render[i]](this.viewData));

    // Call the initializer for this view
    func = 'init' + util.capitalize(to_render[i]);
    if (typeof this[func] === 'function') {
      this[func]($el);
    }
  }
};

PickerUI.prototype.populateWorks = function (and_passages) {
  var that = this;

  $.ajax({
    url: Drupal.settings.nuraniLibrary.baseAPIUrl + '/work?format=jsonp&callback=?',
    dataType: 'jsonp',
    success: function (data) {
      that.viewData.works = that.unpackWorkData(data);

      that.setFilterOptions(['selected_work', 'selected_book', 'selected_chapter']);
      that.renderViews(['toolbar']);

      if (and_passages) {
        that.populatePassages();
      }
    }
  });
};

PickerUI.prototype.populatePassages = function () {
  if (!this.viewData.page) {
    this.viewData.page = 0;
  }

  var that  = this,
      query = {
        work_name: this.viewData.selected_work.name,
        book:      this.viewData.selected_book.name,
        chapter:   this.viewData.selected_chapter.name,
        page:      this.viewData.page,
        pagesize:  100,
        format:    'jsonp',
        callback:  '?'
      };

  $.ajax({
    // TODO: Select the chosen work and pass it here.
    // TODO: Have filters for book / chapter.
    // TODO: Pagination??
    url: Drupal.settings.nuraniLibrary.baseAPIUrl + '/passage?' + $.param(query),
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
      that.renderViews(['passages']);
    }
  });
};

PickerUI.prototype.unpackWorkData = function (data) {
  var i,  j,  k,
      books, chapters,
      range;

  for (i = data.length - 1; i >=0; i--) {
    books = data[i].books;

    for (j = books.length - 1; j >= 0; j--) {
      chapters = data[i].books[j].chapters;

      // Unpack the chapters: 'START-END' format
      if (typeof chapters === 'string') {
        range = chapters.split('-');

        chapters = [];
        for (k = range[0]; k <= range[1]; k++) {
          chapters.push({
            name: k,
            full_name: k
          });
        }
      }
      // Unpack the duplicate chapter name format, eg: chapters: [1, 2, 3]
      else if ($.isArray(chapters)) {
        for (k = chapters.length - 1; k >= 0; k--) {
          if (typeof chapters[k] !== 'object') { // Find 'number' and 'string'
            chapter[k] = {
              name: chapter[k],
              full_name: chapter[k]
            };
          }
        }
      }

      data[i].books[j].chapters = chapters;
    }
  }

  return data;
};

/**
 * Sets the options available for a filter.
 *
 * @param "object" type
 *  Array having values: 'work', 'book', or 'chapter'.
 */
PickerUI.prototype.setFilterOptions = function (to_clear) {
  var i, len = to_clear.length;

  for (i = 0; i < len; i++) {
    this.viewData[to_clear[i]] = false;
  }

  if (!this.viewData.selected_work) {
    this.viewData.selected_work = this.viewData.works[0];
    this.viewData.selected_work._key = 0;
  }
  if (!this.viewData.selected_book) {
    this.viewData.selected_book = this.viewData.selected_work.books[0];
    this.viewData.selected_book._key = 0;
  }
  if (!this.viewData.selected_chapter) {
    this.viewData.selected_chapter = this.viewData.selected_book.chapters[0];
    this.viewData.selected_chapter._key = 0;
  }
}

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

