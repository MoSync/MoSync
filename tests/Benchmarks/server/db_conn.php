<?php

$db_user = 'benchmark_read'; //read has SELECT rights only in the db
$pwd = 'W8RQhyYKhPDGMGfW';
$link = mysql_connect('localhost', $db_user, $pwd);

if (!$link) {
    die('Could not connect: ' . mysql_error());
}

$db_name = 'benchmark';
if (!mysql_select_db($db_name)) {
    echo "Unable to select database $db_name: " . mysql_error();
    exit;
}

//echo 'Connected successfully';

?>