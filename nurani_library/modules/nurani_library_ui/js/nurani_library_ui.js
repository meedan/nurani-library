var NL = (function ($) {

  /**
   * Corebox util library.
   */
  function Util() {
  }

  // Globally available CB.Util
  var util = new Util();

  // paulirish.com/2009/log-a-lightweight-wrapper-for-consolelog/
  var log = function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};


  /**
   * Capitalizes the first letter of a string.
   * @see: http://stackoverflow.com/a/1026087/806988
   */
  Util.prototype.capitalize = function (string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
  };

  Util.prototype.findByName = function (array, name) {
    var search, key;
    search = $.grep(array, function (o, i) { return o.name == name; });
    return search.length > 0 ? $.extend({ _key: array.indexOf(search[0]) }, search[0]) : false;
  };
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
    var that = this;

    $('.form-item-passage', $passages)
      .click(function () {
        var $this = $(this);

        if (!$this.attr('checked')) {
          return true;
        }

        // Determine the contiguous group this checkbox belongs to then remove
        // all other ticked boxes
        var $checkboxes = $('.form-item-passage', that.$element),
            origin_i = $checkboxes.index(this),
            first = that.contiguous($checkboxes, origin_i, -1),
            last = that.contiguous($checkboxes, origin_i,  1),
            first_verse = first.split('.')[2],
            last_verse = last.split('.')[2];

        $checkboxes.each(function () {
          var parts = $(this).val().split('.');

          if (parts[2] < first_verse || parts[2] > last_verse) {
            $(this).removeAttr('checked');
          }
        });
      });
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

  PickerUI.prototype.getSelectionOSIS = function ($origin) {
    $origin = $origin || $('.form-item-passage[checked]:first', this.$element);

    var first = $origin.val(),
        last = $origin.val(),
        $checkboxes = $('.form-item-passage', this.$element),
        length = $checkboxes.length,
        origin_i = $checkboxes.index($origin),
        first_parts, last_parts, osisID;

    if (origin_i >= 0) {
      // Iterate backwards and forwards to find boundaries of this contiguously
      // checked group.
      first = this.contiguous($checkboxes, origin_i, -1);
      last  = this.contiguous($checkboxes, origin_i,  1);

      first_parts = first.split('.');
      last_parts = last.split('.');

      osisID = [];
      osisID.push(first_parts[0]);
      osisID.push(first_parts[1]);

      if (first_parts[2] != last_parts[2]) {
        osisID.push(first_parts[2] + '-' + last_parts[2]);
      } else {
        osisID.push(first_parts[2]);
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
    this.$element.find('.toolbar').css('width', this.$element.width());
  };


  // Static template for the picker UI. Handlebars compiles this shared template
  // into a specific HTML blob for each PickerUI instance.
  PickerUI.templates = {
    toolbar: [
      // Search box
      '<div class="form-item form-type-textfield form-item-search">',
        '<label for="edit-search">',
          'Search for a passage ',
        '</label>',
        '<input type="text" id="edit-search" name="search" value="" size="60" maxlength="1024" class="form-text required">',
      '</div>',
      '<input class="search-action form-submit" type="submit" id="edit-search-submit" name="op" value="Search">',
      // Chapter filter
      '<div class="form-item form-type-select form-item-chapter-filter">',
        '<label for="edit-chapter-filter">',
          'Chapter ',
        '</label>',
        '<select id="edit-chapter-filter" name="chapter_filter" class="form-select">',
          '{{#eachOption selected_book.chapters selected_chapter}}',
            '<option value="{{name}}"{{markSelected this}}>{{full_name}}</option>',
          '{{/eachOption}}',
        '</select>',
      '</div>',
      // Book filter
      '<div class="form-item form-type-select form-item-book-filter">',
        '<label for="edit-book-filter">',
          'Book ',
        '</label>',
        '<select id="edit-book-filter" name="book_filter" class="form-select">',
          '{{#eachOption selected_work.books selected_book}}',
            '<option value="{{name}}"{{markSelected this}}>{{full_name}}</option>',
          '{{/eachOption}}',
        '</select>',
      '</div>',
      // Works filter
      '<div class="form-item form-type-select form-item-work-filter">',
        '<label for="edit-work-filter">',
          'Text ',
          '<span class="form-required" title="This field is required.">*</span>',
        '</label>',
        '<select id="edit-work-filter" name="work_filter" class="form-select required">',
          '{{#eachOption works selected_work}}',
            '<option value="{{name}}"{{markSelected this}}>{{full_name}}</option>',
          '{{/eachOption}}',
        '</select>',
      '</div>',
    ].join(''),


    passages: [
      '{{#each passages}}',
        '{{#isChapterBeginning this}}',
          '<h4>{{book_full_name}}, Chapter {{chapter_full_name}}</h4>',
        '{{/isChapterBeginning}}',
        '<div class="form-item form-type-checkbox form-item-passage-row {{work_language}}">',
          // "Select passage" tickbox
          '<input type="checkbox" id="{{css_id}}" name="passage[]" value="{{osisID}}" class="form-checkbox form-item-passage"> ',
          // The verse and its number link
          '<label class="option" for="{{css_id}}">',
            '<span class="verse">',
              '<a href="{{verse_url}}">{{verse}}</a>',
            '</span> ',
            '{{text}}',
          '</label>',
        '</div>',
      '{{/each}}',
    ].join('')
  };


  $(function () {

    /**
     * Handlebars.js helper, detects first chapter and verse condition
     */
    Handlebars.registerHelper('isBookBeginning', function (passage, options) {
      if (passage.chapter_name == 1) {
        return options.fn(this);
      }
    });

    /**
     * Handlebars.js helper, detects first chapter and verse condition
     */
    Handlebars.registerHelper('isChapterBeginning', function (passage, options) {
      if (passage.verse == 1) {
        return options.fn(this);
      }
    });

    /**
     * Extension of the 'each' helper which marks each item as selected or not.
     */
    Handlebars.registerHelper('eachOption', function(context, selected, options) {
      var fn = options.fn, inverse = options.inverse;
      var ret = "", data;

      if (options.data) {
        data = Handlebars.createFrame(options.data);
      }

      if (context && context.length > 0) {
        for (var i = 0, j = context.length; i < j; i++) {
          if (data) { data.index = i; }
          ret = ret + fn($.extend({ selected: (context[i].name == selected.name) }, context[i]), { data: data });
        }
      } else {
        ret = inverse(this);
      }

      return ret;
    });

    /**
     * Helps selecting <option></option> tags.
     */
    Handlebars.registerHelper('markSelected', function (context) {
      return context.selected ? ' selected="selected"' : '';
    });
  });

  return {PickerUI: PickerUI};

})(jQuery);