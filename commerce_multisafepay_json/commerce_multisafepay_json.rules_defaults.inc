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

$rule_complete_to_shipped = '{ "rules_commerce_multisafepay_order_complete_multisafepay_shipped" : {
    "LABEL" : "MultiSafepay Order Complete: Shipped at MultiSafepay",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "ACTIVE" : false,
    "WEIGHT" : "1",    
    "REQUIRES" : [ "rules", "commerce_multisafepay_json", "entity" ],
    "ON" : { "commerce_order_update" : [] },
    "IF" : [
      { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "completed" } },
      { "AND" : [] },
      { "NOT data_is" : {
          "data" : [ "commerce-order-unchanged:status" ],
          "value" : [ "commerce-order:status" ]
        }
      }
    ],
    "DO" : [ { "commerce_multisafepay_json_ship_order_action" : { "order" : [ "commerce-order" ] } } ]
  }
}';
  
  $rules['rules_multisafepay_order_paid_in_full_order_state_to_processing'] = rules_import($rule_paid_to_processing);
  $rules['rules_commerce_multisafepay_order_complete_multisafepay_shipped'] = rules_import($rule_complete_to_shipped);
  
  return $rules;
}