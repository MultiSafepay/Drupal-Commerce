<?php

/**
 * @file
 * Administrative forms for the MultiSafepay payment gateway module.
 */

/**
 * Form callback: allows the user to refund a credit on a prior transaction.
 */
function commerce_multisafepay_refund_form($form, &$form_state, $order, $transaction) {
    $form_state['order'] = $order;
    $form_state['transaction'] = $transaction;
    $form_state['payment_method'] = commerce_payment_method_instance_load($transaction->instance_id);


    $transaction_statuses = commerce_payment_transaction_statuses();

    foreach (commerce_payment_transaction_load_multiple(array(), array('order_id' => $order->order_id)) as $transaction) {
        // If the payment transaction status indicates it should include the
        // current transaction in the total...
        if ($transaction_statuses[$transaction->status]['total']) {
            // Add the transaction to its currency's running total if it exists...
            if (isset($totals[$transaction->currency_code])) {
                $totals[$transaction->currency_code] += $transaction->amount;
            } else {
                // Or begin a new running total for the currency.
                $totals[$transaction->currency_code] = $transaction->amount;
            }
        }
    }


    $default_amount = commerce_currency_amount_to_decimal($totals[$transaction->currency_code], $transaction->currency_code);

    $form['amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Refund amount'),
        '#description' => t('Enter the amount to be refunded.'),
        '#default_value' => $default_amount,
        '#field_suffix' => check_plain($transaction->currency_code),
        '#size' => 16,
    );

    $form = confirm_form($form, t('What amount do you want to refund?'), 'admin/commerce/orders/' . $order->order_id . '/payment', '', t('Refund'), t('Cancel'), 'confirm'
    );

    return $form;
}

/**
 * Validate handler: check the credit amount before attempting a refund request.
 */
function commerce_multisafepay_refund_form_validate($form, &$form_state) {
    $transaction = $form_state['transaction'];
    $amount = $form_state['values']['amount'];
    
    $order_int = $form_state['order'];
    
    $transaction_statuses = commerce_payment_transaction_statuses();
    

    foreach (commerce_payment_transaction_load_multiple(array(), array('order_id' => $order_int->order_id)) as $transaction) {
        // If the payment transaction status indicates it should include the
        // current transaction in the total...
        if ($transaction_statuses[$transaction->status]['total']) {
            // Add the transaction to its currency's running total if it exists...
            if (isset($totals[$transaction->currency_code])) {
                $totals[$transaction->currency_code] += $transaction->amount;
            } else {
                // Or begin a new running total for the currency.
                $totals[$transaction->currency_code] = $transaction->amount;
            }
        }
    }
    
    $available_amount = commerce_currency_amount_to_decimal($totals[$transaction->currency_code], $transaction->currency_code);

    

    // Ensure a positive numeric amount has been entered for refund.
    if (!is_numeric($amount) || $amount <= 0) {
        form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
    }

    // Ensure the amount is less than or equal to the captured amount.
    if ($amount > $available_amount) {
        form_set_error('amount', t('You cannot refund more then the remaining available fund of the origional transaction.'));
    }

    // If the transaction is older than 60 days, display an error message and
    // redirect.
    if (time() - $transaction->created > 86400 * 60) {
        drupal_set_message(t('This transaction has passed its 60 day limit for issuing refunds.'), 'error');
        drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
    }
}

/**
 * Submit handler: process a refund request.
 */
function commerce_multisafepay_refund_form_submit($form, &$form_state) {
    $msp = new API_Client;
    $payment_method = $form_state['payment_method'];
    
    if (!$payment_method['settings']['multisafepay_environment']) {
        $msp->setApiKey($payment_method['settings']['multisafepay_live_api_key']);
        $msp->setApiUrl('http://devapi.multisafepay.com/v1/json/');
    } else {
        $msp->setApiKey($payment_method['settings']['multisafepay_test_api_key']);
        $msp->setApiUrl('http://devapi.multisafepay.com/v1/json/');
    }

    $order_int = $form_state['order'];
    $transaction = $form_state['transaction'];
    $amount = $form_state['values']['amount'] * 100;

    $endpoint = 'orders/' . $transaction->transaction_id . '/refunds';
    try {
        $order = $msp->orders->post(array(
            "type" => "refund",
            "amount" => $amount,
            "currency" => $transaction->currency_code,
            "description" => "Refund for order: " . $transaction->order_id,
                ), $endpoint);
    } catch (MultiSafepay_API_Exception $e) {
        drupal_set_message(t('Refund failed. @reason', array('@reason' => $e->getMessage())), 'error');
    }

    // Create a new transaction to record the credit.
    $credit_transaction = commerce_payment_transaction_new($payment_method['method_id'], $order_int->order_id);
    $credit_transaction->instance_id = $payment_method['instance_id'];
    $credit_transaction->remote_id = $order->transaction_id;
    $credit_transaction->amount = ($amount * -1);
    $credit_transaction->currency_code = $transaction->currency_code;
    $credit_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $credit_transaction->remote_status = 'refunded';
    $credit_transaction->message = t('Transaction (partially) refunded');

    // Save the credit transaction.
    commerce_payment_transaction_save($credit_transaction);
    drupal_set_message(t('Refund for @amount issued successfully.', array('@amount' => commerce_currency_format(($amount), $transaction->currency_code))));

    // Save the updated original transaction.
    commerce_payment_transaction_save($transaction);

    $form_state['redirect'] = 'admin/commerce/orders/' . $order_int->order_id . '/payment';
}
