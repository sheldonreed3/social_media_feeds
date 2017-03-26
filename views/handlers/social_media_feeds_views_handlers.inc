<?php

/**
 * @file
 *
 * Social Media Feeds custom Views handlers.
 */

/**
 * Class social_media_feeds_handler_field
 */

class social_media_feeds_handler_field extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
  }

  // Override query().
  function query() { }

  /**
   * @param $values
   */
  function post_execute(&$values) {
    // Run the feeds import on post_execute so that the values are cached.
    parent::post_execute($values);
    if (empty($values)) {
      // Get display type from options.
      foreach ($this->view->style_plugin->options['social_media_feeds_arrange'] as $type) {
        if ($type !== 0) {
          $this->view->social_media_feeds_arrange = strtolower($type);
          break;
        }
      }
      // Import the social media feeds.
      $feed = new social_media_feeds_data();
      switch ($this->view->social_media_feeds_arrange) {
        case 'aggregated':
          $values = $feed->get_aggregated_data();
          break;
        case 'indexed':
          $values = $feed->get_indexed_data();
          break;
      }
    }
  }

  /**
   * @param $values
   *
   * @return string
   */
  function render($values) {
    // Display thumbnail link.
    return $values[$this->real_field];
  }
}

class social_media_feeds_handler_sort extends views_handler_sort {

  function init(&$view, &$options) {
    parent::init($view, $options);
  }

  function query() { }

  function post_execute(&$values) {
    // Let's retain the items_per_page setting from the Views UI.
    $per_page = $this->query->pager->options['items_per_page'];

    switch ($this->view->social_media_feeds_arrange) {
      case 'aggregated':
        // Sort our custom values.
        $values = $this->sort($values);
        break;
      case 'indexed':
        // Sort the indexed arrays.
        $sort_vals = array();
        foreach ($values as $index => $value) {
          $sort_vals[$index] = $this->sort($value);
        }
        // Merge the array, removing the groups.
        $merge_values = array();
        for ($i = 0; $i < $per_page; $i++) {
          foreach (array_keys($values) as $key) {
            if (isset($sort_vals[$key][$i])) {
              $merge_values[] = $sort_vals[$key][$i];
            }
          }
        }
        $values = $merge_values;
        break;
    }
    // Limit the returned values per items_per_page setting.
    $values = array_slice($values, 0 , $per_page);
  }

  function sort($data) {
    // Sort function for the social feeds handlers.
    switch ($this->options['order']) {
      case 'ASC':
        usort($data, function ($a, $b) {
          // Default to the created field (for some reason $this isn't always an
          // object.
          $field = isset($this) && is_object($this) ? $this->real_field : 'timestamp';
          return $a[$field] >= $b[$field] ? 1 : 0;
        });
        break;
      case 'DESC':
        usort($data, function ($a, $b) {
          $field = isset($this) && is_object($this) ? $this->real_field : 'timestamp';
          return $a[$field] <= $b[$field] ? 0 : 1;
        });
        break;
    }
    return $data;
  }
}
