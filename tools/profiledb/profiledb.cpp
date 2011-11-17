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

#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <ostream>
#include <sstream>
#include <expat.h>
#include <stdio.h>
#include "File.h"
#include "profiledb.h"
#include "XMLWriter.h"

using namespace std;

#define ELEMENT_PLATFORM "platform"
#define ELEMENT_CAPABILITY "capability"
#define ATTR_INHERIT "inherit"
#define ATTR_FAMILY "family"
#define ATTR_VARIANT "variant"
#define ATTR_RUNTIME "runtime"
#define ATTR_ABSTRACT "abstract"
#define ATTR_NAME "name"
#define ATTR_STATE "state"
#define ATTR_FRAGMENTATION "fragmentation"

struct ParserState {
	Profile* profile;
	ProfileDB* db;
	int lineNo;
	string fileName;
	set<string> alreadyFound;
};

string ProfileDB::profilesdir() {
	static const char* md = NULL;
	if (!md) {
		md = getenv("MOSYNCDIR");
		if (!md) {
			printf("MOSYNCDIR is not set!\n");
			exit(1);
		}
	}
	return string(md) + "/profiles/platforms";
}

static bool isWildcard(string pattern) {
	return pattern.length() > 0 && pattern.at(pattern.length() - 1) == '*';
}

static bool equalsIgnoreCaseASCII(string s1, string s2) {
	if (s1.length() == s2.length()) {
		for (size_t i = 0; i < s1.length(); i++) {
			if (tolower(s1.at(i)) != tolower(s2.at(i))) {
				return false;
			}
		}
		return true;
	}
	return false;
}

static bool beginsWith(string prefix, string str) {
	bool result = str.length() >= prefix.length() &&
			equalsIgnoreCaseASCII(prefix, str.substr(0, prefix.length()));
	return result;
}

static bool matches(string pattern, string name) {
	size_t ix = 0;
	size_t newIx = 0;
	while (newIx != string::npos) {
		newIx = pattern.find_first_of(',', ix);
		size_t endIx = newIx == string::npos ? pattern.length() : newIx;
		string subPattern = pattern.substr(ix, endIx - ix);
		bool wildcard = isWildcard(subPattern);
		if (wildcard && beginsWith(
				subPattern.substr(0, subPattern.length() - 1), name)) {
			return true;
		} else if (equalsIgnoreCaseASCII(subPattern, name)) {
			return true;
		}
		ix = newIx + 1;
	}
	return false;
}

static const char* findAttr(const char* name, const char** attributes) {
	for (int i = 0; attributes[i]; i += 2) {
		if (!strcmp(attributes[i], name)) {
			return attributes[i + 1];
		}
	}
	return NULL;
}

static void error(const char* file, int lineNo, string msg) {
	ostringstream errMsg;
	if (file) {
		errMsg << file << ":" << lineNo << ": ";
	}
	errMsg << "ERROR: " << msg << '\n';
	printf(errMsg.str().c_str(), "");
	exit(1);
}

static void error(ParserState* state, string msg) {
	if (state) {
		error(state->fileName.c_str(), state->lineNo, msg);
	} else {
		error(NULL, -1, msg);
	}
}

void innerListAllProfiles(File& root, string name, string pattern,
		bool onlyFamilies, vector<string>& result) {
	list<File> files = root.listFiles();
	for (list<File>::iterator filesIterator = files.begin(); filesIterator
			!= files.end(); filesIterator++) {
		File file = *filesIterator;
		if (file.isDirectory()) {
			const char* slash = name.size() == 0 ? "" : "/";
			string newName = name.size() == 0 || !onlyFamilies ? name + slash
					+ file.getName() : name;
			innerListAllProfiles(file, newName, pattern, onlyFamilies, result);
		} else if (string("profile.xml") == file.getName() && matches(pattern,
				name)) {
			result.push_back(name);
		}
	}
}

void ProfileDB::listProfiles(string pattern, bool onlyFamilies) {
	File root = File(profilesdir());
	vector<string> result;
	innerListAllProfiles(root, string(), pattern, onlyFamilies, result);

	ostringstream output;

	set<string> families;
	for (size_t i = 0; i < result.size(); i++) {
		string profile = result.at(i);
		if (families.find(profile) == families.end()) {
			families.insert(profile);
			output << profile << '\n';
		}
	}

	printf("%s", output.str().c_str());
}

void ProfileDB::dumpProfile(Profile* profile, string profileName) {
	vector<Profile*> profiles;
	if (profile) {
		profiles.push_back(profile);
	}
	dumpProfiles(profiles, profileName);
}

void ProfileDB::dumpProfiles(vector<Profile*> profiles, string profileName) {
	ostringstream output;
	XMLWriter writer = XMLWriter(&output);
	writer.start();
	writer.startTag("db");
	writer.setAttr("version", "1.0");
	writer.setAttr("id", "main");

	string line;

	if (profiles.size() > 0) {
		for (size_t i = 0; i < profiles.size(); i++) {
			Profile* profile = profiles.at(i);
			if (profile) {
				profile->toXML(writer);
			}
		}
	} else {
		writer.startTag("error");
		writer.text(string("Could find no matching profile for ") + profileName);
		writer.endTag();
	}

	writer.endTag();

	printf("%s", output.str().c_str());
}

void ProfileDB::getProfiles(string profilePattern, vector<Profile*>& profiles) {
	File root = File(profilesdir());
	vector<string> profileNames;
	innerListAllProfiles(root, string(), profilePattern, false, profileNames);
	for (size_t i = 0; i < profileNames.size(); i++) {
		string profileName = profileNames.at(i);
		Profile* profile = findProfile(profileName, set<string> ());
		if (profile && !isExcluded(profile)) {
			profiles.push_back(profile);
		}
	}
}

void ProfileDB::setExcluded(string excludePattern) {
	fExcludePattern = excludePattern;
}

void ProfileDB::getProfiles(string profilePattern) {
	vector<Profile*> profiles;
	getProfiles(profilePattern, profiles);
	dumpProfiles(profiles, profilePattern);
}

void ProfileDB::listCapabilities(string statePattern) {
	vector<Profile*> profiles;
	set<string> aggregateCapabilities;
	getProfiles("*", profiles);
	for (size_t i = 0; i < profiles.size(); i++) {
		Profile* profile = profiles.at(i);
		set<string> capabilities = profile->getCapabilities();
		for (set<string>::iterator capabilityIt = capabilities.begin(); capabilityIt
				!= capabilities.end(); capabilityIt++) {
			Capability capability = profile->getCapability(*capabilityIt);
			if (capability.matchCapability(statePattern)) {
				aggregateCapabilities.insert(*capabilityIt);
			}
		}
	}

	ostringstream output;
	XMLWriter writer = XMLWriter(&output);
	writer.start();
	writer.startTag("db");
	writer.setAttr("version", "1.0");
	writer.setAttr("id", "main");
	for (set<string>::iterator capabilityIt = aggregateCapabilities.begin(); capabilityIt
			!= aggregateCapabilities.end(); capabilityIt++) {
		writer.startTag(ELEMENT_CAPABILITY);
		writer.setAttr(ATTR_NAME, *capabilityIt);
		writer.endTag();
	}
	writer.endTag();
	writer.end();
	writer.dump();
}

static void xmlStart(void *data, const char *tagName, const char **attributes) {
	ParserState* state = (ParserState*) data;
	ProfileDB* db = state->db;
	Profile* profile = state->profile;

	if (!strcmp(ELEMENT_PLATFORM, tagName)) {
		const char* parentProfile = findAttr(ATTR_INHERIT, attributes);
		if (parentProfile) {
			Profile* parent = db->findProfile(parentProfile,
					state->alreadyFound);
			if (parent == NULL) {
				error(
						state,
						string(
								"This platform element refers to an invalid parent profile: ")
								+ parentProfile);
			} else {
				profile->setParent(parent);
			}
		}
		const char* family = findAttr(ATTR_FAMILY, attributes);
		if (!family) {
			error(
					state,
					string("No family attribute defined: ")
							+ profile->getProfileName());
		}
		const char* variant = findAttr(ATTR_VARIANT, attributes);
		if (!variant) {
			error(
					state,
					string("No variant attribute defined: ")
							+ profile->getProfileName());
		}
		const char* runtime = findAttr(ATTR_RUNTIME, attributes);
		if (!runtime) {
			error(
					state,
					string("No runtime attribute defined: ")
							+ profile->getProfileName());
		}
		const char* abstract = findAttr(ATTR_ABSTRACT, attributes);
		bool isAbstract = abstract && !strcmp("true", abstract);

		const char* slash = strlen(variant) > 0 ? "/" : "";
		if (!equalsIgnoreCaseASCII(string(family) + slash + string(variant),
				profile->getProfileName())) {
			error(state, string("Expected family (") + family +
					string(") and variant (") + variant + string(") to match ")
					+ profile->getProfileName());
		}
		profile->setFamily(string(family));
		profile->setVariant(string(variant));
		profile->setRuntime(string(runtime));
		profile->setAbstract(isAbstract);
	} else if (!strcmp(ELEMENT_CAPABILITY, tagName)) {
		const char* capabilityName = findAttr(ATTR_NAME, attributes);
		const char* capabilityStateStr = findAttr(ATTR_STATE, attributes);
		const char* fragmentationStr = findAttr(ATTR_FRAGMENTATION, attributes);
		CapabilityState capabilityState;
		if (!capabilityStateStr) {
			capabilityState = UNSUPPORTED;
		} else if (!strcmp(capabilityStateStr, "SUPPORTED")) {
			capabilityState = SUPPORTED;
		} else if (!strcmp(capabilityStateStr, "UNSUPPORTED")) {
			capabilityState = UNSUPPORTED;
		} else if (!strcmp(capabilityStateStr, "NOT_IMPLEMENTED")) {
			capabilityState = NOT_IMPLEMENTED;
		} else if (!strcmp(capabilityStateStr, "REQUIRES_PERMISSION")) {
			capabilityState = REQUIRES_PERMISSION;
		} else if (!strcmp(capabilityStateStr, "REQUIRES_PRIVILEGED_PERMISSION")) {
			capabilityState = REQUIRES_PRIVILEGED_PERMISSION;
		} else {
			error(state, string("Unknown state: ") + capabilityStateStr);
		}
		Fragmentation fragmentation = fragmentationStr && !strcmp("buildtime",
				fragmentationStr) ? BUILDTIME : RUNTIME;
		Capability capability = Capability(capabilityName, capabilityState,
				fragmentation);
		state->profile->addCapability(capability);
	}
}

static void xmlEnd(void *data, const char *tagName) {

}

Profile* ProfileDB::findProfile(string profileName, set<string> alreadyFound) {
	Profile* profile = new Profile(profileName);
	if (alreadyFound.count(profileName) > 0) {
		error(NULL, string("Circular dependency, profile ") + profileName);
	}
	alreadyFound.insert(profileName);
	if (!parseProfileXML(profile, alreadyFound)) {
		delete profile;
		profile = NULL;
	}
	return profile;
}

bool ProfileDB::parseProfileXML(Profile* profile, set<string> alreadyFound) {
	string profileName = profile->getProfileName();
	File file =
			File(string(profilesdir() + "/" + profileName + "/profile.xml"));
	string path = file.getAbsolutePath();

	ParserState state;
	state.db = this;
	state.profile = profile;
	state.lineNo = 0;
	state.fileName = path;
	state.alreadyFound = alreadyFound;

	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, xmlStart, xmlEnd);
	XML_SetUserData(parser, (void*) &state);

	string line;
	ifstream fs(path.c_str());
	if (fs.is_open()) {
		while (!fs.eof()) {
			state.lineNo++;
			getline(fs, line);
			if (!XML_Parse(parser, line.c_str(), line.length(), false)) {
				printf("<!-- FATAL ERROR: XML malformatted (%s, line %d) -->",
						path.c_str(), state.lineNo);
				return NULL;
			}
		}
	} else {
		return NULL;
	}

	XML_Parse(parser, "", 0, true);
	XML_ParserFree(parser);

	return profile;
}

bool internalMatchProfile(Profile* profile,
		vector<Capability>& requiredCapabilites,
		vector<Capability>& optionalCapabilites,
		string& matchToken) {
	bool match = true;
	for (vector<Capability>::iterator capability = requiredCapabilites.begin(); capability
			!= requiredCapabilites.end(); capability++) {
		Capability matchedCapability = profile->getCapability(
				capability->getName());
		CapabilityState matchedState = matchedCapability.getState();
		// Ok, so we don't support this, bail out!
		if (matchedState == NOT_IMPLEMENTED || matchedState == UNSUPPORTED) {
			match = false;
		}
	}

	// So, the optional capabilities are matched in a different way.
	// We add stuff to the matchTokens string to produce a string that
	// is unique to the optional capabilities being matched.
	for (vector<Capability>::iterator capability = optionalCapabilites.begin(); capability
			!= optionalCapabilites.end(); capability++) {
		Capability matchedCapability = profile->getCapability(
				capability->getName());
		CapabilityState matchedState = matchedCapability.getState();
		Fragmentation fragmentation = matchedCapability.getFragmentation();
		bool fragmented = fragmentation == BUILDTIME && (matchedState == NOT_IMPLEMENTED || matchedState == UNSUPPORTED);
		matchToken.append(fragmented ? "+" : "-");
	}
	return match;
}

bool ProfileDB::isExcluded(Profile* profile) {
	return !profile || profile->isAbstract() || matches(fExcludePattern,
			profile->getProfileName());
}

bool ProfileDB::matchProfiles(string profilePattern,
		vector<Capability> requiredCapabilities,
		vector<Capability> optionalCapabilities) {
	File root = File(profilesdir());
	vector<string> profiles;
	innerListAllProfiles(root, string(), profilePattern, false, profiles);
	vector<Profile*> matchingProfiles;

	for (vector<string>::iterator profileIt = profiles.begin(); profileIt
			!= profiles.end(); profileIt++) {
		matchProfile(*profileIt, requiredCapabilities,
				optionalCapabilities, matchingProfiles);
	}

	set<string> profileNames; // TODO: Maybe we want VARIANTS as well!?
	vector<Profile*> prunedProfiles;
	for (vector<Profile*>::iterator matchingProfilesIt =
			matchingProfiles.begin(); matchingProfilesIt
			!= matchingProfiles.end(); matchingProfilesIt++) {
		Profile* match = *matchingProfilesIt;
		// Make sure we only add a specific profile once.
		if (profileNames.find(match->getProfileName())
				== profileNames.end()) {
			profileNames.insert(match->getProfileName());
			prunedProfiles.push_back(match);
		}
	}

	dumpProfiles(prunedProfiles, profilePattern);

	return profileNames.size() > 0;
}

void ProfileDB::matchProfile(string profileName,
		vector<Capability> requiredCapabilities,
		vector<Capability> optionalCapabilities,
		vector<Profile*>& result) {
	Profile* profile = findProfile(profileName, set<string> ());
	vector<Profile*> genealogy;
	while (profile) {
		genealogy.push_back(profile);
		profile = profile->getParent();
	}

	set<string> matchTokens;
	for (vector<Profile*>::reverse_iterator genealogyIt = genealogy.rbegin();
			genealogyIt != genealogy.rend(); genealogyIt++) {
		Profile* profileToMatch = *genealogyIt;
		string matchToken;
		if (!profileToMatch->isAbstract() &&
				internalMatchProfile(profileToMatch, requiredCapabilities, optionalCapabilities, matchToken)) {
			if (matchTokens.find(matchToken) == matchTokens.end()) {
				matchTokens.insert(matchToken);
				result.push_back(profileToMatch);
			}
		}
	}
}

void Profile::addCapability(Capability capability) {
	fCapabilities[capability.getName()] = capability;
}

set<string> Profile::getCapabilities() {
	set<string> result;
	for (map<string, Capability>::iterator capability = fCapabilities.begin(); capability
			!= fCapabilities.end(); capability++) {
		result.insert(capability->first);
	}
	if (fParent) {
		set<string> parentCapabilities = fParent->getCapabilities();
		result.insert(parentCapabilities.begin(), parentCapabilities.end());
	}
	return result;
}

Capability Profile::getCapability(string capabilityName) {
	map<string, Capability>::iterator capability = fCapabilities.find(
			capabilityName);
	if (capability == fCapabilities.end()) {
		if (fParent) {
			return fParent->getCapability(capabilityName);
		}
		return Capability();
	} else {
		return fCapabilities[capabilityName];
	}
}

void Profile::toXML(XMLWriter& writer) {
	writer.startTag(ELEMENT_PLATFORM);
	writer.setAttr(ATTR_FAMILY, fFamily);
	writer.setAttr(ATTR_VARIANT, fVariant);
	writer.setAttr(ATTR_RUNTIME, fRuntime);
	if (fIsAbstract) {
		writer.setAttr(ATTR_ABSTRACT, "true");
	}
	set<string> capabilities = Profile::getCapabilities();
	for (set<string>::iterator capability = capabilities.begin(); capability
			!= capabilities.end(); capability++) {
		Capability value = getCapability(*capability);
		value.toXML(writer);
	}
	writer.endTag();
}

string getStateString(CapabilityState state) {
	switch (state) {
	case SUPPORTED:
		return "SUPPORTED";
		break;
	case UNSUPPORTED:
		return "UNSUPPORTED";
		break;
	case NOT_IMPLEMENTED:
		return "NOT_IMPLEMENTED";
		break;
	case REQUIRES_PERMISSION:
		return "REQUIRES_PERMISSION";
		break;
	case REQUIRES_PRIVILEGED_PERMISSION:
		return "REQUIRES_PRIVILEGED_PERMISSION";
		break;
	}
	return "";
}

void Capability::toXML(XMLWriter& writer) {
	string stateStr = getStateString(fState);

	writer.startTag(ELEMENT_CAPABILITY);
	writer.setAttr(ATTR_NAME, fName);
	writer.setAttr(ATTR_STATE, stateStr);
	writer.setAttr(ATTR_FRAGMENTATION,
			fFragmentation == BUILDTIME ? "buildtime" : "runtime");
	writer.endTag();
}

bool Capability::matchCapability(string statePattern) {
	return matches(statePattern, getStateString(fState));
}
