--TEST--
test1() Basic test
--EXTENSIONS--
kjmmap
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension kjmmap is loaded and working!
NULL
