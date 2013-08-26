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

#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_

#include <string>
#include <vector>
#include <helpers/cpp_defs.h>

struct Key {
	int keyCode;
	int x, y, w, h;
};

struct Skin {
	std::string model;
	std::string vendor;
	short screenX, screenY, screenW, screenH;
	std::vector<Key> keys;
	std::string selectedImage;
	std::string unselectedImage;
};

// returns a new char[], or NULL.
char* readFileIntoMem(const char* filename, int* len);

// returns true if succeeded, false if not
bool parseConfig(const std::string& configPath);

// return NULL if no skin is found.
Skin* chooseSkin(const std::string& model, const std::string& vendor);

#endif /* _CONFIG_PARSER_H_ */
