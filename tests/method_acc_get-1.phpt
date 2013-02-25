--TEST--
method_acc_get normal test
--SKIPIF--
<?php if (!extension_loaded("prlib")) print "skip"; ?>
--FILE--
<?php 

class A {

    public function foo() {

    }

    protected static function foo2() {

    }
}

class B extends Exception {

}

class C extends A {

}

ini_set('display_errors',true);
var_dump(method_acc_get('A', "foo"));
var_dump(method_acc_set('A', "foo", "private"));
var_dump(method_acc_get('A', "foo"));
var_dump(method_acc_get('A', "foo2"));
var_dump(method_acc_set('A', "foo2", "public"));
var_dump(method_acc_get('A', "foo2"));

//var_dump(method_acc_get('B', "getMessage"));
var_dump(method_acc_get('C', "foo"));
?>
--EXPECT--
string(6) "public"
bool(true)
string(7) "private"
string(9) "protected"
bool(true)
string(6) "public"
string(6) "public"
