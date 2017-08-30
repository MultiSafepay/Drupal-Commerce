COMMERCE MULTISAFEPAY JSON
----------------

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Implementation
 * Integration (with other modules)
 * Troubleshooting (known Theme issues)
 * Contributions are welcome!!



INTRODUCTION
------------

This is the new official MultiSafepay Commerce plugin using the 
MultiSafepay JSON REST API.

It offers all supported MultiSafepay Payment methods within your Drupal 
Commerce store. When offering iDEAL as a payment method, the customer can select
the iDEAL issuer from within your store.

This module also supports the MultiSafepay Refund API call so that refunds can 
be done from within the Drupal Commerce transaction of an order. 


FEATURES
--------

The new Commerce MultiSafepay plugins use the new JSON API and has support for
1. Adding separate payment methods to Commerce checkout
2. Pay After Delivery
3. Direct iDEAL (Select the issuer from within your drupal commerce store)
4. Refund API
5. Shipped API 



IMPLEMENTATION
--------------

The plugins can be installed by uploading them to your modules folder. When
uploaded the plugins can be installed from within Drupal. All plugins depend on
the main commerce_multisafepay_json plugin. That is the main plugin and must
always be configured.

RULES
--------------

On installation of the main module, two additonal rules are installed (but not enabled):
1. MultiSafepay Order paid in full: Order state to processing
2. MultiSafepay Order Complete: Shipped at MultiSafepay

When the first one is enabled, the order state will be set to processing once the order has been paid in full.
The second rule will update the order status of the transaction at MultiSafepay. This will activate the Pay After Delivery/Klarna/E-invoice invoicing process.



INTEGRATION (WITH OTHER MODULES)
--------------------------------

The plugins have support for:
1. Commerce_fees
2. Commerce_discounts
3. Commerce_coupons


TROUBLESHOOTING (KNOWN THEME ISSUES)
------------------------------------

Whenever you have issues or found a bug then please contact us on the drupal
project page or by contacting us by e-mailing techsupport@multisafepay.com.



CONTRIBUTIONS ARE WELCOME!!
---------------------------

Feel free to follow up in the issue queue for any contributions, bug
reports, feature requests.
Tests, feedback or comments in general are highly appreciated.
