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
 * @file Util.h
 * @author Emma Tresanszki
 *
 * This file contains the utility functions and values for the application.
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <MAUtil/String.h>
#include <mastring.h>
#include <MAUtil/util.h>

// Colors used for fonts & background.
enum Colors
{
	SEA_GREEN = 0xC1FFC1 ,
	DARK_SEA_GREEN = 0x698B69,
	LIGHT_GRAY = 0xD9D9D9 ,
	INTENSE_BLUE = 0x104E8B ,
	LIGHT_BLUE = 0xB2DFEE
};

/**
 * Strings for Video Screen.
 */
const MAUtil::String SOURCE_LOADING     = "Source is loading..";
const MAUtil::String SOURCE_READY       = "Source is ready for playback";
const MAUtil::String SOURCE_PLAYING     = "Video is playing";
const MAUtil::String SOURCE_PAUSED      = "Video is paused";
const MAUtil::String SOURCE_STOPPED     = "Video is stopped";
const MAUtil::String SOURCE_FINISHED    = "Video has finished playing";
const MAUtil::String SOURCE_ERROR       = "Some error occurred";

/**
 * Strings for the Settings Screen.
 */
const MAUtil::String DISPLAY_DURATION 	   = " Display Duration ";
const MAUtil::String HOME_URL			   = " Home URL ";
const MAUtil::String HOME_URL_ANDROID 	   = "http://www.mosync.com/files/videos/Video.3gp";
const MAUtil::String HOME_URL_IOS 		   = "http://www.mosync.com/files/videos/ExVideo.m3u8";
const MAUtil::String HOME_URL_WINDOWSPHONE = "http://www.educationalquestions.com/video/ELL_PART_5_768k.wmv";
const MAUtil::String RESET_TO_DEFAULT 	   = "Reload default source";

void getScreenSize();
int getScreenWidth();
int getScreenHeight();

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool isAndroid();

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
bool isIOS();

/**
 * Detects if the current platform is Windows Phone.
 * @return true if the platform is Windows Phone, false otherwise.
 */
bool isWindowsPhone();

/**
 * Converts milliseconds value into a more readable output.
 * @param milliseconds An int.
 * @return A string in the form: HH::MM::SS.
 */
MAUtil::String getFormatedDuration(int milliseconds);

/**
 * The screen width.
 */
extern int screenWidth;

/**
 * The screen height.
 */
extern int screenHeight;

#endif /* UTIL_H_ */
