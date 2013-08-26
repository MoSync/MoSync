/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
#define FONT_ID							3
#define BASE_SCREEN_WIDTH				240
#define SCALE_FACTOR 					(gScreenWidth / BASE_SCREEN_WIDTH)
#define TEXT_SIZE						(10 * SCALE_FACTOR)
#define OFFSET_X						(10 * SCALE_FACTOR)
#define OFFSET_Y						TEXT_SIZE

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
