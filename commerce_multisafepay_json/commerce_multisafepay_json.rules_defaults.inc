<?php
/**
* Implements hook_default_rules_configuration().
*/
function commerce_multisafepay_json_default_rules_configuration() {
  $rules = array();
  $rule_paid_to_processing = '{ "rules_multisafepay_order_paid_in_full_order_state_to_processing" : {
    "LABEL" : "MultiSafepay Order paid in full: Order state to processing",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "WEIGHT" : "1",
    "REQUIRES" : [ "commerce_cart", "commerce_order", "commerce_payment" ],
    "ON" : [ "commerce_payment_order_paid_in_full" ],
    "IF" : [
      { "NOT commerce_order_is_cart" : { "commerce_order" : [ "commerce_order" ] } }
    ],
    "DO" : [
      { "commerce_order_update_status" : { "commerce_order" : [ "commerce_order" ], "order_status" : "processing" } }
    ]
  }
}';
  
  $rules['rules_multisafepay_order_paid_in_full_order_state_to_processing'] = rules_import($rule_paid_to_processing);
  return $rules;
}