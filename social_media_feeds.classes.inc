<?php

/**
 * @file
 * Main class for the Social Media Feeds module.
 *
 * Provide public functions to access data from custom modules.
 */

class social_media_feeds_data {
  protected $fb_data = array(), $tw_data = array(), $inst_data = array();

  function __construct() {
    // Pull in the social data when constructed.
    $this->fb_data = $this->set_fb_data();
    $this->tw_data = $this->set_tw_data();
    $this->inst_data = $this->set_inst_data();
  }

  /**
   * Return a merged array.
   *
   * @return array
   */
  public function get_aggregated_data() {
    return array_merge($this->tw_data, $this->fb_data, $this->inst_data);
  }

  /**
   * Return an indexed, by type, array of social feed data.
   *
   * @return array
   */
  public function get_indexed_data() {
    $indexed = array();
    if (!empty($this->fb_data)) {
      $indexed['facebook'] = $this->fb_data;
    }
    if (!empty($this->tw_data)) {
      $indexed['twitter'] = $this->tw_data;
    }
    if (!empty($this->inst_data)) {
      $indexed['instagram'] = $this->inst_data;
    }
    return $indexed;
  }

  /**
   * Public function to access social data after Social Feeds has added some
   * elements.
   *
   * @return array
   */
  public function get_feed_data($type) {
    switch (strtolower($type)) {
      case 'twitter':
        return $this->tw_data;
        break;
      case 'facebook':
        return $this->fb_data;
        break;
      case 'instagram':
        return $this->inst_data;
        break;
      default:
        return FALSE;
    }
  }

  /**
   * Standardize Twitter data.
   *
   * @return array
   */
  protected function set_tw_data() {
    $data = array();
    $tw = $this->get_twitter_data();
    if ($tw) {
      foreach ($tw as $tweet) {
        if (isset($tweet['extended_entities'])) {
          // Save image url and post url without the protocol
          $tweet['image_url'] = str_ireplace(array(
            'http:',
            'https:'
          ), '', $tweet['extended_entities']['media'][0]['media_url']);
          $tweet['post_url'] = '//' . $tweet['extended_entities']['media'][0]['display_url'];
          // We need to add type of feed and created timestamp to the post data.
          $tweet['timestamp'] = strtotime($tweet['created_at']);
          $tweet['type'] = 'twitter';
          $data[] = $tweet;
        }
      }
    }
    return $data;
  }

  /**
   * Standardize Facebook data.
   *
   * @return array
   */
  protected function set_fb_data() {
    $data = array();
    $fb = $this->get_facebook_data();
    if ($fb) {
      $fields =  variable_get('social_media_feeds_fb_fields', array('attachments', 'created_time'));
      foreach ($fb as $post) {
        // Make it so the attachments aren't buried.
        // TODO Generalize this so it works for multiple field options.
        $post['attachments'] = $post['attachments']['data'][0];

        // Let's move all pertinent field information to the same level.
        foreach ($fields as $field) {
          if (is_array($post[$field])) {
            foreach ($post[$field] as $index => $item) {
              $post[$index] = $item;
            }
            unset($post[$field]);
          }
        }
        // Save image url and post url without the protocol
        // We need to grab the first image if there are multiple images
        // associated with this post.
        $post['image_url'] = isset($post['subattachments']) ? $post['subattachments']['data'][0]['media']['image']['src'] : $post['media']['image']['src'];
        $post['image_url'] = str_ireplace(array('http:', 'https:'), '', $post['image_url']);
        $post['post_url'] = str_ireplace(array('http:', 'https:'), '', $post['url']);
        // We need to add type of feed and created timestamp to the post data.
        $post['timestamp'] = strtotime($post['created_time']);
        $post['type'] = 'facebook';
        $data[] = $post;
      }
    }
    return $data;
  }

  /**
   * Standardize Instagram data.
   *
   * @return array
   */
  protected function set_inst_data() {
    $data = array();
    $img_type = variable_get('social_media_feeds_inst_image_type', 'low_resolution');
    $inst = $this->get_instagram_data();
    if ($inst) {
      foreach ($inst as $post) {
        // Save image url and post url without the protocol
        $post['image_url'] = str_ireplace(array('http:', 'https:'), '', $post['images'][$img_type]['url']);
        $post['post_url'] = str_ireplace(array('http:', 'https:'), '', $post['link']);
        // We need to add type of feed and created timestamp to the post data.
        $post['timestamp'] = $post['created_time'];
        $post['type'] = 'instagram';
        $data[] = $post;
      }
    }
    return $data;
  }

  /**
   * Public function to return unedited Instagram data.
   *
   * @return array|bool|mixed
   */
  public function get_instagram_data() {
    $username = check_plain(variable_get('social_media_feeds_instagram_username', FALSE));

    if (!$username) {
      return FALSE;
    }
    else {
      $access_token = check_plain(variable_get('social_media_feeds_instagram_access_token'));
      $count = check_plain(variable_get('social_media_feeds_instagram_count', 8));

      // Try the 'correct' way first with access_token.
      $url = "https://api.instagram.com/v1/users/self/media/recent/?access_token=$access_token&count=$count";
      $request = drupal_http_request($url);
      $data = drupal_json_decode($request->data);
      if ($data['meta']['code'] === 200) {
        $data = $data['data'];
      }
      // Provide a fallback in case the access_token is expired
      // TODO Check this after June 2016 to make sure it does not get deprecated.
      else {
        $request = drupal_http_request("https://www.instagram.com/$username/media/?size=t");
        $data = drupal_json_decode($request->data);
        $data = array_slice($data['items'], 0, $count);
        // TODO Notify that access_token is expired or there is another issue.
      }
      return $data;
    }
  }

  /**
   * Public function to retrieve unedited Facebook data.
   *
   * @return bool|mixed
   */
  public function get_facebook_data() {
    // Let's grab the site name from the config form.
    $sitename = check_plain(variable_get('social_media_feeds_fb_sitename', FALSE));

    // If there is no site name we do not need to import this type.
    if (!$sitename) {
      return FALSE;
    }
    else {
      // Let's grab the rest of the settings from the config form.
      $app_id = check_plain(variable_get('social_media_feeds_fb_app_id', FALSE));
      $secret_key = check_plain(variable_get('social_media_feeds_fb_secret_key', FALSE));
      $count = check_plain(variable_get('social_media_feeds_fb_count', 8));
      $fields =  check_plain(variable_get('social_media_feeds_fb_fields', 'attachments,created_time'));

      // Grab facebook data.
      $url = "https://graph.facebook.com/$sitename/posts?&limit=$count&fields=$fields&access_token=$app_id|$secret_key";
      $request = drupal_http_request($url);
      $data = drupal_json_decode($request->data);
      $data =  $data['data'];
      return $data;
    }
  }

  /**
   * Public function to retrieve unedited Twitter data.
   *
   * @return bool|mixed
   */
  public function get_twitter_data() {
    // Let's grab the username from the config form.
    $twitter_username = check_plain(variable_get('social_media_feeds_twitter_username', FALSE));

    if (!$twitter_username) {
      return FALSE;
    }
    else {
      // Let's grab the rest of the settings.
      $twitter_consumer_key = variable_get('social_media_feeds_twitter_app_id');
      $twitter_consumer_secrete = variable_get('social_media_feeds_twitter_secret_key');
      $data_count = variable_get('social_media_feeds_twitter_count', 8);
      $only_images = variable_get('social_media_feeds_twitter_only_images', 1);

      // Import code below copied from socialfeed module https://www.drupal.org/project/socialfeed.
      // Auth Parameters.
      $api_key = urlencode($twitter_consumer_key);
      $api_secret = urlencode($twitter_consumer_secrete);
      $auth_url = 'https://api.twitter.com/oauth2/token';

      // What we want?
      $data_username = $twitter_username;
      $data_url = 'https://api.twitter.com/1.1/statuses/user_timeline.json';

      // Get API Access Token.
      $api_credentials = base64_encode($api_key . ':' . $api_secret);
      $auth_headers = 'Authorization: Basic ' . $api_credentials . "\r\n" . 'Content-Type: application/x-www-form-urlencoded;charset=UTF-8' . "\r\n";
      $auth_context = stream_context_create(
        array(
          'http' => array(
            'header' => $auth_headers,
            'method' => 'POST',
            'content' => http_build_query(
              array(
                'grant_type' => 'client_credentials',
              )
            ),
          ),
        )
      );
      $auth_response = json_decode(file_get_contents($auth_url, 0, $auth_context), TRUE);
      $auth_token = $auth_response['access_token'];

      // Get Tweets.
      $data_context = stream_context_create(
        array(
          'http' => array(
            'header' => 'Authorization: Bearer ' . $auth_token . "\r\n",
          ),
        )
      );
      $extra = $only_images ? '&filter=images&include_entities=true' : '';
      $data = json_decode(file_get_contents($data_url . '?count=' . $data_count . '&screen_name=' . urlencode($data_username) . $extra, 0, $data_context), TRUE);
      // End socialfeed code.

      return $data;
    }
  }
}