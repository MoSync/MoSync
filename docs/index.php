<?php

// Log file for dynamically called update timestamps
$UPDATELOG = "updatetime";

// Log file for scheduled update timestamps
$SCHEDULEDLOG = "gentime";


//error_reporting ( E_ALL );


/************ Functions begin ***********/

// Displays the index page.
// $scheduled is passed as a flag indicating if
// this call is from a scheduled (from the 'cron'/'at' job)
// or if it was called dynamically online.
function outputindex ( $lastsched , $last , $msg , $redirect="" ) {

	$output = '
	<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
	<html xmlns="http://www.w3.org/1999/xhtml">

	<head>
	<title>MoSync :: Reference Documentation</title>

	<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"/>
	%4$s
	<link rel="stylesheet" href="mosyncdocsindex.css" type="text/css" />

	<script type="text/javascript">

	self.setInterval ( "countdown()" , 1000 );
		
	// Time of last update
	var lastsched  = %1$d * 1000; 
	var last = %2$d * 1000;

	// Calculate and set the time of the next scheduled update
	var next = new Date( lastsched );
	next.setHours ( next.getHours() + 3 );
	next = next.getTime() / 1000;


	function init() {

		// Set the text
		var lastField = document.getElementById ("lastsched");
		lastField.innerHTML = new Date ( lastsched ).toString();

		var lastUp = document.getElementById ( "last" );
		lastUp.innerHTML = new Date ( last ).toString();

		countdown();
	}
	
	function countdown () {

	    var newField = document.getElementById ("newdate");
	
	    var now = Math.round ( new Date().getTime() / 1000 ); // in milliseconds / 1000 to seconds

	    var raw = next - now;	

	    if ( raw > 0 ) {

		    var hDiff = Math.floor ( raw / 3600 ); // 3600  = 60 * 60  (seconds / hour )
	
		    raw = raw - ( hDiff * 3600 );

		    var mDiff = Math.floor ( raw / 60 );
	
		    raw = raw - ( mDiff * 60 );

		    var sDiff = Math.floor (raw);

		    newField.innerHTML = "Next scheduled update in: " + hDiff + " h " + mDiff + " m " + sDiff + " s";
	    }
	    else
		    newField.innerHTML = "Documentation updated. Please refresh this page.";

	}	

					

	</script>


	</head>


	<body onload="init();">

	<div id="centerblock">

		<div id="header">
			<div id="mainpic"><p id="maintext">MoSync Reference Documentation</p></div>
		</div>

		<div id="spacer"></div>
	
		<div id="content">
		<dl>

			<dt><a href="MAStd/index.html">MAStd</a></dt>
			<dd>
			Standard libraries and syscalls. Includes graphics, sound, communication, user input, permanent storage, malloc/free, string manipulation and a rudimentary text output console.
			</dd>

			<dt><a href="MAUtil/index.html">MAUtil</a></dt>
			<dd>
			C++ utility libraries. Includes templates Map and Vector, class String, a base class for Reference Counting, a read-only stream system and a SAX-like XML parser.
			</dd>

		</dl>
		</div>

		<div class="timer">
			<p>Last scheduled update on <span id="lastsched"></span></p>
			<p>Last updated on <span id="last"></span></p>
			<p><span id="newdate"></span></p>

			<!--
			<p id="updateover">%3$s</p>
			<p><a href="index.php?update=true">Update Documentation Now</a></p>
			-->

			<p id="update">%3$s</p>

		</div>

		<div id="bottom">

		<p>
			<a href="http://validator.w3.org/check?uri=referer"><img src="http://www.w3.org/Icons/valid-xhtml10" alt="Valid XHTML 1.0 Strict" height="31" width="88" /></a>
		</p>
		</div>

	</div>
	</body>
	</html>
	';


	
	printf ( $output, $lastsched , $last , $msg , $redirect );
}



// update() checks if the update call has been called
// more than 3 times in the past 60 minute period and limits updates
// to only 3 in that period
// If the current call is outside of one hour of the previous
// call, 
function allowupdate () {

	global $UPDATELOG;

	// Number of seconds per "period"
	// Default is 1 hour == 60 * 60 == 3600 seconds
	$PERIOD = 3600;

	// Number of updates within $PERIOD
	$ALLOWEDUPDATES = 3;

	$times = @file ( $UPDATELOG );

	// If the file doesn't exist, this must be the first update,
	// so write the file and allow the update
	// ALLOW the update
	if ( !$times ) { 		
		
		filewrite ( $UPDATELOG , time() + "\n" );

		return true;		
	}

	else {
		// Read the first line in the file 'updatetimes'
		// which represents the first update time
		$first = $times[0];

		// Then get the time now
		$now = time();


		// If we've been called in the same period as the original timestamp
		// AND the total number of stamps is less than $ALLOWEDUPDATES
		// ALLOW the update
		if ( (($now - $first) < $PERIOD ) && ( sizeof ( $times ) < $ALLOWEDUPDATES ) ){
			
			filewrite ( $UPDATELOG , $now );
			return true;
		}

		// If the first timestamp is from longer than a cycle ago
		// clear the 'updatetimes' file to reset the count
		// and write in the current time as the timestamp
		// ALLOW the update
		else if ( ($now - $first) > $PERIOD ) {
			
			unlink ( $UPDATELOG );
			filewrite ( $UPDATELOG , $now );
			
			return true;
		}

		// If the first timestamp is still within the same period
		// but there are more than 3 existing timestamps,
		// DISALLOW the update
		else if ( (($now - $first) < $PERIOD ) && (sizeof ( $times ) > $ALLOWEDUPDATES )) {

			return false;
		}

		// DISALLOW the update in all other cases
		else {
			return false;
		}
	}
}


// Open specified file,
// Append the specified input
// Close the file.
function filewrite ( $filename , $input ) {

	$f = fopen ( $filename , "a+");
	fwrite ( $f , $input . "\n" );

	fclose( $f );
}

// Retrieves the first line of specified file
// which is assumed to be a timestamp and simply
// returned as a string.
// For the curious, the timestamp format is
// in the number of seconds since the UNIX Epoch.
function getfirsttime ( $filename ) {

	$times = @file ( $filename );
	
	return $times[0];
}


function getlasttime ( $filename ) {

	$times = @file ( $filename );

	//$last = sizeof ( $times );

	return $times[ sizeof($times)-2 ];
}



/********** END FUNCTIONS *********/



global $SCHEDULEDLOG;

$LASTSCHED = getfirsttime ( $SCHEDULEDLOG );


$UPDATELINK = "<a href='index.php?update=true'>Update Documentation Now</a>";
$NOUPDATE = "Sorry, the maximum number of updates been reached in " . $PERIOD . " seconds. <br/>Please try again in the next cycle.";

$MSG = "";

$REDIRECT = "<meta http-equiv=\"Refresh\" content=\"0; url=index.php\">";

$LASTTIME = getlasttime ( $UPDATELOG );

if ( !$LASTTIME )
	$LASTTIME = $LASTSCHED;

if ( isset ( $_GET['update'] ) &&  !empty ( $_GET['update'] ) && $_GET['update'] == 'true' ) {


	if ( allowupdate() ) {
		
		// Generate the docs
		$p = popen ( "/bin/bash ./gendox.sh", "r" );
		pclose ( $p );

		outputindex ( $LASTSCHED , $LASTTIME , $UPDATELINK );
	}

	else
		outputindex ( $LASTSCHED , $LASTTIME , $NOUPDATE );
}

else {
	// Normal output
	outputindex ( $LASTSCHED , $LASTTIME , $UPDATELINK );

}




?>
