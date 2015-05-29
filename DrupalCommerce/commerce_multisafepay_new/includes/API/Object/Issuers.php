<?php
class API_Object_Issuers extends API_Object_Core {
    
    public $success;
    public $data;
    
    public function get(){
        
        $result = parent::get('issuers', 'ideal');
        $this->success= $result->success;
        $this->data= $result->data;
        
        return $this->data;
    }
}