<?php

/**
 * @file
 *
 * Views definitions for Social Media Feeds module.
 */

/**
 * Implements hook_views_plugins().
 *
 * @return array
 */
function social_media_feeds_views_plugins() {
  $path = drupal_get_path('module', 'social_media_feeds') . '/views';
  return array(
    'module' => 'social_media_feeds',
    'style' => array(
      'social_media_feeds' => array(
        'title' => t('Social Media Feeds'),
        'help' => t('Social Media Feeds'),
        'handler' => 'social_media_feeds_views_plugin_style',
        'theme' => 'views_social_media_feeds_style',
        'theme file' => 'views-social-media-feeds-style.tpl.php',
        'theme path' => "$path/theme",
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'path' => "$path/plugins",
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );
}