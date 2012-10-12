/**
 * When using AJAX callbacks to add an another textarea (footer block) that have
 * wysiwyg editors enable, something happens with the editors and there content
 * is not submitted to the server when "Save" is triggered.
 *
 * So to ensure that content is moved from the editor back into the textarea, we
 * toggle all the editors, which forces them to move the content into the
 * textarea before submit.
 */
(function ($) {
  $(document).ready(function() {
    $('#edit-submit').click(function() {
      $('.wysiwyg-ajax').each(function() {
        var id = $(this).attr('id');
        var params = Drupal.wysiwyg.instances[id];
        Drupal.wysiwyg.toggleWysiwyg({
          'data' : {
            'params' : params,
            'context' : $('#blocks-fieldset-wrapper')
          }
        });
      });
    });
  });
})(jQuery);
