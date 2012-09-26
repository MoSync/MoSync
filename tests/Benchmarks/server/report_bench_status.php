
<?php
//This script is used for the automatic benchmark suite to post the status of its runs. The status is just pass/fail, and 10 results are kept in the database at once, so that old failures don't live forever
//but live long enough to be discovered and examined.
require_once('db_priv_conn.php'); //set up the connection to the database

$result = mysql_escape_string($_GET['result']);
mysql_query("INSERT INTO automatic_runs (result) VALUES ('$result')");

$res = mysql_query("SELECT * FROM automatic_runs");
$num_rows = mysql_num_rows($res);

if($num_rows > 10)
	mysql_query("DELETE FROM automatic_runs ORDER BY id LIMIT 1");


//free result and close db connection
mysql_free_result($res);
require_once('db_conn_close.php');
?>
