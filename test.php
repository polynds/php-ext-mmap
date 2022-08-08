<?php

$filename = '/mnt/d/go1.txt';
$o = new Kjmmap();
$o->open($filename,8000);
$s = "hellowo222fdfsdsdsd-wsedadasdasdas2222222rldhjfdskjkj,fdnjskfnsdk fhdjskhfkjsadlfsda 3242432hjbcjkzx435345b34";
$o->write($s,strlen($s));
$s = "2014年4月12日 自由控制脚本中数据的输出。它非常地有用,特别是对于:当你想 在数据已经输出后,再输出文件头的情况。输出控制函数不对使用 header() 或setcookie(), 发送的文...5b34";
$o->write($s,strlen($s));
$o->seek(0,0);
var_dump($o->read(100));
$o->flush();
$o->close();
var_dump(1111);