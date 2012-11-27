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
