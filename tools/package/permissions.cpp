/* Copyright (C) 2010 MoSync AB

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

#include <set>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "permissions.h"

using namespace std;

// Parses a comma-separated list of permissions
void parsePermissions(set<string>& out, const char* permissions) {
	string permissionStr = string(permissions);

	string::size_type lastPos = permissionStr.find_first_not_of(",", 0);
	string::size_type pos = permissionStr.find_first_of(",", lastPos);

	while (string::npos != pos || string::npos != lastPos) {
		out.insert(permissionStr.substr(lastPos, pos - lastPos));
        lastPos = permissionStr.find_first_not_of(",", pos);
        pos = permissionStr.find_first_of(",", lastPos);
	}
}

bool isPermissionSet(std::set<std::string>& permissionSet, const char* permission) {
	return permissionSet.find(string(permission)) != permissionSet.end();
}
