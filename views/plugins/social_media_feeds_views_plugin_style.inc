<?php

/**
 * @file
 *
 * Social Media Feeds custom Views plugins.
 */

/**
 * Class social_media_feeds_views_plugin_style
 */
class social_media_feeds_views_plugin_style extends views_plugin_style_list {

  // Add custom options to the Views style.
  function option_definition() {
    $options = parent::option_definition();
    $options['social_media_feeds_arrange'] = array('default' => 0);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Add checkboxes to determine returned data style, indexed by type (multi-
    // dimension indexed array, or aggregated (single dimension array)
    $form['social_media_feeds_arrange'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Organize Posts'),
      '#description' => t('Display posts grouped by post type or group together.'),
      '#options' => drupal_map_assoc(array(t('Indexed'), t('Aggregated'))),
      '#default_value' => $this->options['social_media_feeds_arrange'],
    );
  }
}
