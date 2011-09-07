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
 * @file consts.h
 * @author Florin Leu
 */

#ifndef CONSTS_H_
#define CONSTS_H_

// the number of sensors in MoSync
#define SENSOR_TYPES					5
// maximum number of values returned by a sensor
#define SENSOR_VALUES					3

#define BUFFER_SIZE						50
#define BG_COLOR						0
#define TEXT_COLOR						0xFF0000
#define TEXT_SIZE						20
#define OFFSET_Y						20

// useful texts
#define TXT_NONE						""
#define TXT_SENSOR_ACCELEROMETER		"Accelerometer"
#define TXT_SENSOR_MAG_FIELD			"Magnetic field"
#define TXT_SENSOR_ORIENTATION			"Orientation"
#define TXT_SENSOR_GYROSCOPE			"Gyroscope"
#define TXT_SENSOR_PROXIMITY			"Proximity"

#define TXT_AVAILABLE					"Available"
#define TXT_NOT_AVAILABLE				"Not available"
#define TXT_INTERVAL					"Interval not set"
#define TXT_ENABLED						"Already enabled"

#define TXT_UNKNOWN						"Unknown"
#define TXT_PORTRAIT					"Portrait"
#define TXT_UPSIDE_DOWN					"Portrait upside down"
#define TXT_LANDSCAPE_LEFT				"Landscape left"
#define TXT_LANDSCAPE_RIGHT				"Landscape right"
#define TXT_FACE_UP						"Face up"
#define TXT_FACE_DOWN					"Face down"

#endif //CONSTS_H_
