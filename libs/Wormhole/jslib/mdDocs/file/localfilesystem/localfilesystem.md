LocalFileSystem
===============

This object provides a way to obtain root file systems.

Methods
----------

- __requestFileSystem:__ Requests a filesystem. _(Function)_
- __resolveLocalFileSystemURI:__ Retrieve a DirectoryEntry or FileEntry using local URI. _(Function)_

Constants
---------

- `LocalFileSystem.PERSISTENT`: Used for storage that should not be removed by the user agent without application or user permission.
- `LocalFileSystem.TEMPORARY`: Used for storage with no guarantee of persistence.

NOTE: `LocalFileSystem.TEMPORARY` is currently NOT supported.

Details
-------

The `LocalFileSystem` object methods are defined on the __window__ object.

Request File System Quick Example
---------------------------------

	function onSuccess(fileSystem) {
		console.log(fileSystem.name);
	}

	// request the persistent file system
	window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, onSuccess, onError);

Resolve Local File System URI Quick Example
-------------------------------------------

	function onSuccess(fileEntry) {
		console.log(fileEntry.name);
	}

	window.resolveLocalFileSystemURI("file:///example.txt", onSuccess, onError);

Full Example
------------


	<!DOCTYPE html>
	<html>
	<head>
		<title>Local File System Example</title>

		<script type="text/javascript" charset="utf-8" src="js/wormhole.js"></script>
		<script type="text/javascript" charset="utf-8">

		// Wait for Wormhole to load
		//
		document.addEventListener("deviceready", onDeviceReady, false);

		// Wormhole is ready
		//
		function onDeviceReady() {
			window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, onFileSystemSuccess, fail);
			window.resolveLocalFileSystemURI("file:///example.txt", onResolveSuccess, fail);
		}

		function onFileSystemSuccess(fileSystem) {
			console.log(fileSystem.name);
		}

		function onResolveSuccess(fileEntry) {
			console.log(fileEntry.name);
		}

		function fail(evt) {
			console.log(evt.target.error.code);
		}

		</script>
	</head>
	<body>
		<h1>Example</h1>
		<p>Local File System</p>
	  </body>
	</html>
