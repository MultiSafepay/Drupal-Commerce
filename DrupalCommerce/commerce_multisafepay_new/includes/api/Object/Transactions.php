<?php
class MultiSafepay_API_Object_Transactions extends MultiSafepay_API_Object_Core{
    
    public $success;
    public $data;
    
    public function patch($body, $endpoint) {
        $result = parent::patch(json_encode($body), $endpoint);
        $this->success = $result->succes;
        $this->data = $result->data;
        return $this->data;
    }
    
    public function get($id) {
        $result = parent::get('transactions', $id);
        $this->success = $result->succes;
        $this->data = $result->data;
        return $this->data;
    }
    
}