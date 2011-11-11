<?php

require_once('db_priv_conn.php'); //connect to the database

/* check that the $_GET variables that needs to be set is set */
if(!isset($_GET['benchmark'], $_GET['phone'], $_GET['runtime']))
	die('All required GET parameters was not sent!');

/* store the input retrieved via HTTP GET */
$benchmark = mysql_real_escape_string($_GET['benchmark']); //which benchmark we ran 'linpack', 'opengl', etc.
$revision = mysql_real_escape_string($_GET['revision']); //which revision of mosync we ran it on (if we did not run a native runtime)
$runtime = mysql_real_escape_string($_GET['runtime']); //the runtime name, must be matched to a runtime_id
$git_hash = mysql_real_escape_string($_GET['git_hash']); //the git_hash of the current build (if running a mosync runtime or mosync generated native code)
$phone = mysql_real_escape_string($_GET['phone']); //the phone name, must be matched to a phone_id 
$native_sdk_ver = mysql_real_escape_string($_GET['native_sdk_ver']); //the native SDK version
$mflops = mysql_real_escape_string($_GET['mflops']); //the linpack result in MFLOPS (float)

$test = mysql_real_escape_string($_GET['test']); //additional parameters

//echo(" $benchmark $revision $runtime_id $git_hash $phone_id $native_sdk_ver $mflops"); //DEBUG prints to see the values of the GET params

/* retrieve the phone ID of this phone from the 'phones' table*/
$res = mysql_query("SELECT phone_id from phones WHERE phone_name = '$phone'");
$row = mysql_fetch_assoc($res);
$phone_id = $row['phone_id'];
if(mysql_num_rows($res) == 0){ /* NO match in the database, we need to add this phone/device to the 'phones' table */
	$res = mysql_query("INSERT INTO phones (phone_name) VALUES ('$phone')");
	/* try to retrieve the phone_id again */
	$res = mysql_query("SELECT phone_id from phones WHERE phone_name = '$phone'");
	$row = mysql_fetch_assoc($res);
	$phone_id = $row['phone_id'];
}
	
/* retrieve the runtime ID of this runtime from the 'runtimes' table*/
$res = mysql_query("SELECT runtime_id from runtimes WHERE runtime_name = '$runtime'");
$row = mysql_fetch_assoc($res);
$runtime_id = $row['runtime_id'];
/* NO match in the database, we need to add this runtime to the 'runtimes' table */
if(mysql_num_rows($res) == 0){
	$res = mysql_query("INSERT INTO runtimes (runtime_name) VALUES ('$runtime')");
	/* try to retrieve the runtime_id again */
	$res = mysql_query("SELECT runtime_id from runtimes WHERE runtime_name = '$runtime'");
	$row = mysql_fetch_assoc($res);
	$runtime_id = $row['runtime_id'];
}

if($benchmark == 'linpack'){ //insert row into 'linpack_testruns'
	$res = mysql_query("INSERT INTO linpack_testruns (revision, runtime_id, git_hash, phone_id, native_sdk_ver, mflops) VALUES ('$revision', '$runtime_id', '$git_hash', '$phone_id', '$native_sdk_ver', '$mflops')");
}else if($benchmark == 'opengl'){ //insert row into 'opengl_testruns'
	$res = mysql_query("INSERT INTO opengl_testruns (revision, runtime_id, git_hash, phone_id, native_sdk_ver, mflops) VALUES ('$revision', '$runtime_id', '$git_hash', '$phone_id', '$native_sdk_ver', '$mflops')");
}else{ //die, no matching table for the benchmark param
	die('benchmark was not correctly set in the GET request');
}

if(!$res)
	mysql_error($link); //any mysql errors?

require_once('db_conn_close.php'); //close the db connection

?>