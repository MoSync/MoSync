/* Copyright (C) 2011 MoSync AB

This program is free software you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef PERMISSION_H
#define PERMISSION_H

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
#define VIBRATE "Vibrate"
#define HOMESCREEN "Home Screen"
#define AUTOSTART "Auto Start"
#define NFC "NFC"
#define PUSH_NOTIFICATIONS "Push Notifications"

// Parse permissions
void parsePermissions(std::set<std::string>& out, const char* permissions);

bool isPermissionSet(std::set<std::string>& permissions, const char* permission);

#endif // PERMISSION_H
