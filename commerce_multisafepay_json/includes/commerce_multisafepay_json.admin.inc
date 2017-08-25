<?php

/**
 *
 * DISCLAIMER
 *
 * Do not edit or add to this file if you wish to upgrade the MultiSafepay plugin
 * to newer versions in the future. If you wish to customize the plugin for your
 * needs please document your changes and make backups before you update.
 *
 * @category MultiSafepay
 * @package Connect
 * @author TechSupport <techsupport@multisafepay.com>
 * @copyright Copyright (c) 2017 MultiSafepay, Inc. (http://www.multisafepay.com)
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/**
 * @file
 * Administrative forms for the MultiSafepay payment gateway module.
 */

/**
 * Form callback: allows the user to refund a credit on a prior transaction.
 */
function commerce_multisafepay_json_refund_form($form, &$form_state, $order, $transaction)
{
    $form_state['order'] = $order;
    $form_state['transaction'] = $transaction;
    $form_state['payment_method'] = commerce_payment_method_instance_load($transaction->instance_id);

    $totals = array();
    $totals['captured_total'] = 0;
    $transaction_statuses = commerce_payment_transaction_statuses();

    foreach (commerce_payment_transaction_load_multiple(array(), array(
        'order_id' => $order->order_id
    )) as $transaction) {
        if ($transaction_statuses[$transaction->status]['total']) {
            if (isset($totals[$transaction->currency_code])) {
                $totals[$transaction->currency_code] += $transaction->amount;
            } else {
                $totals[$transaction->currency_code] = $transaction->amount;
                if ($transaction->amount >= 0) {
                    $totals['captured_total'] += $transaction->amount;
                }
            }
        }
    }

    $orig_amount = commerce_currency_amount_to_decimal($totals['captured_total'], $transaction->currency_code);
    $default_amount = commerce_currency_amount_to_decimal($totals[$transaction->currency_code], $transaction->currency_code);

    $form['amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Refund amount'),
        '#description' => t('Enter the amount to be refunded. Origional transaction amount: <b>') . $transaction->currency_code . ' ' . $orig_amount . t('</b>. Maximum amount available for refunding: <b>') . $transaction->currency_code . ' ' . $default_amount . '</b>',
        '#default_value' => $default_amount,
        '#field_suffix' => check_plain($transaction->currency_code),
        '#size' => 16
    );

    $form = confirm_form($form, t('What amount do you want to refund?'), 'admin/commerce/orders/' . $order->order_id . '/payment', '', t('Refund'), t('Cancel'), 'confirm');

    return $form;
}

/**
 * Validate handler: check the credit amount before attempting a refund request.
 */
function commerce_multisafepay_json_refund_form_validate($form, &$form_state)
{
    $transaction = $form_state['transaction'];
    $amount = $form_state['values']['amount'];
    $order_int = $form_state['order'];
    $transaction_statuses = commerce_payment_transaction_statuses();

    foreach (commerce_payment_transaction_load_multiple(array(), array(
        'order_id' => $order_int->order_id
    )) as $transaction) {
        if ($transaction_statuses[$transaction->status]['total']) {
            if (isset($totals[$transaction->currency_code])) {
                $totals[$transaction->currency_code] += $transaction->amount;
            } else {
                $totals[$transaction->currency_code] = $transaction->amount;
            }
        }
    }

    $available_amount = commerce_currency_amount_to_decimal($totals[$transaction->currency_code], $transaction->currency_code);

    if (!is_numeric($amount) || $amount <= 0) {
        form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
    }

    if ($amount > $available_amount) {
        form_set_error('amount', t('You cannot refund more then the remaining available fund of the origional transaction.'));
    }

    if (time() - $transaction->created > 86400 * 60) {
        drupal_set_message(t('This transaction has passed its 60 day limit for issuing refunds.'), 'error');
        drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
    }
}

/**
 * Submit handler: process a refund request.
 */
function commerce_multisafepay_json_refund_form_submit($form, &$form_state)
{
    $msp = new API_Client;
    $payment_method = $form_state['payment_method'];
    //$payment = commerce_payment_method_instance_load('commerce_multisafepay_json|commerce_payment_commerce_multisafepay_json');
    $payment_api_rule = 'commerce_multisafepay_json|commerce_payment_commerce_multisafepay_json';
    drupal_alter('multisafepay_api_rule', $payment_api_rule, $order);
    $payment = commerce_payment_method_instance_load($payment_api_rule);

    $payment_method['settings'] +=$payment['settings'];

    if (!$payment_method['settings']['multisafepay_environment']) {
        $msp->setApiKey($payment_method['settings']['multisafepay_live_api_key']);
        $msp->setApiUrl('https://api.multisafepay.com/v1/json/');
    } else {
        $msp->setApiKey($payment_method['settings']['multisafepay_test_api_key']);
        $msp->setApiUrl('https://testapi.multisafepay.com/v1/json/');
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
            "description" => "Refund for order: " . $transaction->order_id
                ), $endpoint);
    } catch (MultiSafepay_API_Exception $e) {
        drupal_set_message(t('Refund failed. @reason', array(
            '@reason' => $e->getMessage()
                )), 'error');
        watchdog('MultiSafepay', 'Refund failed. @reason', array('@reason' => htmlspecialchars($e->getMessage())), WATCHDOG_ERROR);
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

    commerce_payment_transaction_save($credit_transaction);
    drupal_set_message(t('Refund for @amount issued successfully.', array(
        '@amount' => commerce_currency_format(($amount), $transaction->currency_code)
    )));

    commerce_payment_transaction_save($transaction);
    $form_state['redirect'] = 'admin/commerce/orders/' . $order_int->order_id . '/payment';
}

function commerce_multisafepay_json_shipped_form($form, &$form_state, $order, $transaction)
{
    $form_state['order'] = $order;
    $form_state['transaction'] = $transaction;
    $form_state['payment_method'] = commerce_payment_method_instance_load($transaction->instance_id);

    $msp = new API_Client;

    $payment_method = $payment_method = commerce_payment_method_instance_load('commerce_multisafepay_json|commerce_payment_commerce_multisafepay_json');

    if (!$payment_method['settings']['multisafepay_environment']) {
        $msp->setApiKey($payment_method['settings']['multisafepay_live_api_key']);
        $msp->setApiUrl('https://api.multisafepay.com/v1/json/');
    } else {
        $msp->setApiKey($payment_method['settings']['multisafepay_test_api_key']);
        $msp->setApiUrl('https://testapi.multisafepay.com/v1/json/');
    }

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
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
}
