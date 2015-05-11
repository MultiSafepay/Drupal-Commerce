<?php
class MultiSafepay_API_Object_Gateways extends MultiSafepay_API_Object_Core {
    
    public $success;
    public $data;
    
    public function get($body = array(), $query_string){
        $result = parent::get('gateways', '', json_encode($body), $query_string );
        $this->success= $result->succes;
        $this->data= $result->data;
        
        return $this->data;
    }
}


