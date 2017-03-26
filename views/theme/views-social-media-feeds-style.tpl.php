<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php if (isset($rows)) foreach ($rows as $id => $row): ?>
  <div<?php if (!empty($classes_array)) {
    print ' class="' . implode(' ', $classes_array) . '"';
  } ?>>
    <?php print $row; ?>
  </div>
<?php endforeach; ?>
