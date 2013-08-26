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

#ifndef _SHARED_H_
#define _SHARED_H_

#include "Icon.h"
#include <string>
#include <helpers/attribute.h>

namespace MoSync {

/**
 * Function used to exit on error. 
 * \param reason The reason for exiting (i.e. the error).
 */
void errorExit(const std::string& reason) GCCATTRIB(noreturn);

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
