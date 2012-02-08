// Startning point of the program.
document.addEventListener(
	"deviceready",
	function() {
		//Populate the Device Info panel
		document.getElementById("platform_li").innerHTML =
			"Platform: " + device.platform;
		document.getElementById("plaformversion_li").innerHTML =
			"Version: " + device.version;
		document.getElementById("deviceName_li").innerHTML =
			"Device: " + device.name;
		initFileManager();
	},
	true);

var videoPath;
/**
 * Open the native Video Capture interface to record a single movie
 */
function captureVideo()
{
	navigator.device.capture.captureVideo(
		/**
		 * Callback that returns the movie that was captured
		 * @param mediaFiles Array with the movies that were
		 * captured during the last capture session
		 */
		function(mediaFiles)
		{
			//The following code is required to circumvent an iOS 4 bug
			$('#videoWidget').remove();
			var videoWidget = document.createElement('video');
			videoWidget.setAttribute("id", "videoWidget");
			videoWidget.setAttribute("controls", "true");
			videoWidget.setAttribute("style", "width:100%;height:50%");
			videoWidget.setAttribute("src", "file://" + mediaFiles[0].fullPath);
			$('#videoWidgetContainer').append(videoWidget);
		},
		function(error)
		{
			alert("Error " + error.code);
		});
}

/**
 * Open the native Video Capture interface to record a single movie
 */
function captureImage()
{
	navigator.device.capture.captureImage(
		/**
		 * Callback that returns the image that was captured
		 * @param mediaFiles Array with the images that were captured
		 * during the last capture session
		 */
		function(mediaFiles)
		{
			document.getElementById('capturedImage').setAttribute(
				"src", "file://" + mediaFiles[0].fullPath);
		},
		function(error)
		{
			alert("Error " + error.code);
		});
}