(function ($) {
  Drupal.behaviors.ding_footer_callback = {
    attach: function (context, settings) {
      $('#edit-submit').click(function() {     
         $('.wysiwyg-ajax').each(function() {               
           var id = $(this).attr('id');
           var data = CKEDITOR.instances[id].getData();           
           $(this).val(data);
         });
      });
    }
  };
  
})(jQuery);

