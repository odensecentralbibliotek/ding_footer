<?php
/** Variables: $footer_blocks **/

$num_footer_blocks = count($footer_blocks);

?>
<div class="ding-footer-wrapper">
<?php
  
foreach($footer_blocks as $key => $foot) {
  
?>
  <div class="ding-footer-column footer-block-<?php echo ($key + 1);?>-of-<?php echo $num_footer_blocks; ?>">
<?php $foot['content']; ?>
  </div>
<?php

}

?>
</div>
