<?php

class API_Object_Orders extends API_Object_Core {

    public $success;
    public $data;
    
    public function patch($body, $endpoint) {
        $result = parent::patch(json_encode($body), $endpoint);
        $this->success = $result->success;
        $this->data = $result->data;
        return $result;
    }

    
    public function get($id) {
        $result = parent::get('orders', $id);
        $this->success = $result->success;
        $this->data = $result->data;
        return $this->data;
    }

    public function post($body, $endpoint = 'orders') {
        $result = parent::post(json_encode($body), $endpoint);
        if(isset($result->succes)){
        $this->success = $result->success;
        }
        $this->data = $result->data;
        return $this->data;
    }


    public function getPaymentLink() {
        return $this->data->payment_url;
    }

}