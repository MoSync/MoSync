// These are HTML and JavaScript code snippets that have
// been used for testing. Currently they are not used,
// but remain as a reference.

// Test that JavaScript runs in the browser.
#define HTML1 " \
	<html><body> \
    <script>for(var i = 0; i < 100; ++i) { document.write('Hello World ' + i + '</br>'); }</script> \
    </body></html>"

// Call MoSync via JavsScript from a clickable link.
#define HTML2 " \
	<html><body> \
	<a href=\"#\" onclick=\"document.location = 'mosync://Notify/HelloWorld'\">Click Me</a></br> \
    </body></html>"

// Test many features in one example.
#define HTML3 " \
	<html> \
	<head> \
    <style type=\"text/css\"> \
		div { font-size:1.8em; margin-top:20pt; } \
		a { cursor: hand; } \
		input { font-size:1.2em; } \
	</style> \
	</head> \
	<body> \
	<script> \
		function MoSyncMessage(message) { \
			document.location = 'mosync://' + message; } \
		function ProcessData() { \
			var message = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncMessage(message); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Red')\">Red</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('CreatePanic')\">Create Panic</a></div> \
    </body> \
    </html>"

// Test that percent signs work (was an issue on Android).
#define HTML4 " \
	<html> \
	<head> \
    <style type=\"text/css\"> \
		div { font-size:150%; margin-top:20pt; } \
		a { cursor: hand; } \
		input { font-size:120%; } \
	</style> \
	</head> \
	<body> \
	<script> \
		function MoSyncMessage(message) { \
			document.location = 'mosync://' + message; } \
		function ProcessData() { \
			var message = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncMessage(message); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Red')\">Red</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
    </body> \
    </html>"

// Test subjective performance of JS background color change,
// if better than via MoSync roundtrip.
#define HTML5 " \
	<html> \
	<head> \
    <style type=\"text/css\"> \
		div { font-size:1.8em; margin-top:20pt; } \
		a { cursor: hand; } \
		input { font-size:1.2em; } \
	</style> \
	</head> \
	<body> \
	<script> \
		function SetBgColor(color) { \
			document.getElementById('ColorMessage').innerHTML = color + ' is a beautiful color!'; \
			document.bgColor = color; } \
		function MoSyncMessage(message) { \
			document.location = 'mosync://' + message; } \
		function ProcessData() { \
			var message = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncMessage(message); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"SetBgColor('Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"SetBgColor('Red')\">Red</a> \
		<a href=\"#\" onclick=\"SetBgColor('Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
    </body> \
    </html>"
