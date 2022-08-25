<p align="center">
  <img src="https://www.multisafepay.com/img/multisafepaylogo.svg" width="400px" position="center">
</p>

# MultiSafepay plugin for Drupal Commerce

Easily integrate MultiSafepay payment solutions into your Drupal Commerce webshop with our free plugin.

[![Latest stable version](https://img.shields.io/github/release/multisafepay/Drupal-Commerce.svg)](https://github.com/MultiSafepay/Drupal-Commerce/releases)

## About MultiSafepay

MultiSafepay is a collecting payment service provider, which means we take care of electronic contracts, technical details, and payment collection for each payment method. You can start selling online today and manage all your transactions in one place.

## Supported payment methods

See MultiSafepay Docs – [PrestaShop 1.6](https://docs.multisafepay.com/docs/prestashop-1-6) > Payment methods.

## Prerequisites

- You will need a [MultiSafepay account](https://testmerchant.multisafepay.com/signup). Consider a test account first.
- Drupal Commerce
- Drupal 7

## Installation and configuration

1. Unpack the contents of the .zip archive and upload the **Modules** folder via SFTP to the PrestaShop root directoy, merging the two folders.
2. Sign in to your PrestaShop 1.6 backend.
3. Go to **Modules and services** > **Payments and gateways**.
    **Note:** You must install and configure the MultiSafepay Core Module (MultiSafepay) because all payment methods require certain settings and/or the API key in the core module.
4. In the next screen, proceed with the installation.
5. Enter your [API key](https://docs.multisafepay.com/docs/sites#site-id-api-key-and-security-code), and then click **Save**.
6. On the **Payments** tab, enable the relevant payment methods.

For more information, see PrestaShop 1.6](https://docs.multisafepay.com/docs/prestashop-1-6).

## Rules

On installation of the main module, two additonal rules are installed (but not enabled):

- MultiSafepay order paid in full: Order state to processing
- MultiSafepay order complete: Shipped at MultiSafepay

When the first one is enabled, the order state changes to processing once the order has been paid in full. The second rule updates the MultiSafepay order status to trigger invoicing for BNPL orders.
 
## Support

- Create an issue on this repository. 
- Email <a href="mailto:integration@multisafepay.com">integration@multisafepay.com</a>

## Want to be part of the team?

Are you a developer interested in working at MultiSafepay? Check out our [job openings](https://www.multisafepay.com/careers/#jobopenings) and feel free to get in touch!
