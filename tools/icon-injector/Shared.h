/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef _SHARED_H_
#define _SHARED_H_

#include "Icon.h"
#include <string>

namespace MoSync {

/**
 * Function used to exit on error. 
 * \param reason The reason for exiting (i.e. the error).
 */
void errorExit(const std::string& reason);

/**
 * Function used to convert an icon instance to an image.
 * \param iconInstance The icon instance.
 * \param dstFilename The filename of the resulting image.
 * \param size A string describing the size of the resulting image on the form: WxH
 * \param format A string describing the format of the resulting image (the file extension of the format)
 */
bool convertInstanceToImageFormat(const IconInstance *iconInstance, const char *dstFilename, const std::string& size, const std::string& format);

/**
 * Function to get the extension of a full filepath string.
 */
std::string getExtension(const std::string& str);

/**
 * Function to get the absolute path of a full filepath string.
 */
std::string getAbsolutePath(const std::string& path);

/// Parse a string on the format <width>x<height>.
void sizeString(const std::string& size, int* w, int* h);

/// Run a command using system().
int run(const char* cmd);
}

#endif // _SHARED_H_
