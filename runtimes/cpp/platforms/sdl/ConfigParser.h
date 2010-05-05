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
