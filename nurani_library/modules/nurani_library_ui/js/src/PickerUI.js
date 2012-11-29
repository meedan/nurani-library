/**
 * Main controller object for the bundle UI.
 */
function PickerUI(opts) {
  this.defaults = {
    osisIDWork: '',
    osisID:     '',
  };

  this.opts = $.extend(this.defaults, opts);

  if (this.opts.osisID) {
    this.opts.osisIDParts = this.opts.osisID.split('.');
  }

  this.init();

  return this;
}

PickerUI.prototype.init = function () {
  // Contents of div.toolbar and div.passages is populated via handlebars.js templates
  this.$element = $('<div class="passage-picker-ui"><div class="toolbar"></div><div class="alternateWorks"></div><div class="passages"></div></div>');

  // Render the initial templates, empty
  this.templates = {};
  this.viewData = { works: [], passages: [] };
  this.renderViews();

  this.populateWorks(true, true);
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
      that.viewData.selectedWork = util.findByName(that.viewData.works, $(this).val());
      that.setAlternateWorks();
      that.setFilterOptions(['book', 'chapter']);
      that.renderViews(['toolbar', 'alternateWorks']);
      that.populatePassages();
    });
  $('#edit-book-filter', $toolbar)
    .change(function () {
      var i = that.viewData.selectedWork._key;
      that.viewData.selectedBook = util.findByName(that.viewData.works[i].books, $(this).val());
      that.setFilterOptions(['chapter']);
      that.renderViews(['toolbar']);
      that.populatePassages();
    });
  $('#edit-chapter-filter', $toolbar)
    .change(function () {
      var i = that.viewData.selectedWork._key,
          j = that.viewData.selectedBook._key;
      that.viewData.selectedChapter = util.findByName(that.viewData.works[i].books[j].chapters, $(this).val());
      that.populatePassages();
    });
};

/**
 * Initialize the elements in the passages list. This is called dynamically
 * when the passages view is loaded.
 */
PickerUI.prototype.initPassages = function ($passages) {
  var that = this;

  $('.form-item-passage', $passages)
    .click(function () {
      var $this = $(this);

      if ($this.attr('checked')) {
        // Determine the contiguous group this checkbox belongs to then remove
        // all other ticked boxes
        var $checkboxes = $('.form-item-passage', that.$element),
            originI = $checkboxes.index(this),
            first = that.contiguous($checkboxes, originI, -1),
            last = that.contiguous($checkboxes, originI,  1),
            firstVerse = first.split('.')[2],
            lastVerse = last.split('.')[2];

        $checkboxes.each(function () {
          var parts = $(this).val().split('.');

          if (parts[2] < firstVerse || parts[2] > lastVerse) {
            $(this).removeAttr('checked');
          }
        });
      }

      // Display the additional works ribbon
      // that.
    });
};

PickerUI.prototype.renderViews = function (toRender) {
  var func, $el;
  toRender = toRender || ['toolbar', 'alternateWorks', 'passages'];

  for (var i = toRender.length - 1; i >= 0; i--) {
    $el = this.$element.find('.' + toRender[i]);
    // Compile handlebars.js templates as needed
    // @see: PickerUI.templates.js
    if (!this.templates[toRender[i]]) {
      this.templates[toRender[i]] = Handlebars.compile(PickerUI.templates[toRender[i]]);
    }
    $el.html(this.templates[toRender[i]](this.viewData));

    // Call the initializer for this view
    func = 'init' + util.capitalize(toRender[i]);
    if (typeof this[func] === 'function') {
      this[func]($el);
    }
  }
};

PickerUI.prototype.populateWorks = function (andPassages, setDefaultState) {
  var that = this;

  $.ajax({
    url: Drupal.settings.nuraniLibrary.baseAPIUrl + '/work?format=jsonp&callback=?',
    dataType: 'jsonp',
    success: function (data) {
      that.viewData.works = that.unpackWorkData(data);

      that.setFilterOptions(['work', 'book', 'chapter'], setDefaultState ? ['work', 'book', 'chapter'] : null);

      that.renderViews(['toolbar', 'alternateWorks']);

      if (andPassages) {
        that.populatePassages(setDefaultState);
      }
    }
  });
};

PickerUI.prototype.populatePassages = function (setDefaultState) {
  if (!this.viewData.page) {
    this.viewData.page = 0;
  }

  var that  = this,
      query = {
        work_name: this.viewData.selectedWork.name,
        book:      this.viewData.selectedBook.name,
        chapter:   this.viewData.selectedChapter.name,
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
          parts, firstVerse, lastVerse, passage;

      if (setDefaultState && that.opts.osisID && that.opts.osisIDParts[2]) {
        parts = that.opts.osisIDParts[2].split('-');
        firstVerse = parts[0];
        lastVerse = parts.length == 2 ? parts[1] : parts[0];
      } else {
        setDefaultState = false;
      }

      for (i = 0; i < len; i++) {
        passage = data[i];
        passage.osisID    = [passage.book_name, passage.chapter_name, passage.verse].join('.');
        passage.cssId    = 'edit-passage-' + passage.osisID.replace(/\./g, '-');
        passage.verseUrl = Drupal.settings.nuraniLibrary.baseUrl + '/passages/' + passage.work_name + '/' + passage.osisID;

        if (setDefaultState && passage.verse >= firstVerse && passage.verse <= lastVerse) {
          passage.selected = true;
        }

        data[i] = passage;
      }

      that.viewData.passages = data;
      that.renderViews(['passages']);

      that.$element.animate({ scrollTop: 0 });
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
 * @param "object" toClear
 *  Array having values: 'work', 'book', or 'chapter'.
 */
PickerUI.prototype.setFilterOptions = function (toClear, toDefault) {
  var i, j, len, type, objects, defaultsMap;

  toClear   = toClear   || [];
  toDefault = toDefault || [];

  for (i = toClear.length - 1; i >= 0; i--) {
    this.viewData['selected' + util.capitalize(toClear[i])] = false;
  }

  // Only set defaults if:
  // (a) There are defaults to set and
  // (b) It was requested (ie: if toDefault empty the loop won't run)
  if (this.opts.osisIDWork && this.opts.osisID) {
    defaultsMap = {
      work:    this.opts.osisIDWork,
      book:    this.opts.osisIDParts[0],
      chapter: this.opts.osisIDParts[1]
    };

    for (i = 0, len = toDefault.length; i < len; i++) {
      type = toDefault[i];

      switch (type) {
        case 'work':    objects = this.viewData.works; break;
        case 'book':    objects = this.viewData.selectedWork.books; break;
        case 'chapter': objects = this.viewData.selectedBook.chapters; break;
      }

      for (j = objects.length - 1; j >= 0; j--) {
        if (objects[j].name == defaultsMap[type]) {
          this.viewData['selected' + util.capitalize(type)] = objects[j];
          this.viewData['selected' + util.capitalize(type)]._key = j;
        }
      }
    }
  }

  if (!this.viewData.selectedWork) {
    this.viewData.selectedWork = this.viewData.works[0];
    this.viewData.selectedWork._key = 0;
  }
  if (!this.viewData.selectedBook) {
    this.viewData.selectedBook = this.viewData.selectedWork.books[0];
    this.viewData.selectedBook._key = 0;
  }
  if (!this.viewData.selectedChapter) {
    this.viewData.selectedChapter = this.viewData.selectedBook.chapters[0];
    this.viewData.selectedChapter._key = 0;
  }

  this.setAlternateWorks();
}

PickerUI.prototype.setAlternateWorks = function (originWork) {
  this.viewData.alternateWorks = this.viewData.works;
};

PickerUI.prototype.getSelectionOSIS = function ($origin) {
  $origin = $origin || $('.form-item-passage[checked]:first', this.$element);

  var first = $origin.val(),
      last = $origin.val(),
      $checkboxes = $('.form-item-passage', this.$element),
      length = $checkboxes.length,
      originI = $checkboxes.index($origin),
      firstParts, lastParts, osisID;

  if (originI >= 0) {
    // Iterate backwards and forwards to find boundaries of this contiguously
    // checked group.
    first = this.contiguous($checkboxes, originI, -1);
    last  = this.contiguous($checkboxes, originI,  1);

    firstParts = first.split('.');
    lastParts = last.split('.');

    osisID = [];
    osisID.push(firstParts[0]);
    osisID.push(firstParts[1]);

    if (firstParts[2] != lastParts[2]) {
      osisID.push(firstParts[2] + '-' + lastParts[2]);
    } else {
      osisID.push(firstParts[2]);
    }

    return {
      osisIDWork: $('#edit-work-filter').val(),
      osisID: osisID.join('.')
    }
  }

  // FIXME: Errors should be managed by the PickerUI object.
  // for (var key in data.errors) {
  //   if (data.errors.hasOwnProperty(key)) {
  //     util.setMessage(that.$dialog, data.errors[key], 'error');
  //   }
  // }

  return false;
};

PickerUI.prototype.contiguous = function ($checkboxes, i, dir, bound) {
  bound = $checkboxes[i].value;

  // Failsafe
  if (dir != -1 && dir != 1) {
    return '';
  }

  if (i + dir >= 0 && i + dir < $checkboxes.length && $checkboxes[i + dir].checked) {
    return this.contiguous($checkboxes, i + dir, dir, bound);
  } else {
    return bound;
  }
};

PickerUI.prototype.didResize = function () {
  this.$element.find('.toolbar,.alternateWorks').css('width', this.$element.width());
};

