<?php

/**
 * Admin settings form builder.
 */
function islandora_data_admin_settings() {
  $form['islandora_data_solution_pack_ethercalc_host'] = array(
    '#title' => t('Ethercalc hostname'),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('islandora_data_solution_pack_ethercalc_host', 'http://example.com:8000'),
    '#description' => t("The hostname of your Ethercalc server. Include the port number."),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

