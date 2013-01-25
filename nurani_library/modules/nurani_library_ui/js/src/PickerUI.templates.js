// Static template for the picker UI. Handlebars compiles this shared template
// into a specific HTML blob for each PickerUI instance.
PickerUI.templates = {
  toolbar: [
    // Search box
    '<div class="form-item form-type-textfield form-item-search">',
      '<label for="edit-search">',
        'Search for a passage ',
      '</label>',
      '<input type="text" id="edit-search" name="search" value="{{currentSearch}}" size="60" maxlength="1024" class="form-text required">',
    '</div>',
    '<input class="search-action form-submit" type="submit" id="edit-search-submit" name="op" value="Search">',

    // Chapter filter
    '<div class="form-item form-type-select form-item-chapter-filter">',
      '<label for="edit-chapter-filter">',
        'Chapter ',
      '</label>',
      '<select id="edit-chapter-filter" name="chapterFilter" class="form-select">',
        '{{#eachOption selectedBook.chapters selectedChapter}}',
          '<option value="{{name}}"{{selected this "selected"}}>{{full_name}}</option>',
        '{{/eachOption}}',
      '</select>',
    '</div>',
    // Book filter
    '<div class="form-item form-type-select form-item-book-filter">',
      '<label for="edit-book-filter">',
        'Book ',
      '</label>',
      '<select id="edit-book-filter" name="bookFilter" class="form-select">',
        '{{#eachOption selectedWork.books selectedBook}}',
          '<option value="{{name}}"{{selected this "selected"}}>{{full_name}}</option>',
        '{{/eachOption}}',
      '</select>',
    '</div>',
    // Works filter
    '<div class="form-item form-type-select form-item-work-filter">',
      '<label for="edit-work-filter">',
        'Text ',
        '<span class="form-required" title="This field is required.">*</span>',
      '</label>',
      '<select id="edit-work-filter" name="workFilter" class="form-select required">',
        '{{#eachOption works selectedWork}}',
          '<option value="{{name}}"{{selected this "selected"}}>{{full_name}}</option>',
        '{{/eachOption}}',
      '</select>',
    '</div>'
  ].join(''),

  alternateWorks: [
    '<div class="inner clearfix">',
      '<label>',
        'Additional translations to display ',
      '</label>',
      '{{#each alternateWorks}}',
        '<div class="form-item form-type-checkbox form-item-alternate-works">',
          '<input type="checkbox" id="{{cssId}}" name="alternateWorks[]" value="{{name}}" class="form-checkbox form-item-alternate-works"{{selected this "checked"}}> ',
          '<label class="option" for="{{cssId}}">',
            '{{full_name}}',
          '</label>',
        '</div>',
      '{{/each}}',
    '</div>'
  ].join(''),

  passages: [
    '{{#each passages}}',
      '{{#isChapterBeginning this}}',
        '<h4>{{book_full_name}}, Chapter {{chapter_full_name}}</h4>',
      '{{/isChapterBeginning}}',
      '<div class="form-item form-type-checkbox form-item-passage-row form-item-passage-row-{{verse}} {{work_language}}">',
        // "Select passage" tickbox
        '<input type="checkbox" id="{{cssId}}" name="passage[]" value="{{osisID}}" class="form-checkbox form-item-passage"{{selected this "checked"}}> ',
        // The verse and its number link
        '<label class="option" for="{{cssId}}">',
          '<span class="verse">',
            // TODO: When ready, link to verses in the Nurani Library.
            // '<a href="{{verseUrl}}">{{verse}}</a>',
            '<strong>{{verse}}</strong>',
          '</span> ',
          // Note, triple '{{{.}}}' for RAW output. This is coming direct from
          // the Nurani Library server and should not be an XSS vector.
          '{{{text}}}',
        '</label>',
      '</div>',
    '{{/each}}'
  ].join('')
};


$(function () {

  /**
   * Handlebars.js helper, detects first chapter and verse condition
   */
  Handlebars.registerHelper('isBookBeginning', function (passage, options) {
    if (passage.chapterName == 1) {
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
   * Helps selecting / checking <option></option> and <input> tags.
   */
  Handlebars.registerHelper('selected', function (context, label) {
    label = label || 'selected';
    return new Handlebars.SafeString(context.selected ? ' ' + label + '="' + label + '"' : '');
  });

});
