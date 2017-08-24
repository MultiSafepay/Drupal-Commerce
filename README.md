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
be done from within the Drupal Commerce transaction of an order. When offering 
Pay After Delivery you can also set the status to shipped at MultiSafepay, 
directly from within your Drupal Commerce store.


FEATURES
--------

The new Commerce MultiSafepay plugins use the new JSON API and have support for
1. Adding separate payment methods added to Commerce checkout
2. Pay After Delivery
3. Direct iDEAL (Select the issuer from within your drupal commerce store)
4. Refund API
5. Shipped API (Pay After Deliver)



IMPLEMENTATION
--------------

The plugins can be installed by uploading them to your modules folder. When
uploaded the plugins can be installed from within Drupal. All plugins depend on
the main commerce_multisafepay_json plugin. That is the main plugin and must
always be configured.


INTEGRATION (WITH OTHER MODULES)
--------------------------------

The plugins have support for:
1. Commerce_fees
2. Commerce_discounts
3. Commerce_coupons

These can be used in combination with Pay After Delivery, where all cart data
is need to create a transaction request at MultiSafepay.



TROUBLESHOOTING (KNOWN THEME ISSUES)
------------------------------------

Whenever you have issues or found a bug then please contact us on the drupal
project page or by contacting us by e-mailing techsupport@multisafepay.com.



CONTRIBUTIONS ARE WELCOME!!
---------------------------

Feel free to follow up in the issue queue for any contributions, bug
reports, feature requests.
Tests, feedback or comments in general are highly appreciated.
