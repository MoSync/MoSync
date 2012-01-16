/*
  File: filesystest-main.js
  Auhthor: Mikael Kindborg

  Test script for PhoneGap File API.

  TODO: Add conditional checks to the test cases, so that
  this becomes a proper test.

  TODO: Rename this file to a better name.
*/

function MyLog(s)
{
	if (typeof mosync === "undefined")
	{
		console.log(s);
	}
	else
	{
		mosync.log(s);
	}
}

function PrintObject(obj, indent)
{
	if (undefined === indent)
	{
		indent = "";
	}

	console.log(indent + "@@ PrintObject");

	for (var field in obj)
	{
		if (typeof obj[field] != "function")
		{
			MyLog("  " + indent + "[" + field + ": " + obj[field] + "]");
			if ((null != obj[field]) && (typeof obj[field] == "object"))
			{
				PrintObject(obj[field], indent + "  ");
			}
		}
	}
}

var FileSys = function()
{
	var FileSys = {};

	/**
	 * Make generic error handling function that calls fun.
	 */
	function error(fun)
	{
		return function(result)
		{
			MyLog("@@@ FileSys error: " + result.code);
			fun(false, null);
		};
	}

	FileSys.writeText = function(path, data, fun)
	{
		FileSys.writeTextAtPosition(path, data, 0, fun);
	};

	FileSys.writeTextAtPosition = function(path, data, position, fun)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				fileSystem.root.getFile(
					path,
					{ create: true },
					function(fileEntry)
					{
						var writer = new FileWriter(fileEntry);
						writer.onwrite = function(obj)
						{
							fun(true);
						};
						writer.error = function(obj)
						{
							fun(false);
						};
						writer.seek(position);
						writer.write(data);
					},
					error(fun));
			},
			error(fun));
	};

	FileSys.readText = function(path, fun)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				fileSystem.root.getFile(
					path,
					{ create: true },
					function(fileEntry)
					{
						var reader = new FileReader();
						reader.onload = function(obj)
						{
							fun(true, obj.target.result);
						};
						reader.onerror = function(obj)
						{
							 fun(false, null);
						};
						reader.readAsText(fileEntry);
					},
					error(fun));
			},
			error(fun));
	};

	FileSys.readAsDataURL = function(path, fun)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				fileSystem.root.getFile(
					path,
					{ create: true },
					function(fileEntry)
					{
						var reader = new FileReader();
						reader.onload = function(obj)
						{
							fun(true, obj.target.result);
						};
						reader.onerror = function(obj)
						{
							 fun(false, null);
						};
						reader.readAsDataURL(fileEntry);
					},
					error(fun));
			},
			error(fun));
	};

	FileSys.truncate = function(path, size, fun)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				fileSystem.root.getFile(
					path,
					{ create: true },
					function(fileEntry)
					{
						var writer = new FileWriter(fileEntry);
						writer.onwrite = function(obj)
						{
							fun(true);
						};
						writer.error = function(obj)
						{
							fun(false);
						};
						writer.truncate(size);
					},
					error(fun));
			},
			error(fun));
	};

	FileSys.getMetaData = function(path, fun)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				fileSystem.root.getFile(
					path,
					{ create: false },
					function(fileEntry)
					{
						fileEntry.getMetadata(
							function(metadata) { fun(true, metadata); },
							error(fun));
					},
					error(fun));
			},
			error(fun));
	};

	/**
	 * Copy of move a file from a source path to a
	 * destination path.
	 * @param path Full source path.
	 * @param newPath Full destination path.
	 * @param move true if the file should be moved,
	 * false if it should be copied.
	 */
	FileSys.copyOrMoveFile = function(path, newPath, fun, move)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				fileSystem.root.getFile(
					path,
					{ create: false },
					function(fileEntry)
					{
						// Full parent directory path of new file destination,
						// excluding the file name.
						var newParentPath;

						// New file name.
						var newFileName;

						// The new full path name.
						var newFullPath = fileSystem.root.fullPath + "/" + newPath;

						// Split into parent path and file name if the file
						// is not at the root of the local file system.
						var index = newFullPath.lastIndexOf("/");
						if (index > -1)
						{
							newParentPath = newFullPath.substring(0, index);
							newFileName = newFullPath.substring(index + 1);

							MyLog("copyOrMoveFile newParentPath: " + newParentPath);
							MyLog("copyOrMoveFile newFileName: " + newFileName);
						}
						else
						{
							// Path name is invalid.
							error(fun);
						}

						// Find destination directory name.
						// TODO: This will break if file is at the
						// root of the file system.
						var directoryName = "";
						index = newParentPath.lastIndexOf("/");
						if (index > -1)
						{
							directoryName = newParentPath.substring(index + 1);
							MyLog("copyOrMoveFile directoryName: " + directoryName);
						}

						// Create destination directory object.
						var parentDirectory = new DirectoryEntry();
						parentDirectory.fullPath = newParentPath;
						parentDirectory.name = directoryName;

						if (move)
						{
							fileEntry.moveTo(
								parentDirectory,
								newFileName,
								function(result) {
									MyLog("FileSys.copyOrMoveFile MOVE");
									PrintObject(result);
									fun(true, result); },
								error(fun));
						}
						else
						{
							fileEntry.copyTo(
								parentDirectory,
								newFileName,
								function(result) {
									MyLog("FileSys.copyOrMoveFile COPY");
									PrintObject(result);
									fun(true, result); },
								error(fun));
						}
					},
					error(fun));
			},
			error(fun));
	};

	FileSys.copyFile = function(path, newPath, fun)
	{
		return FileSys.copyOrMoveFile(path, newPath, fun, false);
	};

	FileSys.moveFile = function(path, newPath, fun)
	{
		return FileSys.copyOrMoveFile(path, newPath, fun, true);
	};

	FileSys.removeFile = function(path, fun)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				fileSystem.root.getFile(
					path,
					{ create: false },
					function(fileEntry)
					{
						fileEntry.remove(
							function(fileEntry) { fun(true, fileEntry); },
							error(fun));
					},
					error(fun));
			},
			error(fun));
	};

	FileSys.removeDirectory = function(path, fun)
	{
		window.requestFileSystem(
			LocalFileSystem.PERSISTENT,
			0,
			function (fileSystem)
			{
				var fullPath = fileSystem.root.fullPath + "/" + path;

				// Find directory name.
				var directoryName = "";
				var index = fullPath.lastIndexOf("/");
				if (index > -1)
				{
					directoryName = fullPath.substring(index + 1);
					MyLog("removeDirectory directoryName: " + directoryName);
				}

				// Create destination directory object.
				var directory = new DirectoryEntry();
				directory.fullPath = fullPath;
				directory.name = directoryName;

				directory.remove(
					function(entry) { fun(true, entry); },
					error(fun));
			},
			error(fun));
	};

	return FileSys;
}();

function testFileSystem()
{
	// ****** Write/Read files ****** //

	function writeFile()
	{
		FileSys.writeText("hello2.txt", "Hello\nWonderful\nWorld!", fileWritten);
	}

	function fileWritten(success)
	{
		MyLog("@@@@@ FileWritten result: " + success);
		if (success)
		{
			FileSys.readText("hello2.txt", fileRead);
		}
	}

	function fileRead(success, data)
	{
		MyLog("@@@@@ FileRead result: " + success);
		MyLog("@@@@@ FileRead data: " + data);

		if (success)
		{
			// This does not work, since seek does not work. Skip for now.
			// FileSys.writeTextAtPosition("hello2.txt", "Micke", 5, fileWrittenAtPosition);
			FileSys.truncate("hello2.txt", 10, fileTruncated);
		}
	}

	// Writing at postion does not work.
	function fileWrittenAtPosition(success, data)
	{
		MyLog("@@@@@ FileWritten2 result: " + success);

		if (success)
		{
			FileSys.truncate("hello2.txt", 10, fileTruncated);
		}
	}

	function fileTruncated(success, data)
	{
		MyLog("@@@@@ FileTruncated result: " + success);

		if (success)
		{
			FileSys.readAsDataURL("hello2.txt", fileReadAsDataURL);
		}
	}

	function fileReadAsDataURL(success, url)
	{
		MyLog("@@@@@ FileReadAsDataURL result: " + success);

		if (success)
		{
			MyLog("@@@@@ FileReadAsDataURL url: " + url);
			var i = url.indexOf(",");
			var data = url.substring(i + 1);
			MyLog("@@@@@ FileReadAsDataURL data: " + data);
			var decodedData = atob(data);
			MyLog("@@@@@ FileReadAsDataURL decodedData: " + decodedData);

			// Next test.
			FileSys.getMetaData("hello2.txt", fileMetaData);
		}
	}

	function fileMetaData(success, metadata)
	{
		MyLog("@@@@@ FileMetaData result: " + success);
		MyLog("@@@@@ FileMetaData modificationTime: " + metadata.modificationTime);

		if (success)
		{
			FileSys.moveFile("hello2.txt", "hello3.txt", fileMoved);
		}
	}

	// ****** Move, Copy, Remove files ****** //

	function fileMoved(success)
	{
		MyLog("@@@@@ FileMoved result: " + success);

		if (success)
		{
			FileSys.copyFile("hello3.txt", "hello4.txt", fileCopied);
		}
	}

	function fileCopied(success)
	{
		MyLog("@@@@@ FileCopied result: " + success);

		if (success)
		{
			FileSys.removeFile("hello4.txt", fileRemoved);
		}
	}

	function fileRemoved(success)
	{
		MyLog("@@@@@ FileRemoved result: " + success);

		if (success)
		{
			// Write file in non-existing directory.
			FileSys.writeText("foo/hello.txt", "Hello World!", fileWrittenInDirectory);
		}
	}

	// ****** Write/Read files in directories ****** //

	function fileWrittenInDirectory(success)
	{
		MyLog("@@@@@ FileWrittenInDirectory result: " + success);
		if (success)
		{
			FileSys.readText("foo/hello.txt", fileReadFromDirectory);
		}
	}


	function fileReadFromDirectory(success, data)
	{
		MyLog("@@@@@ FileReadFromDirectory result: " + success);
		MyLog("@@@@@ FileReadFromDirectory data: " + data);

		if (success)
		{
			FileSys.moveFile("foo/hello.txt", "bar/hello2.txt", fileMovedDirectory);
		}
	}

	// ****** Move, Copy, Remove files in directories ****** //

	function fileMovedDirectory(success)
	{
		MyLog("@@@@@ FileMovedDirectory result: " + success);

		if (success)
		{
			FileSys.copyFile("bar/hello2.txt", "foobar/hello3.txt", fileCopiedDirectory);
		}
	}

	function fileCopiedDirectory(success)
	{
		MyLog("@@@@@ FileCopiedDirectory result: " + success);

		if (success)
		{
			FileSys.removeFile("bar/hello2.txt", fileRemovedFromDirectory);
		}
	}

	function fileRemovedFromDirectory(success)
	{
		MyLog("@@@@@ FileRemovedFromDirectory result: " + success);

		if (success)
		{
			//FileSys.removeFile("bar", fileRemovedDirectory);
			FileSys.removeDirectory("bar", fileRemovedDirectory);
		}
	}

	function fileRemovedDirectory(success)
	{
		MyLog("@@@@@ FileRemovedDirectory result: " + success);

		if (success)
		{
			// Read from foobar.
			FileSys.readText("foobar/hello3.txt", fileReadFromCopiedDirectory);
		}
	}

	function fileReadFromCopiedDirectory(success, data)
	{
		MyLog("@@@@@ FileReadFromCopiedDirectory result: " + success);
		MyLog("@@@@@ FileReadFromCopiedDirectory data: " + data);

		if (success)
		{
			// TODO: add more tests here.
		}
	}

	// Start first test.
	writeFile();
}

/**
 * Will overwrite previous testFileSystem ;-)
 * Tests directory functions.
 */
function testFileSystem()
{
	function writeFiles()
	{
		FileSys.writeText("foz/hello1.txt", "Hello World", reportResult);
		FileSys.writeText("foz/baz/hello2.txt", "Hello World", reportResult);
		FileSys.writeText("foz/baz/hello3.txt", "Hello World", listFiles);
	}

	function listFiles(success)
	{
		MyLog("@@@@@ listFiles got result: " + success);
		if (success)
		{
			//FileSys.removeDirectory("foz", reportResult);
			FileSys.removeFile("foz", reportResult);
		}
	}

	function reportResult(success)
	{
		MyLog("reportResult: " + success);
	}

	writeFiles();
}

function XtestFileSystem() {
	MyLog("@@ Calling window.requestFileSystem");
	window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, gotFileSystem, filefail);
	//window.requestFileSystem(LocalFileSystem.TEMPORARY, 0, gotFileSystem, filefail);
}

function gotFileSystem(fileSystem)
{
	MyLog("@@ gotFileSystem");
	PrintObject(fileSystem);
	MyLog("@@ Calling fileSystem.root.getFile");
	fileSystem.root.getFile("helloworld.txt", {create: true}, gotOpenFileEntry, filefail);
}

function gotOpenFileEntry(fileEntry) {
	gFileEntry = fileEntry;
	MyLog("@@ gotOpenFileEntry");
	MyLog("@@ Calling fileEntry.createWriter");
	fileEntry.createWriter(gotFileWriter, filefail);
}

function gotFileWriter(writer) {
	MyLog("@@ gotFileWriter");
	writer.onwrite = function(obj) {
		MyLog("@@ writer.onwrite obj: " + obj);
		PrintObject(obj);
		readFileEntry(gFileEntry);
	};
	writer.error = function(obj) {
		MyLog("@@ writer.error");
		PrintObject(obj);
	};
	writer.write("Hello World");
	MyLog("@@ writer.write called");
}

function readFileEntry(fileEntry)
{
	MyLog("@@ readFileEntry");
	MyLog("@@ Calling fileEntry.file");
	fileEntry.file(function(file)
	{
		readAsText(file);
	},
	filefail);
}

function readAsText(file) {
	MyLog("@@ readAsText");
	var reader = new FileReader();
	reader.onload = function(obj)
	{
		MyLog("@@ reader.onload");
		PrintObject(obj);
	};
	reader.onerror = function(obj)
	{
		MyLog("@@ reader.onerror");
		PrintObject(obj);
	};
	MyLog("@@ Calling reader.readAsText");
	reader.readAsText(file);
}

function readFile(file){
	MyLog("@@ readFile");
	readDataUrl(file);
	readAsText(file);
}

function readDataUrl(file) {
	MyLog("@@ readDataUrl");
	var reader = new FileReader();
	reader.onloadend = function(evt) {
		MyLog("Read as data URL");
		MyLog(evt.target.result);
	};
	reader.readAsDataURL(file);
}


function filefail(error) {
	MyLog("@@ filefail code: " + error.code);
}

// ---------------------------------------------------------------

var deviceInfo = function() {
	document.getElementById("platform").innerHTML = device.platform;
	document.getElementById("version").innerHTML = device.version;
	document.getElementById("uuid").innerHTML = device.uuid;
	document.getElementById("name").innerHTML = device.name;
	document.getElementById("width").innerHTML = screen.width;
	document.getElementById("height").innerHTML = screen.height;
	document.getElementById("colorDepth").innerHTML = screen.colorDepth;
};

var getLocation = function() {
	var suc = function(p) {
		alert(p.coords.latitude + " " + p.coords.longitude);
	};
	var locFail = function() {
	};
	navigator.geolocation.watchPosition(suc, locFail);
};

var beep = function() {
	navigator.notification.beep(2);
};

var vibrate = function() {
	navigator.notification.vibrate(1000);
};

function roundNumber(num) {
	var dec = 3;
	var result = Math.round(num * Math.pow(10, dec)) / Math.pow(10, dec);
	return result;
}

var accelerationWatch = null;

function updateAcceleration(a) {
	document.getElementById('x').innerHTML = roundNumber(a.x);
	document.getElementById('y').innerHTML = roundNumber(a.y);
	document.getElementById('z').innerHTML = roundNumber(a.z);
}

var toggleAccel = function() {
	if (accelerationWatch !== null) {
		navigator.accelerometer.clearWatch(accelerationWatch);
		updateAcceleration({
			x : "",
			y : "",
			z : ""
		});
		accelerationWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		accelerationWatch = navigator.accelerometer.watchAcceleration(
				updateAcceleration, function(ex) {
					alert("accel fail (" + ex.name + ": " + ex.message + ")");
				}, options);
	}
};

var preventBehavior = function(e) {
	e.preventDefault();
};

function dump_pic(data) {
	var viewport = document.getElementById('viewport');
	MyLog(data);
	viewport.style.display = "";
	viewport.style.position = "absolute";
	viewport.style.top = "10px";
	viewport.style.left = "10px";
	document.getElementById("test_img").src = "data:image/jpeg;base64," + data;
}

function fail(msg) {
	alert(msg);
}

function show_pic() {
	navigator.camera.getPicture(dump_pic, fail, {
		quality : 50
	});
}

function close() {
	var viewport = document.getElementById('viewport');
	viewport.style.position = "relative";
	viewport.style.display = "none";
}

function contacts_success(contacts) {
	alert(contacts.length
			+ ' contacts returned.'
			+ (contacts[2] && contacts[2].name ? (' Third contact is ' + contacts[2].name.formatted)
					: ''));
}

function get_contacts() {
	var obj = new ContactFindOptions();
	obj.filter = "";
	obj.multiple = true;
	navigator.contacts.find(
			[ "displayName", "name" ], contacts_success,
			fail, obj);
}

function check_network() {
	var networkState = navigator.network.connection.type;

	var states = {};
	states[Connection.UNKNOWN]  = 'Unknown connection';
	states[Connection.ETHERNET] = 'Ethernet connection';
	states[Connection.WIFI]	 = 'WiFi connection';
	states[Connection.CELL_2G]  = 'Cell 2G connection';
	states[Connection.CELL_3G]  = 'Cell 3G connection';
	states[Connection.CELL_4G]  = 'Cell 4G connection';
	states[Connection.NONE]	 = 'No network connection';

	confirm('Connection type:\n ' + states[networkState]);
}

function init() {
	// the next line makes it impossible to see Contacts on the HTC Evo since it
	// doesn't have a scroll button
	// document.addEventListener("touchmove", preventBehavior, false);
	document.addEventListener("deviceready", deviceInfo, true);
}
