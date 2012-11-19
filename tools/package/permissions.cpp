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
#include <algorithm>

#include "permissions.h"

using namespace std;

// Parses a comma-separated list of permissions
void parsePermissions(set<string>& out, const char* permissions) {
	if (permissions == NULL) {
		return;
	}
	string permissionStr = string(permissions);

	string::size_type lastPos = permissionStr.find_first_not_of(",", 0);
	string::size_type pos = permissionStr.find_first_of(",", lastPos);

	while (string::npos != pos || string::npos != lastPos) {
		string permission = permissionStr.substr(lastPos, pos - lastPos);
		out.insert(normalizePermission(permission));
		lastPos = permissionStr.find_first_not_of(",", pos);
        pos = permissionStr.find_first_of(",", lastPos);
	}
}

std::string normalizePermission(std::string permission) {
	std::string result(permission);
	std::replace(result.begin(), result.end(), ' ', '-');
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

string getParentPermission(string permission) {
	permission = normalizePermission(permission);
	string::size_type slashPos = permission.find_last_of("/", string::npos);
	if (string::npos == slashPos) {
		return string();
	} else {
		return permission.substr(0, slashPos);
	}
}

bool isPermissionSet(std::set<std::string>& permissionSet, string permission) {
	if (permission.length() == 0) {
		return false;
	}
	permission = normalizePermission(permission);
	string parent = getParentPermission(permission);
	return permissionSet.find(string(permission)) != permissionSet.end() ||
		isPermissionSet(permissionSet, parent);
}

PermissionContext::PermissionContext(MustacheContext* parent, const char* permissions) : DefaultContext(parent) {
	parsePermissions(fParsedPermissions, permissions);
}

string PermissionContext::getParameter(string key) {
	// Convert : to /
	std::replace(key.begin(), key.end(), ':', '/');
	string result = isPermissionSet(fParsedPermissions, key) ? key : string();
	return result.empty() ? DefaultContext::getParameter(key) : result;
}

vector<MustacheContext*> PermissionContext::getChildren(string key) {
	return vector<MustacheContext*>();
}
