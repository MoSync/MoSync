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
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include "XMLWriter.h"

using namespace std;

class ProfileDB;

enum CapabilityState { UNSUPPORTED, NOT_IMPLEMENTED, SUPPORTED, REQUIRES_PERMISSION, REQUIRES_PRIVILEGED_PERMISSION };
enum Fragmentation { BUILDTIME, RUNTIME };

class Capability {
private:
	string fName;
	CapabilityState fState;
	Fragmentation fFragmentation;
public:
	Capability() : fName(""), fState(UNSUPPORTED), fFragmentation(RUNTIME) { }
	Capability(string name, CapabilityState state, Fragmentation fragmentation) :
		fName(name), fState(state), fFragmentation(fragmentation) { }
	string getName() { return fName; }
	CapabilityState getState() { return fState; }
	Fragmentation getFragmentation() { return fFragmentation; }
	bool matchCapability(string statePattern);
	void toXML(XMLWriter& output);
};

class Profile {
private:
	string fProfileName;
	string fFamily;
	string fVariant;
	string fRuntime;
	Profile* fParent;
	bool fIsAbstract;
	map<string, Capability> fCapabilities;
public:
	Profile(string profileName) : fProfileName(profileName), fParent(NULL) { }
	string getProfileName() { return fProfileName; }
	void setParent(Profile* parent) { fParent = parent; }
	void setFamily(string family) { fFamily = family; }
	void setVariant(string variant) { fVariant = variant; }
	void setRuntime(string runtime) { fRuntime = runtime; }
	void setAbstract(bool isAbstract) { fIsAbstract = isAbstract; }
	bool isAbstract() { return fIsAbstract; }
	void addCapability(Capability capability);
	Profile* getParent() { return fParent; }
	set<string> getCapabilities();
	Capability getCapability(string capabilityName);
	void toXML(XMLWriter& output);
};

class ProfileDB {
private:
	string fExcludePattern;
	string profilesdir();
	bool isExcluded(Profile* profile);
	bool parseProfileXML(Profile* profile, set<string> alreadyFound);
	void getProfiles(string profilePattern, vector<Profile*>& profiles);
	void matchProfile(string profileName, vector<Capability> requiredCapabilities,
			vector<Capability> optionalCapabilities, vector<Profile*>& result);
	void dumpProfile(Profile* profile, string profileName);
	void dumpProfiles(vector<Profile*> profile, string unmatchedName);
public:
	ProfileDB() { };
	void setExcluded(string excludePattern);
	void getProfiles(string profileName);
	void listProfiles(string pattern, bool onlyFamilies);
	void listCapabilities(string pattern);
	bool matchProfiles(string profilePattern, vector<Capability> requiredCapabilities,
			vector<Capability> optionalCapabilities);
	Profile* findProfile(string profileName, set<string> alreadyFound);
};

#endif /* PROFILEDB_H_ */
