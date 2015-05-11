<?php

class API_Autoloader {

    public static function autoload($class_name) {

        $file_name = str_replace("_", "/", $class_name);
        $file_name = realpath(dirname(__FILE__) . "/../$file_name.php");

        if (file_exists($file_name)) {
            require $file_name;
        }
    }

    public static function register() {
        return spl_autoload_register(array(__CLASS__, "autoload"));
    }

    public static function unregister() {
        return spl_autoload_unregister(array(__CLASS__, "autoload"));
    }

}

API_Autoloader::register();
