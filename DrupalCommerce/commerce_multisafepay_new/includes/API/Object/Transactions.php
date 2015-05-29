<?php
class API_Object_Transactions extends API_Object_Core{
    
    public $success;
    public $data;
    
    public function patch($body, $endpoint) {
        $result = parent::patch(json_encode($body), $endpoint);
        $this->success = $result->success;
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