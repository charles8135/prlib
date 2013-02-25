--TEST--
method_disable_exit normal test
--SKIPIF--
<?php if (!extension_loaded("prlib")) print "skip"; ?>
--FILE--
<?php 

ini_set('display_errors',true);
echo "11\n";
disable_exit();
exit();
echo "22\n";
enable_exit();
exit();
echo "33\n";
?>
--EXPECT--
11
22
