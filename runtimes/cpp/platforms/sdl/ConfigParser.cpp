/* Copyright (C) 2009 Mobile Sorcery AB

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

#include "ConfigParser.h"

#include <expat.h>
#include <map>
#include <stdexcept>
#include <string>

#include "config_platform.h"
#include <helpers/log.h>

#include <helpers/constSets.h>

using namespace std;

static char *readFileIntoMem(const char* filename, int *len) {
	FILE *file = fopen(filename, "rb");
	if(!file)
		return NULL;
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *memory = new char[length];
	if(fread(memory, length, 1, file) != 1)
		return NULL;
	fclose(file);
	*len = length;
	return memory;
}

enum ConfigState {
	STATE_NONE,
	STATE_SKINS,
	STATE_SKIN,
	STATE_KEY
};

#define MAK_ELEM(name) pair<std::string, int>("MAK_" #name, MAK_##name),

pair<std::string, int> sMaks[] = {
	CONSTSET_MAK_(MAK_ELEM, MAK_ELEM)
};

map<std::string, int> sMakMap(sMaks, sMaks + (sizeof(sMaks)/sizeof(pair<std::string, int>)));


static ConfigState sConfigState = STATE_NONE;
static map< string, map<string, Skin*> > sSkins;
static std::string sConfigPath;

static string getValidatedAttribute(const map<string, string>& attr, const string& name) {
	map<string, string>::const_iterator i = attr.find(name);
	if(i == attr.end()) throw logic_error("xml parse error");
	else return i->second;
}

static Skin* sSkin;

static void start(void *data, const char *el, const char **attr) {
	string element = el;
	map<string, string> attributes;
	for (int i = 0; attr[i]; i += 2) {
		attributes[attr[i]] = attr[i+1];
	}

	if(element == "skins") {
		if(sConfigState != STATE_NONE) throw logic_error("xml parse error");
		sConfigState = STATE_SKINS;
	} else
	if(element == "skin") {
		if(sConfigState != STATE_SKINS) throw logic_error("xml parse error");
		sConfigState = STATE_SKIN;
		
		sSkin = new Skin;

		sSkin->model = getValidatedAttribute(attributes, "model");
		sSkin->vendor = getValidatedAttribute(attributes, "vendor");
		sSkin->screenX = atoi(getValidatedAttribute(attributes, "screenX").c_str());
		sSkin->screenY = atoi(getValidatedAttribute(attributes, "screenY").c_str());
		sSkin->screenW = atoi(getValidatedAttribute(attributes, "screenW").c_str());
		sSkin->screenH = atoi(getValidatedAttribute(attributes, "screenH").c_str());
		sSkin->selectedImage = sConfigPath+"/"+getValidatedAttribute(attributes, "selectedImage");
		sSkin->unselectedImage = sConfigPath+"/"+getValidatedAttribute(attributes, "unselectedImage");
		sSkins[sSkin->vendor][sSkin->model] = sSkin;
	} else
	if(element == "key") {
		if(sConfigState != STATE_SKIN) throw logic_error("xml parse error");
		sConfigState = STATE_KEY;
		int index2 = sSkin->keys.size();
		sSkin->keys.resize(index2+1);
		sSkin->keys[index2].x = atoi(getValidatedAttribute(attributes, "x").c_str());
		sSkin->keys[index2].y = atoi(getValidatedAttribute(attributes, "y").c_str());
		sSkin->keys[index2].w = atoi(getValidatedAttribute(attributes, "w").c_str());
		sSkin->keys[index2].h = atoi(getValidatedAttribute(attributes, "h").c_str());
		sSkin->keys[index2].keyCode = sMakMap[getValidatedAttribute(attributes, "code")];
	}
}

static void end(void *data, const char *el) {
	string element = el;
	if(element == "skins") {
		if(sConfigState != STATE_SKINS) throw logic_error("xml parse error");
		sConfigState = STATE_NONE;
	} else
	if(element == "skin") {
		if(sConfigState != STATE_SKIN) throw logic_error("xml parse error");
		sConfigState = STATE_SKINS;
	} else
	if(element == "key") {
		if(sConfigState != STATE_KEY) throw logic_error("xml parse error");
		sConfigState = STATE_SKIN;		
	}
}

Skin* chooseSkin(const std::string& model, const std::string& vendor) {
	if(sSkins[vendor][model] != NULL) return sSkins[vendor][model];
	else if(sSkins[vendor]["default"] != NULL) return sSkins[vendor]["default"];
	else if(sSkins["default"]["default"] != NULL) return sSkins["default"]["default"];
	else return NULL;
}

// fix panic code for failure to load config file?
bool parseConfig(const std::string& configPath) {
	try {
		XML_Parser xmlParser = XML_ParserCreate("UTF-8");
		XML_SetElementHandler(xmlParser, start, end);
		int fileLength=0;	//initializer works around a bug in gcc 4.3.2
		sConfigPath = configPath;
		char *file = readFileIntoMem((configPath+"/more_config.xml").c_str(), &fileLength);
		if(file == NULL)
			throw logic_error("file read fail");
		XML_Parse(xmlParser, file, fileLength, 1);
		delete file;
	} catch (exception& e) {
		LOG(e.what());
		return false;
	}

	return true;
}
