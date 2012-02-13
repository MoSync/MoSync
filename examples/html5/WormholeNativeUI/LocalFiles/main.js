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
 * Functions to display device info and handle sensors.
 */

function onDeviceReady()
{
	var platform = document.getNativeElementById("DeviceOSLabel");
	platform.setProperty("text" , "Platform: " + device.platform);
	var version = document.getNativeElementById("OSVersionLabel");
	version.setProperty("text" , "Version: " + device.version);
	var uuid = document.getNativeElementById("UUIDLabel");
	uuid.setProperty("text" , "UUID: " + device.uuid);
	var deviceName = document.getNativeElementById("DeviceNameLabel");
	deviceName.setProperty("text" , "Device Name: " + device.name);
}

var accelPhonegapWatch = null;
function toggleAccelPhonegap()
{
	if (accelPhonegapWatch !== null)
	{
		navigator.accelerometer.clearWatch(accelPhonegapWatch);
		updateAccelPhonegap({
			x : " ",
			y : " ",
			z : " "
		});
		accelPhonegapWatch = null;
	}
	else
	{
		var options = {};
		options.frequency = 1000;
		accelPhonegapWatch = navigator.accelerometer.watchAcceleration(
			updateAccelPhonegap, function(ex) {
				console.log("accel fail (" + ex.name + ": " + ex.message + ")");
			}, options);
	}
}

function roundNumber(num)
{
	if (typeof num == "number")
	{
		var dec = 3;
		var result = Math.round(num * Math.pow(10, dec)) / Math.pow(10, dec);
		return result;
	}
	else
	{
		return num;
	}
}

function updateAccelPhonegap(a)
{
	var data = document.getNativeElementById("AccelX");
	data.setProperty("text" , "X: " + a.x);

	var data = document.getNativeElementById("AccelY");
	data.setProperty("text" , "Y: " + a.y);

	var data = document.getNativeElementById("AccelZ");
	data.setProperty("text" , "Z: " + a.z);
}

var geolocationWatch = null;
function toggleGeolocation()
{
	if (geolocationWatch !== null)
	{
		navigator.geolocation.clearWatch(geolocationWatch);
		console.log("Starting location");
		updateGeolocation({coords:{
			latitude : " ",
			longitude : " ",
			altitude : " "
		}});
		geolocationWatch = null;
	}
	else
	{
		var options = {};
		options.frequency = 1000;
		geolocationWatch = navigator.geolocation.watchPosition(
			// Success function.
			updateGeolocation,
			// Error function.
			function(ex) {
				var data = document.getNativeElementById("LocLat");
				data.setProperty("text" , "Latitude: n/a");
				data = document.getNativeElementById("LocLong");
				data.setProperty("text" , "Longtitude: n/a");
			},
			options);
	}
}

function updateGeolocation(a)
{
	var data = document.getNativeElementById("LocLat");
	data.setProperty("text" , "Latitude: " + a.coords.latitude);
	data = document.getNativeElementById("LocLong");
	data.setProperty("text" , "Longtitude: " + a.coords.longitude);
}

var compassWatch = null;
function toggleCompass()
{
	if (compassWatch !== null)
	{
		navigator.compass.clearWatch(compassWatch);
		updateCompass({
			magneticHeading : " "
		});
		compassWatch = null;
	}
	else
	{
		var options = {};
		options.frequency = 1000;
		compassWatch = navigator.compass.watchHeading(
			// Success function.
			updateCompass,
			// Error function.
			function(ex) {
				var data = document.getNativeElementById("CompassLabel");
				data.setProperty(
					"text",
					"Compass error: " + ex.name + ": " + ex.message);
			},
			options);
	}
}

function updateCompass(a)
{
	var data = document.getNativeElementById("CompassLabel");
	data.setProperty("text", "Compass Heading: " +  a.magneticHeading);
}
