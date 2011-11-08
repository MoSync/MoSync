/* Copyright (C) 2011 Mobile Sorcery AB

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

#ifndef PROFILEDB_H_
#define PROFILEDB_H_

#include <map>
#include <set>
#include <string>
#include <sstream>

using namespace std;

class ProfileDB;

enum CapabilityState { SUPPORTED, UNSUPPORTED, NOT_IMPLEMENTED, REQUIRES_PERMISSION, REQUIRES_PRIVILEGED_PERMISSION };

class Capability {
private:
	string fName;
	CapabilityState fState;
};

class Profile {
private:
	string fProfileName;
	string fFamily;
	string fVariant;
	Profile* fParent;
	map<string, Capability> fCapabilities;
public:
	Profile(string profileName) { fProfileName = profileName; }
	string getProfileName() { return fProfileName; }
	void setParent(Profile* parent) { fParent = parent; }
	void setFamily(string family) { fFamily = family; }
	void setVariant(string variant) { fVariant = variant; }
	Capability getCapability(string capabilityName);
	void toXML(ostringstream& output);
};

class ProfileDB {
private:
	string profilesdir();
	bool parseProfileXML(Profile* profile, set<string> alreadyFound);
public:
	ProfileDB() { };
	void getProfile(string profileName);
	void listProfiles(string pattern);
	Profile* findProfile(string profileName, set<string> alreadyFound);
};

#endif /* PROFILEDB_H_ */
