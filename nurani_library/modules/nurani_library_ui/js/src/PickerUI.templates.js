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
        '{{#each chapters}}',
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
        '{{#each books}}',
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
