/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file main.js
 *
 * The starting point of the program and some of the functions
 * used by the demo.
 */

// Define the starting point of the program.
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
