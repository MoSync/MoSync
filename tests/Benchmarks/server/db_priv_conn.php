<?php

/* login with all privelegies on the benchmark database */

$db_user = 'alexander'; //all has all rights in the database 'benchmark'
$pwd = '983jf73ndsk';
$link = mysql_connect('localhost', $db_user, $pwd);

if (!$link) {
    die('Could not connect: ' . mysql_error());
}

$db_name = 'benchmark';
if (!mysql_select_db($db_name)) {
    echo "Unable to select database $db_name: " . mysql_error();
    exit;
}

echo 'Connected successfully';

?>