<?php

class MultiSafepay_API_Client {

    public $orders;
    public $issuers;
    public $transactions;
    public $gateways;
    protected $api_key;
    public $api_url;
    public $api_endpoint;

    
    public function __construct() {
        $this->orders = new MultiSafepay_API_Object_Orders($this);
        $this->issuers = new MultiSafepay_API_Object_Issuers($this);
        $this->gateways = new MultiSafepay_API_Object_Gateways($this);
        $this->transactions = new MultiSafepay_API_Object_Transactions($this);
    }
    

    public function setApiUrl($url) {
        $this->api_url = trim($url);
    }
    

    public function setApiKey($api_key) {
        $this->api_key = trim($api_key);
    }
    

    public function performApiCall($http_method, $api_method, $http_body = NULL) {
        if (empty($this->api_key)) {
            throw new MultiSafepay_API_Exception("Please set your MultiSafepay API Key");
        }

        $url = $this->api_url . $api_method;
        $ch = curl_init($url);
        
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        curl_setopt($ch, CURLOPT_ENCODING, "");
        curl_setopt($ch, CURLOPT_TIMEOUT, 10);

        $request_headers = array(
            "Accept: application/json",
            "api_key:" . $this->api_key,
        );

        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $http_method);

        if ($http_body !== NULL) {
            $request_headers[] = "Content-Type: application/json";
            curl_setopt($ch, CURLOPT_POST, 1);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $http_body);
        }

        curl_setopt($ch, CURLOPT_HTTPHEADER, $request_headers);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, TRUE);

        $body = curl_exec($ch);

        if (curl_errno($ch)) {
            throw new MultiSafepay_API_Exception("Unable to communicate with MultiSafepay(" . curl_errno($ch) . "): " . curl_error($ch) . ".");
        }
        return $body;
    }
}