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

using namespace std;

#define ELEMENT_PLATFORM "platform"
#define ATTR_INHERIT "inherit"
#define ATTR_FAMILY "family"
#define ATTR_VARIANT "variant"

struct ParserState {
	Profile* profile;
	ProfileDB* db;
	int lineNo;
	string fileName;
	set<string> alreadyFound;
};

string ProfileDB::profilesdir() {
	static const char* md = NULL;
	if(!md) {
		md	= getenv("MOSYNCDIR");
		if(!md) {
			printf("MOSYNCDIR is not set!\n");
			exit(1);
		}
	}
	return string(md) + "/profiles/platforms";
}

static bool matches(string pattern, string name) {
	bool isWildcard = pattern.at(pattern.length() - 1) == '*';
	if (isWildcard) {
		return name.length() > pattern.length() && name.substr(0, pattern.length() - 1) == pattern.substr(0, pattern.length() - 1);
	} else {
		return name == pattern;
	}
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

void innerListAllProfiles(File& root, string name, string pattern, vector<string>& result) {
	list<File> files = root.listFiles();
	for (list<File>::iterator filesIterator = files.begin(); filesIterator != files.end(); filesIterator++) {
		File file = *filesIterator;
		if (file.isDirectory()) {
			const char* slash = name.size() == 0 ? "" : "/";
			innerListAllProfiles(file, name + slash + file.getName(), pattern, result);
		} else if (string("profile.xml") == file.getName() && matches(pattern, name)) {
			result.push_back(name);
		}
	}
}

void ProfileDB::listProfiles(string pattern) {
	File root = File(profilesdir());
	vector<string> result;
	innerListAllProfiles(root, string(), pattern, result);

	ostringstream output;

	for (size_t i = 0; i < result.size(); i++) {
		string profile = result.at(i);
		output << profile << '\n';
	}

	printf("%s", output.str().c_str());
}

void ProfileDB::getProfile(string profileName) {
	ostringstream output;
	output << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";

	output << "<db version=\"1.0\" id=\"main\">\n";

	string line;

	Profile* profile = findProfile(profileName, set<string>());
	if (profile) {
		profile->toXML(output);
	} else {
		output << "\t<error>Could find no profile for " << profileName << "</error>\n";
	}

	output << "<db>\n";

	printf("%s", output.str().c_str());
}

static void xmlStart(void *data, const char *tagName, const char **attributes) {
	ParserState* state = (ParserState*) data;
	ProfileDB* db = state->db;
	Profile* profile = state->profile;

	if (!strcmp(ELEMENT_PLATFORM, tagName)) {
		const char* parentProfile = findAttr(ATTR_INHERIT, attributes);
		if (parentProfile) {
			Profile* parent = db->findProfile(parentProfile, state->alreadyFound);
			if (parent == NULL) {
				error(state, string("This platform element refers to an invalid parent profile: ") + parentProfile);
			} else {
				profile->setParent(parent);
			}
		}
		const char* family = findAttr(ATTR_FAMILY, attributes);
		if (!family) {
			error(state, string("No family attribute defined: ") + parentProfile);
		}
		const char* variant = findAttr(ATTR_VARIANT, attributes);
		if (!variant) {
			error(state, string("No variant attribute defined: ") + parentProfile);
		}

		profile->setFamily(string(family));
		profile->setVariant(string(variant));
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
	File file = File(string(profilesdir() + "/" + profileName + "/profile.xml"));
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
				printf("<!-- FATAL ERROR: XML malformatted (%s, line %d) -->", path.c_str(), state.lineNo);
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

Capability Profile::getCapability(string capabilityName) {
	map<string, Capability>::iterator capability = fCapabilities.find(capabilityName);
	if (capability == fCapabilities.end()) {
		if (fParent) {
			fParent->getCapability(capabilityName);
		}
		return Capability();
	} else {
		return fCapabilities[capabilityName];
	}
}

void Profile::toXML(ostringstream& output) {
	output << "<platform family=\"" << fFamily << "\" variant=\"" << fVariant << "\"";
	if (fParent) {
		output << " inherit=\"" << fParent->fProfileName << "\"";
	}
	output << ">\n";

	output << "</platform>\n";
}
