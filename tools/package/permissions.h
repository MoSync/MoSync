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

#ifndef PERMISSION_H
#define PERMISSION_H

#include <set>
#include <string>
#include "mustache/mustache.h"

#define BLUETOOTH "Bluetooth"
#define CALENDAR "Calendar"
#define CALENDAR_READ "Calendar/Read"
#define CALENDAR_WRITE "Calendar/Write"
#define CAMERA "Camera"
#define CONTACTS "Contacts"
#define CONTACTS_READ "Contacts/Read"
#define CONTACTS_WRITE "Contacts/Write"
#define FILE_STORAGE "File Storage"
#define FILE_STORAGE_WRITE "File Storage/Write"
#define FILE_STORAGE_READ "File Storage/Read"
#define INTERNET "Internet Access"
#define LOCATION "Location"
#define LOCATION_FINE "Location/Fine"
#define LOCATION_COARSE "Location/Coarse"
#define POWER_MANAGEMENT "Power Management"
#define SMS "SMS"
#define SMS_SEND "SMS/Send"
#define SMS_RECEIVE "SMS/Receive"
#define SMS_READ "SMS/Read"
#define VIBRATE "Vibration"
#define VIBRATE_DEPRECATED "Vibrate"
#define HOMESCREEN "Home Screen"
#define AUTOSTART "Autostart"
#define AUTOSTART_DEPRECATED "Auto Start"
#define NFC "NFC"
#define PUSH_NOTIFICATIONS "Push Notifications"
#define PHONE_CALLS "Phone Calls"
#define PURCHASE "In-App Purchase"

// Parse permissions
void parsePermissions(std::set<std::string>& out, const char* permissions);

/**
 * 'Normalizes' a permission, ie converts it to lower case
 * and replaces all spaces with -.
 */
std::string normalizePermission(std::string permission);

std::string getParentPermission(std::string permission);

bool isPermissionSet(std::set<std::string>& permissions, std::string permission);

class PermissionContext : public DefaultContext {
private:
	std::set<std::string> fParsedPermissions;

public:
	PermissionContext(MustacheContext* parent, const char* permissions);
	string getParameter(string key);
	vector<MustacheContext*> getChildren(string key);
};

#endif // PERMISSION_H
