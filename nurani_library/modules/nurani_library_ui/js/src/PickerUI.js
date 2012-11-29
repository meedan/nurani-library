/**
 * Main controller object for the bundle UI.
 */
function PickerUI(opts) {
  this.defaults = {
    osisIDWork: '',
    osisID:     '',
  };

  this.opts = $.extend(this.defaults, opts);

  // The alternateWorks system appends the selected works together. The first
  // is the original text chosen and the subsequent works are the alternates.
  if (this.opts.osisIDWork) {
    this.opts.osisIDWorkParts = this.opts.osisIDWork.split(',');
  }
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

  if (typeof bcv_parser === 'function') {
    this.bcv = new bcv_parser;
    this.bcv.set_options({
      book_alone_strategy: 'include',
      book_sequence_strategy: 'include'
    });
  } else {
    this.bcv = false;
  }

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
      var search = $('#edit-search', $toolbar).val(),
          osises = that.bcv.parse(search).osis_and_indices(),
          did_search = search.length > 0 ? false : true,
          parts, endParts,
          book, chapter, verse;

      if (osises.length > 0) {
        parts = osises[0].osis.split('.');

        book = util.findByName(that.viewData.selectedWork.books, parts[0]);

        if (book) {
          that.viewData.selectedBook = book;

          chapter = util.findByName(that.viewData.selectedBook.chapters, parts[1]);

          if (chapter) {
            that.viewData.selectedChapter = chapter;

            // TODO: Highlight the matched verses and scroll to that point.
          }

          that.renderViews(['toolbar']);
          that.populatePassages();
          did_search = true;
        }
      }

      if (!did_search) {
        that.setMessage(Drupal.t('Could not find any passages matching "@search". Try using the format "Book chapter:verse", eg: "John 3:16".', { '@search': search }), 'warning');
      }

      return false;
    });

  $('#edit-work-filter', $toolbar)
    .change(function () {
      that.viewData.selectedWork = util.findByName(that.viewData.works, $(this).val());
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
      var $this = $(this),
          $checkboxes = $('.form-item-passage', that.$element),
          originI = $checkboxes.index(this);

      if ($this.attr('checked')) {
        // Determine the contiguous group this checkbox belongs to then remove
        // all other ticked boxes
        var first = that.contiguous($checkboxes, originI, -1),
            last = that.contiguous($checkboxes, originI,  1),
            firstVerse = first.split('.')[2],
            lastVerse = last.split('.')[2];

        $checkboxes.each(function () {
          var parts = $(this).val().split('.');

          if (parts[2] < firstVerse || parts[2] > lastVerse) {
            $(this).removeAttr('checked');
          }
        });

        if (that.viewData.alternateWorks.length > 0) {
          that.showAlternateWorks();
        }
      } else if ($checkboxes.filter('[checked]').length == 0) {
        that.hideAlternateWorks();
      }
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
        pagesize:  500, // FIXME: Fixed page size will cause any really long chapters to be chopped off.
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
        firstVerse = parseInt(parts[0], 10);
        lastVerse = parts.length == 2 ? parseInt(parts[1], 10) : firstVerse;
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
      work:    this.opts.osisIDWorkParts[0],
      book:    this.opts.osisIDParts[0],
      chapter: this.opts.osisIDParts[1]
    };

    for (i = 0, len = toDefault.length; i < len; i++) {
      type = toDefault[i];

      // Skip unknown types
      objects = [];
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

  this.setAlternateWorks(this.viewData.selectedWork, this.viewData.selectedBook, this.viewData.selectedChapter, toDefault.indexOf('work') != -1);
}

PickerUI.prototype.setAlternateWorks = function (originWork, originBook, originChapter, setDefaults) {
  var i, j, k,
      work, book, chapter;

  setDefaults = typeof setDefaults !== 'undefined' ? setDefaults : false;

  this.viewData.alternateWorks = [];
  for (i = this.viewData.works.length - 1; i >=0; i--) {
    work = this.viewData.works[i];

    // Don't also add the origin work as an alternate work
    if (work.name == originWork.name) {
      continue;
    }

    for (j = work.books.length - 1; j >= 0; j--) {
      book = work.books[j];

      // This work contains the same book as the origin, next check if has
      // the same chapter too
      if (book.name == originBook.name) {
        for (k = book.chapters.length - 1; k >= 0; k--) {
          chapter = book.chapters[k];

          // This work contains the same book and chapter as the origin
          // that means that almost certainly it is a valid alternate work
          if (chapter.name == originChapter.name) {
            // Need to make a deep copy here, else alternateWorks stae will be
            // bound to works!
            this.viewData.alternateWorks.push($.extend(true, {}, work));
            break; // Successful match found, quit searching chapters
          }
        }
        break; // Matched book, quit searching books
      }
    }
  }

  if (this.viewData.alternateWorks.length == 0) {
    this.hideAlternateWorks(false);
  } else {
    if (setDefaults && this.opts.osisIDWorkParts.length > 1) {
      var alternate,
          alternates = this.opts.osisIDWorkParts.slice(1);

      for (i = this.viewData.alternateWorks.length - 1; i >= 0; i--) {
        alternate = this.viewData.alternateWorks[i];

        if (alternates.indexOf(alternate.name) != -1) {
          this.viewData.alternateWorks[i].selected = true;
        }
      }
    }

    this.showAlternateWorks(false);
  }
};

PickerUI.prototype.showAlternateWorks = function (animated) {
  var that = this,
      selector, css,
      ops  = {
        // FIXME: This hardcoded height will break if too many alternate works are listed
        '.alternateWorks': { height: 36 },
        '.passages': { paddingTop: 70 + 36 + 5 }
      };

  animated = typeof animated !== 'undefined' ? animated : true;

  for (selector in ops) {
    if (ops.hasOwnProperty(selector)) {
      css = ops[selector];

      if (animated) {
        $(selector, this.$element).animate(css, 'fast');
      } else {
        $(selector, this.$element).css(css);
      }
    }
  }
};

PickerUI.prototype.hideAlternateWorks = function (animated) {
  var that = this,
      selector, css,
      ops  = {
        '.alternateWorks': { height: 0 },
        '.passages': { paddingTop: 70 + 5 }
      };

  animated = typeof animated !== 'undefined' ? animated : true;

  for (selector in ops) {
    if (ops.hasOwnProperty(selector)) {
      css = ops[selector];

      if (animated) {
        $(selector, this.$element).animate(css, 'fast');
      } else {
        $(selector, this.$element).css(css);
      }
    }
  }
};

PickerUI.prototype.getSelectionOSIS = function ($origin) {
  $origin = $origin || $('.form-item-passage[checked]:first', this.$element);

  var first = $origin.val(),
      last = $origin.val(),
      $checkboxes = $('.form-item-passage', this.$element),
      length = $checkboxes.length,
      originI = $checkboxes.index($origin),
      firstParts, lastParts, osisID, osisIDWork;

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

    osisIDWork = [$('#edit-work-filter', this.$element).val()];
    $('input.form-item-alternate-works', this.$element).each(function () {
      if (this.checked) {
        osisIDWork.push($(this).val());
      }
    })

    return {
      osisIDWork: osisIDWork.join(','),
      osisID: osisID.join('.')
    }
  }

  this.setMessage(Drupal.t('A passage must be selected.'), 'error');
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

PickerUI.prototype.setMessage = function (message, type, hide_after) {
  hide_after = hide_after || null;
  util.setMessage($('.passages', this.$element), message, type, hide_after);
};
