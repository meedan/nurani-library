// Static template for the picker UI. Handlebars compiles this shared template
// into a specific HTML blob for each PickerUI instance.
PickerUI.templates = {
  toolbar: [
    // Search box
    '<div class="form-item form-type-textfield form-item-search">',
      '<label for="edit-search">',
        'Search for a passage ',
      '</label>',
      '<input type="text" id="edit-search" name="search" value="{{currentSearch}}" size="30" maxlength="1024" class="form-text required">',
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
    '<table class="{{#if annotationsEnabled}}has-annotations{{/if}}">',
      '<tbody>',
        '{{#each passages}}',
          '{{#isChapterBeginning this}}',
            '<tr class="heading">',
              '<td class="passage">',
                '<h4>{{book_full_name}}, Chapter {{chapter_full_name}}</h4>',
              '</td>',
              '{{#if ../annotationsEnabled}}',
                '<td class="annotations">',
                  '<h4>Annotations</h4>',
                '</td>',
              '{{/if}}',
            '</tr>',
          '{{/isChapterBeginning}}',
          '<tr class="passage-row {{oddOrEven verse}}">',
            '<td class="passage" data-index="{{@index}}">',
              '<div class="form-item form-type-checkbox form-item-passage-row form-item-passage-row-{{verse}} {{work_language}} {{direction}}">',
                // "Select passage" tickbox
                '<input type="checkbox" id="{{cssId}}" name="passage[]" value="{{osisID}}" class="form-checkbox form-item-passage"{{selected this "checked"}}> ',
                // The verse and its number link
                '<label class="option" for="{{cssId}}">',
                  '<span class="verse">',
                    // TODO: When ready, link to verses in the Nurani Library.
                    // '<a href="{{verseUrl}}">{{verse}}</a>',
                    '<strong>{{verse}}</strong>',
                  '</span> ',
                  // NOTE: triple '{{{.}}}' for RAW output. This is coming direct from
                  // the Nurani Library server and should not be an XSS vector.
                  '{{{text}}}',
                '</label>',
              '</div>',
            '</td>',
            '{{#if ../annotationsEnabled}}',
              '<td class="annotations">',
                '{{#each notes}}',
                  '{{> annotation this}}',
                '{{/each}}',
              '</td>',
            '{{/if}}',
          '</tr>',
        '{{/each}}',
      '</tbody>',
    '</table>'
  ].join('')
};

PickerUI.partials = {
  annotation: [
    '<div class="annotation {{annotationClasses this}} clearfix" data-index="{{@index}}">',
      '<div class="inner">',
        '<h5 class="title">{{passage_title}}</h5>',

        '<div class="contents clearfix">',
          '<span class="value expandable" data-slice-point="120">{{value}}</span>',
          '{{#if author}}',
            '<span class="attribution">',
              '— <a href="{{author.url}}" title="View user profile." class="username" xml:lang="" about="{{author.url}}" typeof="sioc:UserAccount" property="foaf:name">{{author.name}}</a>',
            '</span>',
          '{{/if}}',
          '{{#if editable}}',
            '<div class="actions clearfix">',
              '<input class="delete-annotation-action form-submit" type="submit" id="edit-delete-annotation-submit-annotation-submit" name="op" value="Delete">',
              '<input class="edit-annotation-action form-submit" type="submit" id="edit-edit-annotation-submit" name="op" value="Edit">',
            '</div>',
          '{{/if}}',
        '</div>',

        '{{#if editable}}',
          '{{> annotationForm this}}',
        '{{/if}}',
      '</div>',
    '</div>'
  ].join(''),

  newAnnotationButton: [
    '<div class="new-annotation-button">',
      '<input class="new-annotation-form-action form-submit" type="submit" id="edit-new-annotation-submit" name="op" value="Annotate this passage">',
    '</div>'
  ].join(''),

  annotationForm: [
    '<form class="annotation-form" method="POST">',
      '<div class="form-item form-type-textarea form-item-value">',
        '<div class="form-textarea-wrapper">',
          '<textarea id="edit-value" name="value" cols="10" rows="5" class="form-textarea">{{value}}</textarea>',
        '</div>',
      '</div>',
      '<div class="actions clearfix">',
        '<a href="#" class="cancel-annotation-action">Cancel</a>',
        '<input class="save-annotation-action form-submit" type="submit" id="edit-save-annotation-submit" name="op" value="Save">',
      '</div>',
      '<input type="hidden" name="id" value="{{id}}">',
      '<input type="hidden" name="passage_id" value="{{passage_id}}">',
      '<input type="hidden" name="author_uuid" value="{{author_uuid}}">',
      '<input type="hidden" name="type" value="{{type}}">',
      '<input type="hidden" name="position" value="{{position}}">',
      '<input type="hidden" name="length" value="{{length}}">',
    '</form>'
  ].join('')
};

$(function () {
  var key;

  // Register all partials
  for (key in PickerUI.partials) {
    if (PickerUI.partials.hasOwnProperty(key)) {
      Handlebars.registerPartial(key, PickerUI.partials[key]);
    }
  }

  /**
   * Handlebars.js helper, detects first chapter and verse condition
   */
  Handlebars.registerHelper('isBookBeginning', function (passage, options) {
    if (passage.chapterName === 1) {
      return options.fn(this);
    }
  });

  /**
   * Handlebars.js helper, detects first chapter and verse condition
   */
  Handlebars.registerHelper('isChapterBeginning', function (passage, options) {
    if (passage.verse === 1) {
      return options.fn(this);
    }

    return options.inverse(this);
  });

  /**
   * Extension of the 'each' helper which marks each item as selected or not.
   */
  Handlebars.registerHelper('eachOption', function(context, selected, options) {
    var fn = options.fn,
        inverse = options.inverse,
        ret = "",
        data,
        i, j;

    if (options.data) {
      data = Handlebars.createFrame(options.data);
    }

    if (context && context.length > 0) {
      for (i = 0, j = context.length; i < j; i++) {
        if (data) { data.index = i; }
        ret = ret + fn($.extend({ selected: (context[i].name === selected.name) }, context[i]), { data: data });
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

  /**
   * Helps truncating strings of text.
   */
  Handlebars.registerHelper('truncate', function (string, length) {
    length = length || 120;

    if (string.length > length) {
      // TODO: More/less for truncated text
      string = [
        // '<span class="short">',
          string.substr(0, length) + '&hellip;'
          // '<a href="" onclick="jQuery(this).parent(\'\').siblings(\'\')">[more]</a>',
        // '</span>'
      ].join('');
    }
    return new Handlebars.SafeString(string);
  });

  /**
   * Helps adding odd or even classes
   */
  Handlebars.registerHelper('oddOrEven', function (row) {
    return new Handlebars.SafeString(row % 2 === 0 ? 'even' : 'odd');
  });

  /**
   * A ternary operator.
   *
   * Eg:
   *  {{ternary true  "1" "2"}} -> "1"
   *  {{ternary false "1" "2"}} -> "2"
   */
  Handlebars.registerHelper('ternary', function (context, ifTrue, ifFalse) {
    ifFalse = ifFalse || '';
    return new Handlebars.SafeString(context ? ifTrue : ifFalse);
  });

  /**
   * Generates classes for an annotation.
   */
  Handlebars.registerHelper('annotationClasses', function (annotation) {
    var classes = ['annotation'];

    if (annotation.type !== 'annotation') {
      classes.push(annotation.type);
    }
    if (annotation.editing) {
      classes.push('editing');
    }
    if (annotation.editable) {
      classes.push('editable');
    }

    return new Handlebars.SafeString(classes.join(' '));
  });

  Handlebars.registerHelper('whatis', function (obj) {
    console.log(obj);
  });

});
