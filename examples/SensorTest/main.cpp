/*
Copyright (C) 2011 MoSync AB

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
 * @file main.cpp
 * @author Florin Leu
 *
 * This is the main entry point for the example application
 * that demonstrates Sensors on MoSync.
 */

#include <MAUtil/Moblet.h>

#include <conprint.h>

#include "consts.h"

// The list with sensor error codes.
int gSensorError[SENSOR_TYPES + 1];
// Values returned by sensors.
float gSensorValue[SENSOR_TYPES + 1][SENSOR_VALUES];
// Names of the available sensors.
const char *gSensorName[SENSOR_TYPES + 1] =
{
	TXT_NONE,
	TXT_SENSOR_ACCELEROMETER,
	TXT_SENSOR_MAG_FIELD,
	TXT_SENSOR_ORIENTATION,
	TXT_SENSOR_GYROSCOPE,
	TXT_SENSOR_PROXIMITY
};

// Screen dimensions.
int gScreenWidth;
int gScreenHeight;

/**
 * Get the screen size into the instance variables
 * mScreenWidth and mScreenHeight.
 */
static void updateScreenDimensions()
{
	/// Get screen dimensions.
	MAExtent size = maGetScrSize();

	/// Extract the screen width
	gScreenWidth = EXTENT_X(size);

	/// Extract the screen height
	gScreenHeight = EXTENT_Y(size);
}

/*
 * Get the error message using the sensor error code.
 */
static const char* getErrorText(int errorCode)
{
	switch (errorCode)
	{
		case SENSOR_ERROR_NOT_AVAILABLE:
			return TXT_NOT_AVAILABLE;
		case SENSOR_ERROR_INTERVAL_NOT_SET:
			return TXT_INTERVAL;
		case SENSOR_ERROR_ALREADY_ENABLED:
			return TXT_ENABLED;
		default:
			return TXT_AVAILABLE;
	}
}

/*
 * @brief Get the orientation text using the sensor orientation code.
 */
static const char* getOrientationText(int orientation)
{
	switch (orientation)
	{
		case UIDEVICE_ORIENTATION_UNKNOWN:
			return TXT_UNKNOWN;
		case UIDEVICE_ORIENTATION_PORTRAIT:
			return TXT_PORTRAIT;
		case UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
			return TXT_UPSIDE_DOWN;
		case UIDEVICE_ORIENTATION_LANDSCAPE_LEFT:
			return TXT_LANDSCAPE_LEFT;
		case UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT:
			return TXT_LANDSCAPE_RIGHT;
		case UIDEVICE_ORIENTATION_FACE_UP:
			return TXT_FACE_UP;
		case UIDEVICE_ORIENTATION_FACE_DOWN:
			return TXT_FACE_DOWN;
		default:
			maPanic(1, "getOrientationText");
	}
}

/*
 * @brief Register to all sensors and get the error codes
 */
void registerSensors()
{
	for (int type=1; type<=SENSOR_TYPES; type++)
	{
		gSensorError[type] = maSensorStart(type, SENSOR_RATE_NORMAL);
	}
}

/*
 * @brief Unregister all the sensors
 */
void unregisterSensors()
{
	for (int type=1; type<=SENSOR_TYPES; type++)
	{
		maSensorStop(type);
	}
}

/*
 * @brief Draw the sensor values.
 * @param index Current sensor index.
 * @param x Coordinate x where to draw the values.
 * @param y Coordinate y where to draw the values.
 */
void drawSensorValue(int index, int x, int y)
{
	float* values = gSensorValue[index];
	char buffer[BUFFER_SIZE];
	switch (index)
	{
		case SENSOR_TYPE_ACCELEROMETER:
		case SENSOR_TYPE_MAGNETIC_FIELD:
		case SENSOR_TYPE_GYROSCOPE:
			sprintf(buffer, "Values: X:%0.4f; Y:%0.4f; Z:%0.4f",
					values[0], values[1], values[2]);
			break;
		case SENSOR_TYPE_ORIENTATION:
			sprintf(buffer, "Value: %s", getOrientationText((int)values[0]));
			break;
		case SENSOR_TYPE_PROXIMITY:
			if (values[0] == SENSOR_PROXIMITY_VALUE_FAR)
				sprintf(buffer, "Value: FAR");
			else
				sprintf(buffer, "Value: NEAR");
			break;
	}
	maDrawText(x, y, buffer);
}

/*
 * @brief Draw the sensor status.
 * @param index Current sensor index.
 * @param x Coordinate x where to draw the values.
 * @param y Coordinate y where to draw the values.
 */
void drawSensorStatus(int index, int x, int y)
{
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "%s: %s",
			gSensorName[index], getErrorText(gSensorError[index]));
	maDrawText(x, y, buffer);
}


/*
 * @brief Displays the sensor values
 * or an error message if the sensor cannot register.
 */
void drawSensorOutput()
{
	//clean the screen
	maSetColor(BG_COLOR);
	maFillRect(0, 0, gScreenWidth, gScreenHeight);

	//set output text color
	maSetColor(TEXT_COLOR);

	int posY = 0;
	for (int i=1; i<=SENSOR_TYPES; i++)
	{
		drawSensorStatus(i, 0, posY);
		posY += OFFSET_Y;

		// skip is sensor couldn't register
		if (gSensorError[i] != 0)
			continue;

		drawSensorValue(i, OFFSET_X, posY);
		posY += OFFSET_Y;
	}
}

/*
 * @brief Sets the font
 * to display the sensors values.
 */
void setFont()
{
	//int nrFonts = maFontGetCount();
	char fontName[BUFFER_SIZE];
	// get font with the specified ID.
	maFontGetName(FONT_ID, fontName, BUFFER_SIZE);
	// Load font with size 20.
	int fontHandle = maFontLoadWithName(fontName, TEXT_SIZE);
	maFontSetCurrent(fontHandle);
}

extern "C" int MAMain()
{
	bool run = true;

	updateScreenDimensions();
	registerSensors();
	setFont();

	// Force output for devices which hasn't got any sensors
	drawSensorOutput();
	maUpdateScreen();

	while (run)
	{
		/// Get any available events.
		/// On back press close program.
		MAEvent event;
		while(maGetEvent(&event))
		{
			if(event.type == EVENT_TYPE_SENSOR)
			{
				memcpy(gSensorValue[event.sensor.type],
						event.sensor.values, SENSOR_VALUES * sizeof(float));
			}
			else if((event.type == EVENT_TYPE_KEY_PRESSED) &&
					(event.key == MAK_BACK))
			{
				run = false;
			}

			drawSensorOutput();

			maUpdateScreen();
		}
	}

	unregisterSensors();
	return 0;
}
