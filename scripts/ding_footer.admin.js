(function ($) {
  Drupal.behaviors.dingFooter = {
    attach: function(context) {
      $('.ding-footer-block .remove').click(function () {
        $(this).parents('tr').remove();

        return false;
      });
    }
  }
})(jQuery);
