<?php
class MultiSafepay_API_Object_Issuers extends MultiSafepay_API_Object_Core {
    
    public $success;
    public $data;
    
    public function get(){
        
        $result = parent::get('issuers', 'ideal');
        $this->success= $result->succes;
        $this->data= $result->data;
        
        return $this->data;
    }
}