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
const MAUtil::String DISPLAY_DURATION 	 = " Display Duration ";
const MAUtil::String HOME_URL			 = " Home URL ";
const MAUtil::String HOME_URL_ANDROID 	 = "http://www.mosync.com/files/videos/heineken.3gp";
const MAUtil::String HOME_URL_IOS 		 = "http://www.mosync.com/files/videos/ExVideo.m3u8";
const MAUtil::String RESET_TO_DEFAULT 	 = "Reload default source";

void getScreenSize();
int getScreenWidth();
int getScreenHeight();

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool isAndroid();

/**
 * The screen width.
 */
extern int screenWidth;

/**
 * The screen height.
 */
extern int screenHeight;

#endif /* UTIL_H_ */
