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

  $default_amount = commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code);

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Enter the amount to be refunded.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to refund?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Refund'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: check the credit amount before attempting a refund request.
 */
function commerce_multisafepay_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for refund.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you captured.'));
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
  $order = $form_state['order'];
  $transaction = $form_state['transaction'];
  $payment_method = $form_state['payment_method'];

  // Convert the amout to the commerce format.
  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);

  $charge = array(
    'amount' => commerce_moneybookers_price_amount($amount, $transaction->currency_code),
    'currency' => $transaction->currency_code,
  );

  // Build a name-value pair array for this transaction.
  /*$data = commerce_multisafepay_request_refund($order, $transaction, $charge);

  // Submit the capture request to Moneybookers.
  $response = commerce_multisafepay_api_request($payment_method, $data);

  if (!isset($response['PROCESSING.RESULT']) || $response['PROCESSING.RESULT'] != 'ACK' || $response['PROCESSING.CODE'] != 'CC.RF.90.00') {
    // Display an error message but leave the transaction pending.
    drupal_set_message(t('Refund failed. @reason', array('@reason' => check_plain($response['PROCESSING.RETURN']))), 'error');

    // Save the failure response message to the original transaction.
    $transaction->payload[REQUEST_TIME . '-refund'] = $response;
    commerce_payment_transaction_save($transaction);
  }
  else {
    // Create a new transaction to record the credit.
    $credit_transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
    $credit_transaction->instance_id = $payment_method['instance_id'];
    $credit_transaction->remote_id = $response['IDENTIFICATION.UNIQUEID'];
    $credit_transaction->amount = $amount * -1;
    $credit_transaction->currency_code = $transaction->currency_code;
    $credit_transaction->payload[REQUEST_TIME] = $response;
    $credit_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $credit_transaction->remote_status = $response['PROCESSING.CODE'];
    $credit_transaction->message = t('Refunded: @date', array('@date' => format_date(REQUEST_TIME, 'short')));

    // Save the credit transaction.
    commerce_payment_transaction_save($credit_transaction);

    drupal_set_message(t('Refund for @amount issued successfully.', array('@amount' => commerce_currency_format($amount, $transaction->currency_code))));
  }

  // Save the updated original transaction.
  commerce_payment_transaction_save($transaction);
*/
  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}
