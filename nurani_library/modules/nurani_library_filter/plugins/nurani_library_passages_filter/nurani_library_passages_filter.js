jQuery(function ($) {

  /**
   * CKEditor plugin to enable picking a Nurani Library passage reference.
   */
  CKEDITOR.plugins.add('nurani_library_passages_filter', {
    init: function(editor) {
      var nuraniLibraryPickerUI = new NL.PickerUI({
            osisIDWork: null,
            osisID: null
          }),
          $window = $(window),
          $dialog = nuraniLibraryPickerUI.$element.dialog({
            autoOpen: false,
            width: $window.width() * 0.88,
            height: $window.height() * 0.80,
            modal: true,
            buttons: {
              Done: function() {
                var data = nuraniLibraryPickerUI.getSelectionOSIS();

                if (data) {
                  editor.insertHtml('[' + data.osisIDWork + ':' + data.osisID + ']');
                }

                $(this).dialog('close');
              },
              Cancel: function() {
                $(this).dialog('close');
              }
            },
            close: function() {
              // TODO: Do things on close, like clear the state.
            },
            open: function () { nuraniLibraryPickerUI.didResize(); },
            resize: function () { nuraniLibraryPickerUI.didResize(); }
          });

      editor.addCommand('nurani_library_passages_filter', {
        exec: function (editor) {
          $dialog.dialog('open');
        }
      });

      editor.ui.addButton('nurani_library_passages_filter', {
        label: 'Insert Nurani Library passage reference',
        command: 'nurani_library_passages_filter',
        icon: this.path + 'images/nurani_library_passages_filter.png'
      });
    }
  });

}(jQuery));
