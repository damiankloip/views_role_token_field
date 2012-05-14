<?php

/**
 * @file
 * role_filter_field views handler class.
 */

class views_role_token_field_handler_field extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['role_tokens'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['views_role_token_field'] = array(
      '#type' => 'fieldset',
      '#title' => t("Role filter settings"),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['role_tokens'] = array(
      '#title' => t('User roles'),
      '#type' => 'fieldset',
      '#description' => t('Select a field token to use for each user role.'),
      '#fieldset' => 'views_role_token_field',
      '#tree' => TRUE,
    );

    foreach (user_roles() as $rid => $name) {
      $default = !empty($this->options['role_tokens'][$rid]) ? $this->options['role_tokens'][$rid] : '';
      $form['role_tokens'][$rid] = array(
        '#title' => check_plain($name),
        '#type' => 'textfield',
        '#size' => 30,
        '#default_value' => $default,
      );
    }

    $form['available_tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#value' => $this->get_available_field_tokens(),
      '#fieldset' => 'views_role_token_field',
    );

  }

  // Don't need anything in the query.
  function query() {}

  function render($values) {
    global $user;

    // Get user role rids for current user.
    $user_roles = array_keys($user->roles);
    // Get options/tokens for rids.
    $role_tokens = $this->options['role_tokens'];

    if (!empty($role_tokens)) {
      $output = array();
      // Filter first so we don't iterate over empty tokens.
      foreach (array_filter($role_tokens) as $rid => $token) {
        if (in_array($rid, $user_roles)) {
          $field = trim(trim($token), '[]');
          // If the field exists.
          if (isset($this->view->field[$field])) {
            $value = $this->view->field[$field]->last_render;
          }
          // Just use the value (could be used for static values).
          else {
            $value = filter_xss_admin($field);
          }
          // Add to the array of output values and wrap.
          $output[] = '<div class="views-role-token-field-' . $rid . '">' . $value . '</div>';
        }
      }
    }

    return implode($output); 
  }

  /**
   * Get a rendered list of only field tokens.
   */
  function get_available_field_tokens() {
    // Get a list of the available fields and arguments for token replacement.
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
      $options[t('Fields')]["[$field]"] = $handler->ui_name();
      // We only use fields up to (and including) this one.
      if ($field == $this->options['id']) {
        break;
      }
    }

    $this->document_self_tokens($options[t('Fields')]);

    // Default text.
    $output = t('<p>You must add some additional fields to this display before using this field. These fields may be marked as <em>Exclude from display</em> if you prefer. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
    // We have some options, so make a list.
    if (!empty($options)) {
      $output = t('<p>The following tokens are available for this field. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.
        If you would like to have the characters \'[\' and \']\' please use the html entity codes \'%5B\' or  \'%5D\' or they will get replaced with empty space.</p>');
      foreach (array_keys($options) as $type) {
        if (!empty($options[$type])) {
          $items = array();
          foreach ($options[$type] as $key => $value) {
            $items[] = $key . ' == ' . $value;
          }
          $output .= theme('item_list',
            array(
              'items' => $items,
              'type' => $type
            ));
        }
      }
    }

    return $output;
  }

} // views_role_token_field_handler_field.
