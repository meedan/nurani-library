(function ($) {

/**
 * .
 */
Drupal.behaviors.nuraniLibraryWorksFormTable = {
  attach: function (context) {
    $('.delete-action', context).click(function (e) {
      // TODO: Finish the delete action.
      // e.stopPropagation();
      // return false;
    });

    $('.add-action', context).click(function (e) {
      // TODO: Finish the add action.
      // e.stopPropagation();
      // return false;
    });
  }
};

})(jQuery);