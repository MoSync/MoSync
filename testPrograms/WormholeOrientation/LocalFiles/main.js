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
 * Called by the NativeUI library when the UI is ready to be shown.
 * Show the main screen here.
 *
 * Here we illustrate how to add events to widgets and how to create
 * widgets from JavaScript code.
 */
mosync.nativeui.UIReady = function()
{
	var tabScreen = document.getNativeElementById("tabScreen");
	tabScreen.show();
	tabScreen.addEventListener(
		"OrientationDidChange",
		tabScreeOrientationDidChange);
	tabScreen.addEventListener(
		"OrientationWillChange",
		tabScreeOrientationWillChange);

	mosync.app.registerOrientationDidChange(appOrientationDidChange);
	mosync.app.registerOrientationWillChange(appOrientationWillChange);
}

/**
 * Init UI.
 */
function initialize()
{
	document.addEventListener("backbutton", close, true);
	mosync.nativeui.initUI();
}

/**
 * Bitmask to store supported orientations.
 * Modified each time an orientation check box changes its state.
 */
var sSupportedOrientations = 0;

/**
 * CheckBox widgets IDs used to set orientation.
 */
var sCheckBoxIDs = [
                    "OCBPortraitUp",
                    "OCBPortraitUpsideDown",
                    "OCBPortrait",
                    "OCBLandscapeLeft",
                    "OCBLandscapeRight",
                    "OCBLandscape",
                    "OCBDynamic"];

/**
 * Set supported orientation depending on the states of the check box widgets.
 */
function setOrientationButtonClicked()
{
	mosync.app.screenSetSupportedOrientations(sSupportedOrientations);
}

/**
 * An orientation check box widget's state has changed.
 * Modify sSupportedOrientations bitmask.
 */
function checkBoxClicked()
{
	sSupportedOrientations = 0;
	var index = 0;
	for (var i = 0; i < sCheckBoxIDs.length; i++)
	{
		var checkBoxID = sCheckBoxIDs[i];
		var checkBox = document.getNativeElementById(checkBoxID);
		checkBox.getProperty("checked",
			function (methodName, isChecked)
			{
				var localCheckBoxID = sCheckBoxIDs[index];
				index = index + 1;
				var changedOrientation = getOrientationFlag(localCheckBoxID);
				if (isChecked == "true")
				{
					sSupportedOrientations = sSupportedOrientations | changedOrientation;
				}
			});
	}
}

/**
 * Display an message box with the supported orientation values.
 */
function getOrientationButtonClicked()
{
	mosync.app.screenGetSupportedOrientations(
		function(supportedOrientations)
		{
			mosync.notification.messageBox(
				"Supported orientations",
				supportedOrientationsFlagToString(supportedOrientations));
		});
}

/**
 * Display an message box with the current orientation value.
 */
function getCurrentOrientationButtonClicked()
{
	mosync.app.screenGetCurrentOrientation(
		function(currentOrientation)
		{
			mosync.notification.messageBox(
					"Current orientation",
					orientationFlagToString(currentOrientation));
		});
}

/**
 * Clear label widgets from second screen.
 */
function clearLabelsButtonClicked()
{
	var label = document.getNativeElementById("appOrientationDidChangeLabel");
	label.setProperty("text", "");

	label = document.getNativeElementById("appOrientationWillChangeLabel");
	label.setProperty("text", "");

	label = document.getNativeElementById("tabScreenOrientationDidChangeLabel");
	label.setProperty("text", "");

	label = document.getNativeElementById("tabScreenOrientationWillChangeLabel");
	label.setProperty("text", "");
}

/**
 * Application orientation did change.
 * Notify user that the event has been received.
 * @param newOrientation New app orientation.
 */
function appOrientationDidChange(newOrientation)
{
	var label = document.getNativeElementById("appOrientationDidChangeLabel");
	label.setProperty("text", "Orientation did change event");
}

/**
 * Application orientation will change.
 * Notify user that the event has been received.
 * Only on iOS platform.
 */
function appOrientationWillChange()
{
	var label = document.getNativeElementById("appOrientationWillChangeLabel");
	label.setProperty("text", "Orientation will change event");
}

/**
 * TabScreen orientation did change.
 * Notify user that the event has been received.
 */
function tabScreeOrientationDidChange()
{
	var label = document.getNativeElementById("tabScreenOrientationDidChangeLabel");
	label.setProperty("text", "TabScreen - orientation did change event");
}

/**
 * TabScreen orientation will change.
 * Notify user that the event has been received.
 */
function tabScreeOrientationWillChange()
{
	var label = document.getNativeElementById("tabScreenOrientationWillChangeLabel");
	label.setProperty("text", "TabScreen - orientation will change event");
}

/**
 * Get the orientation value associated with a check box widget.
 * @return One of the following values:
 *   mosync.SCREEN_ORIENTATION_PORTRAIT_UP
 *   mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
 *   mosync.SCREEN_ORIENTATION_PORTRAIT
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE
 *   mosync.SCREEN_ORIENTATION_DYNAMIC
 */
function getOrientationFlag(widgetID)
{
	var orientationFlag = 0;
	switch (widgetID)
	{
	case "OCBPortraitUp":
		orientationFlag = mosync.SCREEN_ORIENTATION_PORTRAIT_UP;
		break;
	case "OCBPortraitUpsideDown":
		orientationFlag = mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
		break;
	case "OCBPortrait":
		orientationFlag = mosync.SCREEN_ORIENTATION_PORTRAIT;
		break;
	case "OCBLandscapeLeft":
		orientationFlag = mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT;
		break;
	case "OCBLandscapeRight":
		orientationFlag = mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
		break;
	case "OCBLandscape":
		orientationFlag = mosync.SCREEN_ORIENTATION_LANDSCAPE;
		break;
	case "OCBDynamic":
		orientationFlag = mosync.SCREEN_ORIENTATION_DYNAMIC;
		break;
	default:
		orientationFlag = 0;
		break;
	}
	return orientationFlag;
}

/**
 * Convert an orientation value to string.
 * @param orientation One of the following values:
 *   mosync.SCREEN_ORIENTATION_PORTRAIT_UP
 *   mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT
 * @returns The string value.
 */
function orientationFlagToString(orientation)
{
	var orientationString = "";
	switch (orientation) {
	case mosync.SCREEN_ORIENTATION_PORTRAIT_UP:
		orientationString = "portrait up";
		break;
	case mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
		orientationString = "portrait upside down";
		break;
	case mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT:
		orientationString = "landscape left";
		break;
	case mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT:
		orientationString = "landscape right";
		break;
	default:
		break;
	}
	return orientationString;
}

/**
 * Convert an orientation bitmask to string.
 * @param supportedOrientations A bitmask created using the following values:
	mosync.SCREEN_ORIENTATION_PORTRAIT_UP
 *   mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
 *   mosync.SCREEN_ORIENTATION_PORTRAIT
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT
 *   mosync.SCREEN_ORIENTATION_LANDSCAPE
 *   mosync.SCREEN_ORIENTATION_DYNAMIC
 * @returns The string value.
 */
function supportedOrientationsFlagToString(supportedOrientations)
{
	var supportedOrientationsString = "";
	if (supportedOrientations & mosync.SCREEN_ORIENTATION_PORTRAIT_UP)
	{
		supportedOrientationsString += orientationFlagToString(
			mosync.SCREEN_ORIENTATION_PORTRAIT_UP) + ", ";
	}
	if (supportedOrientations & mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN)
	{
		supportedOrientationsString += orientationFlagToString(
			mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN) + ", ";
	}
	if (supportedOrientations & mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT)
	{
		supportedOrientationsString += orientationFlagToString(
			mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT) + ", ";
	}
	if (supportedOrientations & mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT)
	{
		supportedOrientationsString += orientationFlagToString(
			mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT) + ", ";
	}
	return supportedOrientationsString;
}
