<?php

/**
 * @file
 *
 * Configuration page and associated functions.
 */

/**
 * Custom form submit
 *
 * @param $form
 * @param $form_state
 */
function _social_media_feeds_config_form_submit($form, $form_state) {
  foreach ($form_state['values'] as $name => $field) {
    if (stripos($name, 'social_media_feeds') !== FALSE && $field !== '') {
      variable_set($name, check_plain($field));
    }
  }
}

function _social_media_feeds_config_form() {
  $form = array();
  // Facebook section.
  $form['facebook'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Facebook'),
  );
  $form['facebook']['social_media_feeds_fb_sitename'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Name'),
    '#default_value' => variable_get('social_media_feeds_fb_sitename', ''),
  );
  $form['facebook']['social_media_feeds_fb_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#default_value' => variable_get('social_media_feeds_fb_app_id', ''),
  );
  $form['facebook']['social_media_feeds_fb_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('social_media_feeds_fb_secret_key', ''),
  );
  $form['facebook']['social_media_feeds_fb_fields'] = array(
    '#type' => 'textfield',
    '#title' => t('Fields'),
    '#description' => t('Fields to import. See https://developers.facebook.com/docs/graph-api/reference/v2.6/post'),
    '#default_value' => variable_get('social_media_feeds_fb_fields', ''),
  );
  $form['facebook']['social_media_feeds_fb_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Posts'),
    '#default_value' => variable_get('social_media_feeds_fb_count', ''),
  );
  // Twitter section.
  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Twitter'),
  );
  $form['twitter']['social_media_feeds_twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('User Name'),
    '#default_value' => variable_get('social_media_feeds_twitter_username', ''),
  );
  $form['twitter']['social_media_feeds_twitter_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Key'),
    '#default_value' => variable_get('social_media_feeds_twitter_app_id', ''),
  );
  $form['twitter']['social_media_feeds_twitter_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('social_media_feeds_twitter_secret_key', ''),
  );
  $form['twitter']['social_media_feeds_twitter_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Posts'),
    '#default_value' => variable_get('social_media_feeds_twitter_count', ''),
  );
  // Instagram section.
  $form['instagram'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Instagram'),
  );
  $form['instagram']['social_media_feeds_instagram_username'] = array(
    '#type' => 'textfield',
    '#title' => t('User Name'),
    '#default_value' => variable_get('social_media_feeds_instagram_username', ''),
  );
  $form['instagram']['social_media_feeds_instagram_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('social_media_feeds_instagram_client_id', ''),
  );
  $form['instagram']['social_media_feeds_instagram_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('social_media_feeds_instagram_secret_key', ''),
  );
  // TODO Create automated process for updating access_token.
  $form['instagram']['social_media_feeds_instagram_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Token'),
    '#default_value' => variable_get('social_media_feeds_instagram_access_token', ''),
  );
  $form['instagram']['social_media_feeds_instagram_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Posts'),
    '#default_value' => variable_get('social_media_feeds_instagram_count', ''),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
    '#submit' => array('_social_media_feeds_config_form_submit'),
  );
  return $form;
}