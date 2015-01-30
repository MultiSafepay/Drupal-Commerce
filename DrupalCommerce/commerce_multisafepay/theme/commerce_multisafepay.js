jQuery(document).ready(function($){
if($('input[name="commerce_payment\\[payment_details\\]\\[multisafepay_gateway\\]"]:checked').val() == 'IDEAL'){
		$('#edit-commerce-payment-payment-details-issuer').show(500);
		$('#issuer').show(500);
	}
	$('input[name="commerce_payment\\[payment_details\\]\\[multisafepay_gateway\\]"]').change(function(){
	if($('input[name="commerce_payment\\[payment_details\\]\\[multisafepay_gateway\\]"]:checked').val() == 'IDEAL'){
		$('#edit-commerce-payment-payment-details-issuer').show(500);
		$('#issuer').show(500);
	}else{
		$('#edit-commerce-payment-payment-details-issuer').hide(500);
		$('#issuer').hide(500);
	}
	
	});
	
});