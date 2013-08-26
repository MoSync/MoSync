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

#ifndef PROFILEDB_H_
#define PROFILEDB_H_

#include <map>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include "File.h"
#include "XMLWriter.h"

using namespace std;

class ProfileDB;

enum CapabilityState {
	UNSUPPORTED,
	NOT_IMPLEMENTED,
	SUPPORTED,
	REQUIRES_PERMISSION,
	REQUIRES_PRIVILEGED_PERMISSION
};
enum Fragmentation {
	BUILDTIME, RUNTIME
};

class Capability {
private:
	string fName;
	string fType;
	string fValue;
	CapabilityState fState;
	Fragmentation fFragmentation;
public:
	Capability() :
		fName(""), fValue(""), fState(UNSUPPORTED), fFragmentation(RUNTIME) {
	}
	Capability(string name, string type, string value, CapabilityState state,
	        Fragmentation fragmentation) :
		fName(name), fType(type), fValue(value), fState(state),
		        fFragmentation(fragmentation) {
	}
	string getName() {
		return fName;
	}
	CapabilityState getState() {
		return fState;
	}
	Fragmentation getFragmentation() {
		return fFragmentation;
	}
	string getValue() {
		return fValue;
	}
	string getType() {
		return fType;
	}
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
	Profile(string profileName) :
		fProfileName(profileName), fParent(NULL), fIsAbstract(false) {
	}
	string getProfileName() {
		return fProfileName;
	}
	void setParent(Profile* parent) {
		fParent = parent;
	}
	Profile* getParent() {
		return fParent;
	}
	void setFamily(string family) {
		fFamily = family;
	}
	string getFamily() {
		return fFamily;
	}
	void setVariant(string variant) {
		fVariant = variant;
	}
	string getVariant() {
		return fVariant;
	}
	void setRuntime(string runtime) {
		fRuntime = runtime;
	}
	string getRuntime() {
		return fRuntime;
	}
	void setAbstract(bool _isAbstract) {
		fIsAbstract = _isAbstract;
	}
	bool isAbstract() {
		return fIsAbstract;
	}
	void addCapability(Capability capability);
	set<string> getCapabilities();
	Capability getCapability(string capabilityName);
	void toXML(XMLWriter& output, bool includeCapabilities);
};

class ProfileDB {
private:
	string fExcludePattern;
	bool fOutputMappings;
	bool fBrief;
	bool fIncludeCapabilities;
	map<string, Profile*> fFoundProfiles;
	string profilesdir();
	bool isExcluded(Profile* profile);
	bool parseProfileXML(Profile* profile, set<string> alreadyFound);
	void innerListAllProfiles(File& root, string name, string pattern,
	        bool onlyFamilies, vector<string>& result);
	void getProfiles(string profilePattern, vector<Profile*>& profiles);
	void matchProfile(string profileName,
	        vector<Capability> requiredCapabilities,
	        vector<Capability> optionalCapabilities, vector<Profile*>& result,
	        map<string, Profile*>& profileMappings);
	bool internalMatchProfile(Profile* profile,
			vector<Capability>& requiredCapabilites,
			vector<Capability>& optionalCapabilites,
			string& matchToken);
	//void dumpProfile(Profile* profile, string profileName);
	void dumpProfiles(vector<Profile*> profile,
	        map<string, Profile*> profileMappings, string unmatchedName);
	void dumpProfilesXML(vector<Profile*> profiles,
	        map<string, Profile*> profileMappings, string profileName);
	void dumpProfilesBrief(vector<Profile*> profiles,
	        map<string, Profile*> profileMappings, string profileName);
	void dumpProfileMapping(XMLWriter* writer, string profileName,
	        string mappedToProfileName);
	void dumpProfileMappings(XMLWriter* writer,
	        map<string, Profile*> profileMappings);
public:
	ProfileDB() : fOutputMappings(false), fBrief(false), fIncludeCapabilities(true) {
	}
	;
	virtual ~ProfileDB();
	void setBrief(bool brief) {
		fBrief = brief;
	}
	void setIncludeCapabilities(bool includeCapabilities) {
		fIncludeCapabilities = includeCapabilities;
	}
	void setExcluded(string excludePattern);
	void setOutputMappings(bool outputMappings) {
		fOutputMappings = outputMappings;
	}
	void getProfiles(string profileName);
	void listProfiles(string pattern, bool onlyFamilies);
	void listCapabilities(string pattern);
	void listRuntime(string profileName);
	bool matchProfiles(string profilePattern,
	        vector<Capability> requiredCapabilities,
	        vector<Capability> optionalCapabilities);
	Profile* findProfile(string profileName);
	Profile* findProfile(string profileName, set<string> alreadyFound);
	string profilesdir(string family);
};

// XML parsing decl's
struct ParserState {
	Profile* profile;
	ProfileDB* db;
	int lineNo;
	string fileName;
	set<string> alreadyFound;
	vector<Capability> capabilityStack;
};
//static void xmlStart(void *data, const char *tagName, const char **attributes);
//static void xmlEnd(void *data, const char *tagName);

// Utility functions
//static bool isWildcard(string pattern);
//static bool equalsIgnoreCaseASCII(string s1, string s2);
//static bool beginsWith(string prefix, string str);
//static bool matches(string pattern, string name);
//static const char* findAttr(const char* name, const char** attributes);
//static void error(const char* file, int lineNo, string msg) __attribute__ ((noreturn));
//static void error(ParserState* state, string msg);
//static string getStateString(CapabilityState state);
//static string getCapabilityPrefix(vector<Capability>& capabilityStack);

#endif /* PROFILEDB_H_ */
