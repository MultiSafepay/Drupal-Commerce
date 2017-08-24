<?php
/**
*
* DISCLAIMER
*
* Do not edit or add to this file if you wish to upgrade the MultiSafepay plugin
* to newer versions in the future. If you wish to customize the plugin for your
* needs please document your changes and make backups before you update.
*
* @category MultiSafepay
* @package Connect
* @author TechSupport <techsupport@multisafepay.com>
* @copyright Copyright (c) 2017 MultiSafepay, Inc. (http://www.multisafepay.com)
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
