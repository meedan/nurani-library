(function ($) {

Drupal.behaviors.nuraniLibraryRevealPassword = {
  attach: function (context) {

    var $wrapper = $('.reveal-password', context),
        $pass    = $('<span class="pass" style="display: none;">' + $wrapper.html() + '</span>'),
        $confirm = $('<a href="#" class="confirm">' + Drupal.t('Reveal password?') + '</a>');

    $wrapper
      .html('')
      .append($pass)
      .append($confirm);

    $('.confirm', $wrapper).click(function () {
      $confirm.hide();
      $pass.show();
      return false; // Don't follow href
    });

    // NOTE: Uncomment to allow un-revealing
    // $('.pass', $wrapper).click(function () {
    //   $pass.hide();
    //   $confirm.show();
    // });

  }
};

}(jQuery));
