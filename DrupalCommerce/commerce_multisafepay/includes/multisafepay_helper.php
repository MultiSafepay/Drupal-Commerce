<?php

//include_once 'api/MultiSafepay.combined.php';

class drupalcommerceMultisafepayHelper {
    
    private $_result 	= "multisafepay/result";
    private $_postback 	= "multisafepay/postback/initial";
    private $_version 	= "1.0.0";
    private $_api 		= null;
   

    public function getResultCode()
	{
        return $this->_result;
    }

    public function getPostbackCode()
	{
        return $this->_postback;
    }

    public function log($msg)
	{
        watchdog("multisafepay", $msg);
    }

    public function getResultURL()
	{
        global $base_url;
        return sprintf("%s/?q=%s",$base_url,$this->getResultCode());
    }

    public function getPostbackURL()
	{
        global $base_url;
        return sprintf("%s/?q=%s",$base_url,$this->getPostbackCode());
    }


    public function getInfo()
	{
        return '<table class="form">
            <tr>
              <td><img src="http://www.multisafepay.com/downloads/betaalbutton/msp/multisafepay-betaalveiligonline-280px.gif" alt="multisafepay" title="multisafepay" border="0"/></td>
              <td><div style="float: left;">
                <div style="border: 1px solid #D1CBC9; padding: 0px 6px 0px 6px; margin:6px;">
                    <p>Official Multisafepay payment module</p>
                </div>
            </div></td>
            </tr>
			<tr>
              <td>Module Version</td>
              <td>1.0.0</td>
            </tr>
            <tr>
              <td>Website</td>
              <td><a onclick="window.open(\'http://www.multisafepay.com/\');">www.multisafepay.com</a></td>
            </tr>
            </table>';
    }
}