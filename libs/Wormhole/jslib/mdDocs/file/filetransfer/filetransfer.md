FileTransfer
==========

FileTransfer is an object that allows you to upload files to a server or download files from a server.

Properties
----------

N/A

Methods
-------

- __upload__: sends a file to a server. 

Details
-------

The `FileTransfer` object provides a way to upload files to a remote server using an HTTP multi-part POST request.  Both HTTP and HTTPS protocols are supported.  Optional parameters can be specified by passing a FileUploadOptions object to the upload method.  On successful upload, the success callback will be called with a FileUploadResult object. If an error occurs, the error callback will be invoked with a FileTransferError object.

Supported Platforms
-------------------

- Android
- iOS
- Windows Phone 7 ( Mango )

upload
--------------

__Parameters:__

- __filePath__ - Full path of the file on the device
- __server__ - URL of the server to receive the file
- __successCallback__ - A callback that is called with a Metadata object. _(Function)_
- __errorCallback__ - A callback that is called if an error occurs retrieving the Metadata. Invoked with a FileError object. _(Function)_
- __options__ - Optional parameters such as file name and mimetype

__Quick Example__
	
	// !! Assumes variable fileURI contains a valid URI to a  text file on the device
	
	var win = function(r) {
		console.log("Code = " + r.responseCode);
		console.log("Response = " + r.response);
		console.log("Sent = " + r.bytesSent);
	}
	
	var fail = function(error) {
		alert("An error has occurred: Code = " + error.code);
		console.log("upload error source " + error.source);
		console.log("upload error target " + error.target);
	}
	
	var options = new FileUploadOptions();
	options.fileKey="file";
	options.fileName=fileURI.substr(fileURI.lastIndexOf('/')+1);
	options.mimeType="text/plain";

  var params = new Object();
	params.value1 = "test";
	params.value2 = "param";
		
	options.params = params;
	
	var ft = new FileTransfer();
	ft.upload(fileURI, "http://some.server.com/upload.php", win, fail, options);
	
__Full Example__

	<!DOCTYPE html>
	<html>
	<head>
		<meta name="viewport" content="width=320, user-scalable=yes">
		<title>File Upload Example</title>
		<script type="text/javascript" charset="utf-8" src="js/wormhole.js"></script>
		<script type="text/javascript" charset="utf-8">
		var app = (function()
		{
			var app = {};

			app.snapPhotoAndUpload = function()
			{
				navigator.device.capture.captureImage(
					/**
					 * Callback that returns the image that was captured.
					 * @param mediaFiles Array with images.
					 */
					function(mediaFiles)
					{
						var fileURL = "file://" + mediaFiles[0].fullPath;
						alert("Uploading picture");
						app.uploadPhoto(fileURL);
					},
					function(error)
					{
						alert("Could not take picture - error: " + error.code);
					});
			};

			app.uploadPhoto = function(fileURL)
			{
				var options = new FileUploadOptions();
				options.fileKey = "file";
				options.fileName = fileURL.substr(fileURL.lastIndexOf('/') + 1);
				options.mimeType = app.getMimeType(options.fileName);
				options.params = null;

				var transfer = new FileTransfer();
				transfer.upload(
					fileURL,
					"http://dev.mosync.com/mobilelua/PhotoGallery/upload.php",
					function(result)
					{
						alert("Photo uploaded");
						console.log("Code = " + result.responseCode);
						console.log("Response = " + result.response);
						console.log("Sent = " + result.bytesSent);
					},
					function(error)
					{
						alert("Could not upload photo - error: " + error.code);
						console.log("Could not upload photo - error: " + error.code);
					},
					options);
			}

			app.getMimeType = function(fileName)
			{
				if (/\.jpg$/.test(fileName)) return "image/jpeg";
				if (/\.jpeg$/.test(fileName)) return "image/jpeg";
				if (/\.png$/.test(fileName)) return "image/png";
				return "application/octet-stream";
			}

			return app;
		})();
		</script>
	</head>
	<body>
		<h1>File Upload Example</h1>
		<div>
			<input
				type="button"
				value="Snap Photo and Upload"
				onclick="app.snapPhotoAndUpload()"/>
		</div>
	</body>
	</html>
	