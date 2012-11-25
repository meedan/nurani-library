var NL = (function ($) {

  // paulirish.com/2009/log-a-lightweight-wrapper-for-consolelog/
  var log = function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};

  /**
   * Corebox util library.
   */
  function Util() {
  }

  // Globally available CB.Util
  var util = new Util();

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
        that.viewData.works = that.unpackWorkData(data);

        if (!that.viewData.selected_work) {
          that.viewData.selected_work = that.viewData.works[0];
        }
        if (!that.viewData.selected_book) {
          that.viewData.selected_book = that.viewData.selected_work.books[0];
        }

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
          '{{#each selected_book.chapters}}',
            '<option value="{{name}}">{{full_name}}</option>',
          '{{/each}}',
        '</select>',
      '</div>',
      // Book filter
      '<div class="form-item form-type-select form-item-book-filter">',
        '<label for="edit-book-filter">',
          'Book ',
        '</label>',
        '<select id="edit-book-filter" name="book_filter" class="form-select">',
          '{{#each selected_work.books}}',
            '<option value="{{name}}">{{full_name}}</option>',
          '{{/each}}',
        '</select>',
      '</div>',
      // Works filter
      '<div class="form-item form-type-select form-item-work-filter">',
        '<label for="edit-work-filter">',
          'Text ',
          '<span class="form-required" title="This field is required.">*</span>',
        '</label>',
        '<select id="edit-work-filter" name="work_filter" class="form-select required">',
          '{{#each works}}',
            '<option value="{{name}}">{{full_name}}</option>',
          '{{/each}}',
        '</select>',
      '</div>',
    ].join(''),


    passages: [
      '{{#each passages}}',
        '<div class="form-item form-type-checkbox form-item-passage-row {{work_language}}">',
          // "Select passage" tickbox
          '<input type="checkbox" id="{{css_id}}" name="passage" value="{{osisID}}" class="form-checkbox"> ',
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

  return {PickerUI: PickerUI};

})(jQuery);