<?php

/**
 * @file
 * Views integration for the role_filter_field module.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_role_token_field_views_data_alter(&$data) {
  // Add our handler to the global views array.
  $data['views']['views_role_token_field'] = array(
    'title' => t('Role token field'),
    'help' => t('Use field tokens to rewrite the output of this field, based on the users role.'),
    'field' => array(
      'handler' => 'views_role_token_field_handler_field',
    ),
  );
}
