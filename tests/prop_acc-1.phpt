--TEST--
method_acc_get normal test
--SKIPIF--
<?php if (!extension_loaded("prlib")) print "skip"; ?>
--FILE--
<?php 

class A {

    protected $aaa = 123;

    public static $bbb = 123;

}

ini_set('display_errors',true);
var_dump(prop_acc_get('A', "aaa"));
var_dump(prop_acc_set('A', "aaa", "public"));
var_dump(prop_acc_get('A', "aaa"));
var_dump(prop_acc_get('A', "bbb"));
var_dump(prop_acc_set('A', "bbb", "private"));
var_dump(prop_acc_get('A', "bbb"));
?>
--EXPECT--
string(9) "protected"
bool(true)
string(6) "public"
string(6) "public"
bool(true)
string(7) "private"
