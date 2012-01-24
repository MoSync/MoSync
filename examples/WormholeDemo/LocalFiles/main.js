var userFilesDir;
var textChanged = false;
var currentFile;

document.addEventListener("deviceready", function() {
	document.getElementById("platform_li").innerHTML = "Platform: " + device.platform;
	document.getElementById("plaformversion_li").innerHTML = "Version: " + device.version;
	document.getElementById("deviceName_li").innerHTML = "Device: " + device.name;
	window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, function(fileSystem)
	{
		fileSystem.root.getDirectory("UserFiles",
				{create: false, exclusive: false},
				function(dirEntry)
				{
					userFilesDir = dirEntry;
				},
				function(error)
				{
					alert(error);
				});
	},
	function(error)
	{
		alert("fs error");
	});
}, true);

var accelerometer = new SensorConnection("Accelerometer");

accelerometer.addEventListener("onsensordata", updateAccelW3C);

function updateAccelW3C(sensorData){
	document.getElementById('accelW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('accelW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('accelW3Cz').innerHTML = sensorData.data.z;
}

function toggleAccelW3C()
{
	if(accelerometer.status == "open")
	{
		accelerometer.startWatch({interval:1000});
	}
	else
	{
		accelerometer.endWatch();
		updateAccelW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var magneticField = new SensorConnection("MagneticField");

magneticField.addEventListener("onsensordata", updateMagDataW3C);

function updateMagDataW3C(sensorData){
	document.getElementById('MagFieldW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('MagFieldW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('MagFieldW3Cz').innerHTML = sensorData.data.z;
}

function toggleMagFieldW3C()
{
	if(magneticField.status == "open")
	{
		magneticField.startWatch({interval:1000});
	}
	else
	{
		magneticField.endWatch();
		updateMagDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var orientationSensor = new SensorConnection("Orientation");

orientationSensor.addEventListener("onsensordata", updateOrientDataW3C);

function updateOrientDataW3C(sensorData){
	document.getElementById('OrientationW3C').innerHTML = sensorData.data.x;
}

function toggleOrientationW3C()
{
	if(orientationSensor.status == "open")
	{
		orientationSensor.startWatch({interval:1000});
	}
	else
	{
		orientationSensor.endWatch();
		updateOrientDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var gyroscope = new SensorConnection("Gyroscope");

gyroscope.addEventListener("onsensordata", updateGyroDataW3C);

function updateGyroDataW3C(sensorData){
	document.getElementById('GyroscopeW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('GyroscopeW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('GyroscopeW3Cz').innerHTML = sensorData.data.z;
}

function toggleGyroscopeW3C()
{
	if(gyroscope.status == "open")
	{
		gyroscope.startWatch({interval:1000});
	}
	else
	{
		gyroscope.endWatch();
		updateGyroDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var proximity = new SensorConnection("Proximity");

proximity.addEventListener("onsensordata", updateProxDataW3C);

function updateProxDataW3C(sensorData){
	document.getElementById('ProximityW3C').innerHTML = sensorData.data.x;
}

function toggleProximityW3C()
{
	if(proximity.status == "open")
	{
		proximity.startWatch({interval:1000});
	}
	else
	{
		proximity.endWatch();
		updateProxDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

function captureVideo()
{
	navigator.device.capture.captureVideo(
			function(mediaFiles)
			{
				alert(mediaFiles[0].fullPath);
				document.getElementById('videoWidget').setAttribute("src", "file://" + mediaFiles[0].fullPath);
			},
			function(error)
			{
				alert("Error " + error.code);
			});
}

function captureImage()
{
	navigator.device.capture.captureImage(
			function(mediaFiles)
			{
				document.getElementById('capturedImage').setAttribute("src", "file://" + mediaFiles[0].fullPath);
			},
			function(error)
			{
				alert("Error " + error.code);
			});
}

function refreshFiles()
{
	var dirReader = userFilesDir.createReader();
	$('#fileList > li').remove();
	dirReader.readEntries(function(entries)
	{
		for(var i = 0; i < entries.length; i++)
		{
			var newLi = document.createElement("li");
			var newA = document.createElement("a");
			$(newA).data('file', entries[i]);
			newA.setAttribute("onclick", "setFile($(this).data('file'))");
			newA.innerHTML = entries[i].name;
			newLi.appendChild(newA);
			$('#fileList').append(newLi);
		}
	},
	function(error)
	{
		alert("errer");
	});
}

function setFile(file)
{
	jQT.goTo('#filePage','pop');
	$('#filePanelTitle').html(file.name);
	currentFile = file;
	var reader = new FileReader();
	reader.onloadend = function(evt){
		$('#fileContents').val(evt.target.result);
	};
	reader.readAsText(file);
}

function createFile()
{
	var fileName = prompt("New File");
	if(fileName != "")
	{
		userFilesDir.getFile(fileName, {create: true, exclusive: true},function(){
			refreshFiles();
		},
		function()
		{
			alert("error");
		});
	}
}

function deleteFile()
{
	if(confirm("Delete " + currentFile.name + "?") == true)
	{
		currentFile.remove();
		jQT.goBack();
	}
}

function textHasChanged()
{
	textChanged = true;
}

function textLostFocus()
{
	if(textChanged == true)
	{
		textChanged = false;
		currentFile.createWriter(function(writer){
			writer.seek(0);
			text = $('#fileContents').val();
			writer.write(text);
			writer.truncate(text.length);
		},
		function(error){
			alert("error");
		});
	}
}