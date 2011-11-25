<?php

require_once('db_priv_conn.php'); //connect to the database

/* check that the $_GET variables that needs to be set is set */
if(!isset($_GET['benchmark'], $_GET['phone'], $_GET['runtime']))
	die('All required GET parameters was not sent!');

/* store the input retrieved via HTTP GET */
$benchmark = mysql_real_escape_string($_GET['benchmark']); 				//which benchmark we ran 'linpack', 'opengl', etc.
$revision = mysql_real_escape_string($_GET['revision']); 				//which revision of mosync we ran it on (if we did not run a native runtime)
$runtime = mysql_real_escape_string($_GET['runtime']); 					//the runtime name, must be matched to a runtime_id
$git_hash = mysql_real_escape_string($_GET['git_hash']); 				//the git_hash of the current build (if running a mosync runtime or mosync generated native code)
$phone = mysql_real_escape_string($_GET['phone']); 						//the phone name, must be matched to a phone_id 
$native_sdk_ver = mysql_real_escape_string($_GET['native_sdk_ver']);	//the native SDK version

//only access the $_GET indexes we use
if($benchmark == 'linpack'){
	/* LINPACK PARAMETERS */
	$mflops = mysql_real_escape_string($_GET['mflops']); 					//the linpack result in MFLOPS (float)
}else if($benchmark == 'opengl'){
	/* OpenGLES PARAMETERS*/
	$test1 = mysql_real_escape_string($_GET['test1']); 						//OpenGLES test #1
	$test2 = mysql_real_escape_string($_GET['test2']); 						//OpenGLES test #2
	$test3 = mysql_real_escape_string($_GET['test3']); 						//OpenGLES test #3
	$test4 = mysql_real_escape_string($_GET['test4']); 						//OpenGLES test #4
}else if($benchmark == 'membench'){
	/* MEMBENCH PARAMETERS */
	$alloc_str_10 = mysql_real_escape_string($_GET['alloc_str_10']);
	$alloc_str_100 = mysql_real_escape_string($_GET['alloc_str_100']);
	$alloc_void_1 = mysql_real_escape_string($_GET['alloc_void_1']);
	$alloc_void_100 = mysql_real_escape_string($_GET['alloc_void_100']);
	$alloc_void_1000 = mysql_real_escape_string($_GET['alloc_void_1000']);
	$alloc_dummy = mysql_real_escape_string($_GET['alloc_dummy']);
	$alloc_dummy_struct = mysql_real_escape_string($_GET['alloc_dummy_struct']);
	$alloc_dummy_mix = mysql_real_escape_string($_GET['alloc_dummy_mix']);
	$access_array = mysql_real_escape_string($_GET['access_array']);
	$access_vector = mysql_real_escape_string($_GET['access_vector']);
	$add_vector = mysql_real_escape_string($_GET['add_vector']);
	$access_dummy = mysql_real_escape_string($_GET['access_dummy']);
	$access_dummy_struct = mysql_real_escape_string($_GET['access_dummy_struct']);
	$access_dummy_mix = mysql_real_escape_string($_GET['access_dummy_mix']);
}

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
	$res = mysql_query("INSERT INTO linpack_testruns (revision, runtime_id, git_hash, phone_id, native_sdk_ver, mflops) VALUES ('$revision', '$runtime_id', '$git_hash', '$phone_id', '
$native_sdk_ver', '$mflops')");
}else if($benchmark == 'opengl'){ //insert row into 'opengl_testruns' TODO FIX
	$res = mysql_query("INSERT INTO opengl_testruns (revision, runtime_id, git_hash, phone_id, native_sdk_ver, mflops) VALUES ('$revision', '$runtime_id', '$git_hash', '$phone_id', '$native_sdk_ver
', '$mflops')");
}else if($benchmark == 'membench'){
	$res = mysql_query("INSERT INTO membench_testruns (revision, runtime_id, git_hash, phone_id, native_sdk_ver, alloc_str_10, alloc_str_100, alloc_void_1, alloc_void_100, alloc_void_1000, 
	alloc_dummy, alloc_dummy_struct, alloc_dummy_mix, access_array, access_vector, add_vector, access_dummy, access_dummy_struct, access_dummy_mix) VALUES ('$revision', '$runtime_id', '$git_hash', '
	$phone_id', '$native_sdk_ver', '$alloc_str_10', '$alloc_str_100', '$alloc_void_1', '$alloc_void_100', '$alloc_void_1000', '$alloc_dummy', '$alloc_dummy_struct', '$alloc_dummy_mix', '$access_array
	', '$access_vector', '$add_vector', '$access_dummy', '$access_dummy_struct', '$access_dummy_mix')");
}else{ //die, no matching table for the benchmark param
	die('benchmark was not correctly set in the GET request');
}

if(!$res)
	mysql_error($link); //any mysql errors?

require_once('db_conn_close.php'); //close the db connection

?>