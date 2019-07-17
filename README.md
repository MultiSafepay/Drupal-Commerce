<p align="center">
  <img src="https://www.multisafepay.com/img/multisafepaylogo.svg" width="400px" position="center">
</p>

# MultiSafepay plugin for Drupal Commerce

Easily integrate MultiSafepay payment solutions into your Drupal Commerce webshop with the free MultiSafepay Drupal Commerce plugin.

[![Latest Stable Version](https://img.shields.io/github/release/multisafepay/Drupal-Commerce.svg)](https://github.com/MultiSafepay/Drupal-Commerce/releases)

## About MultiSafepay
MultiSafepay is a collecting payment service provider which means we take care of the agreements, technical details and payment collection required for each payment method. You can start selling online today and manage all your transactions from one place.
## Supported Payment Methods
The supported Payment Methods & Gift cards for this plugin can be found over here: [Payment Methods & Gift cards](https://docs.multisafepay.com/plugins/drupal7/faq/#available-payment-methods-in-drupal7)

## Requirements
- To use the plugin you need a MultiSafepay account. You can create a test account on https://testmerchant.multisafepay.com/signup
- Drupal Commerce
- Drupal 7

## Installation
Unpack the content of the .ZIP file in the root of your webshop.

For additional information or instructions please see our [installation & configuration manual](https://docs.multisafepay.com/plugins/drupal7/manual/)

## Rules
On installation of the main module, two additonal rules are installed (but not enabled):

1. MultiSafepay Order paid in full: Order state to processing
2. MultiSafepay Order Complete: Shipped at MultiSafepay

When the first one is enabled, the order state will be set to processing once the order has been paid in full. The second rule will update the order status of the transaction at MultiSafepay. This will activate the Pay After Delivery/Klarna/E-invoice invoicing process.
 
## Support
You can create issues on our repository. If you need any additional help or support, please contact <a href="mailto:techsupport@multisafepay.com">techsupport@multisafepay.com</a>
