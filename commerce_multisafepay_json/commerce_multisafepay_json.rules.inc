<?php

require_once dirname(__FILE__) . "/includes/API/Autoloader.php";
 
/**
 * Implement hook_rules_action_info().
 */
function commerce_multisafepay_json_rules_action_info() {
  return array(
    'commerce_multisafepay_json_ship_order_action' => array(
      'label' => t('Set completed order to shipped at MultiSafepay'),
      'group' => t('MultiSafepay'),
      'parameter' => array(
        'order' => array(
            'type' => 'commerce_order',
            'label' => t('Order to be shipped'),
        ),
      ),
    ),
  );
}

function commerce_multisafepay_json_ship_order_action($order) {
    $transactions = commerce_payment_transaction_load_multiple(array(), array('order_id' =>  $order->order_id));
    $transaction = !empty($transactions) ? array_shift($transactions) : NULL;

    if ($transaction->payment_method == 'commerce_multisafepay_json') {
        $msp = new API_Client;
        
        $payment_api_rule = 'commerce_multisafepay_json|commerce_payment_commerce_multisafepay_json';
        drupal_alter('multisafepay_api_rule', $payment_api_rule, $order);
        $payment_method = commerce_payment_method_instance_load($payment_api_rule);
    
        if (!$payment_method['settings']['multisafepay_environment']) {
            $msp->setApiKey($payment_method['settings']['multisafepay_live_api_key']);
            $msp->setApiUrl('https://api.multisafepay.com/v1/json/');
        } else {
            $msp->setApiKey($payment_method['settings']['multisafepay_test_api_key']);
            $msp->setApiUrl('https://testapi.multisafepay.com/v1/json/');
        }
        $msp->setDebug($payment_method['settings']['multisafepay_debug']);
        
        $transactionid = $transaction->transaction_id;
        $endpoint = 'orders/' . $transactionid;
    
        try {
            $order = $msp->orders->patch(
                    array(
                "tracktrace_code" => '',
                "carrier" => '',
                "ship_date" => date('Y-m-d H:i:s'),
                "reason" => 'Shipped'
                    ), $endpoint);
        } catch (MultiSafepay_API_Exception $e) {
            drupal_set_message(t('Failed to set transaction to Shipped. @reason', array(
                '@reason' => $e->getMessage()
                    )), 'error');
    
            watchdog('MultiSafepay', 'Failed to set transaction to Shipped. @reason', array('@reason' => htmlspecialchars($e->getMessage())), WATCHDOG_ERROR);
        }    
    
        $transaction->remote_status = 'SHIPPED';
        
        commerce_payment_transaction_save($transaction);
    
        drupal_set_message(t('Transaction set to shipped at MultiSafepay', array()));
    }
}