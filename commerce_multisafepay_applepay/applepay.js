(function ($) {
  $( document ).ready(function() {
    var applePayBlock = $('#edit-commerce-payment-payment-method-commerce-multisafepay-applepaycommerce-payment-commerce-multisafepay-applepay').parent().first();
    applePayBlock.css('display', 'none');


    try {
      if (window.ApplePaySession && window.ApplePaySession.canMakePayments()) {
        applePayBlock.css('display', 'block')
      }
    } catch (error) {
      console.warn('MultiSafepay error when trying to initialize Apple Pay:', error);
    }

  });
})(jQuery);
