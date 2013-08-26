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
