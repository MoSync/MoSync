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

/*
 * @file mosync.resource.js
 * @author Ali Sarrafi
 *
 * The library for loading Image resources into Mosync program from Javascript.
 * This library only supports image resources and used  together with the
 * NativeUI library.
 */


/**
 * The Resource handler submodule of the bridge module.
 * @private
 */
mosync.resource = {};


/**
 * A Hash containing all registered callback functions for
 * loadImage function.
 * @private
 */
mosync.resource.imageCallBackTable = {};

mosync.resource.imageIDTable = {};

mosync.resource.imageDownloadQueue = [];

/**
 * Loads images into image handles for use in MoSync UI systems.
 *
 *  @param imagePath relative path to the image file.
 *  @param imageID a custom ID used for refering to the image in JavaScript
 *  @param callBackFunction a function that will be called when the image is ready.
 *
 *  Example
 *  -------
 *  \code
 *    mosync.resource.loadImage("./img/logo.png", "Logo", function(imageID, imageHandle){
 *			var myImageButton = document.getNativeElementById("myImageButton");
 *			myImageButton.setProperty("image", imageHandle);
 * 		});
 *  \endcode
 *
 */
mosync.resource.loadImage = function(imagePath, imageID, successCallback)
{
	mosync.resource.imageCallBackTable[imageID] = successCallback;
	mosync.bridge.send(
			[
				"Resource",
				"loadImage",
				imagePath,
				imageID
			], null);
};

/**
 * A function that is called by C++ to pass the loaded image information.
 *
 * @param imageID JavaScript ID of the image
 * @param imageHandle C++ handle of the imge which can be used for refering to the loaded image
 * @private
 */
mosync.resource.imageLoaded = function(imageID, imageHandle)
{
	var callbackFun = mosync.resource.imageCallBackTable[imageID];
	if (undefined != callbackFun)
	{
		var args = Array.prototype.slice.call(arguments);

		// Call the function.
		callbackFun.apply(null, args);
	}
};

/**
 * Loads images into image handles from a remote URL for use in MoSync UI systems.
 *
 *  @param imageURL URL to the image file.
 *  @param imageID a custom ID used for refering to the image in JavaScript
 *  @param callBackFunction a function that will be called when the image is ready.
 *
 *  Example
 *  -------
 *  \code
 *    mosync.resource.loadRemoteImage("http://www.example.com/img/logo.png", "Logo", function(imageID, imageHandle){
 *			var myImageButton = document.getNativeElementById("myImageButton");
 *			myImageButton.setProperty("image", imageHandle);
 * 		});
 *  \endcode
 */
mosync.resource.loadRemoteImage = function(imageURL, imageID, callBackFunction)
{
	mosync.resource.imageCallBackTable[imageID] = callBackFunction;
	var message = [
		"Resource",
		"loadRemoteImage",
		imageURL,
		imageID
	];
	// Add message to queue.
	mosync.resource.imageDownloadQueue.push(message);

	if (1 == mosync.resource.imageDownloadQueue.length)
	{
		mosync.bridge.send(message, null);
	}

};

mosync.resource.imageDownloadStarted = function(imageID, imageHandle)
{
	mosync.resource.imageIDTable[imageHandle] = imageID;
};

mosync.resource.imageDownloadFinished = function(imageHandle)
{
	var imageID = mosync.resource.imageIDTable[imageHandle];
	var callbackFun = mosync.resource.imageCallBackTable[imageID];
	if (undefined != callbackFun)
	{
		// Call the function.
		callbackFun(imageID, imageHandle);
	}
	// Remove first message.
	if (mosync.resource.imageDownloadQueue.length > 0)
	{
		mosync.resource.imageDownloadQueue.shift();
	}

	// If there are more messages, send the next
	// message in the queue.
	if (mosync.resource.imageDownloadQueue.length > 0)
	{
		mosync.bridge.send(
				mosync.resource.imageDownloadQueue[0],
				null);
	}
};
