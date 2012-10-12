<?php
/**
 * @file
 * Default theme implementation to display ding footer.
 *
 * Available variables:
 * - $blocks: xxx.
 */
?>
<div class="ding-footer ding-footer-wrapper">
  <?php foreach($blocks as $bid => $block) { ?>
  <div class="column block-<?php echo $bid; ?>">
    <?php echo $block; ?>
  </div>
  <?php } ?>
</div>
