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
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <fstream>
#include <ostream>
#include <sstream>
#include <expat.h>
#include <stdio.h>
#include <cctype>
#include <algorithm>
#include "File.h"
#include "xlstcomp.h"
#include "resdirectives.h"

using namespace std;

#define XML_BUFFER_SIZE 4096
#define MAIN_TAG "resources"
#define TAG_CONDITION "condition"
#define ATTR_PLATFORM "platform"
#define ATTR_SCREENSIZE "screenSize"
#define ATTR_LOCALE "locale"

#define RES_IMAGE "image"
#define RES_AUDIO "audio"
#define RES_MEDIA "media"
#define RES_STRING "string"
#define RES_BINARY "binary"
#define RES_PLACEHOLDER "placeholder"


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

static void error(ParserState* state, const char* msg) {
	error(state, string(msg));
}

string getVariantStr(string variant) {
	if (variant.size() == 0) {
		return string("(fallback variant)");
	} else {
		return variant;
	}
}

VariantResourceSet::VariantResourceSet() {
}

VariantResourceSet::~VariantResourceSet() {

}

void VariantResourceSet::assignVirtualIndex(string resId, int virtualIndex) {
	fVirtualResourceIndices[virtualIndex] = resId;
}

void VariantResourceSet::assignMappedIndex(string resId, string variant, int mappedIndex) {
	fMappedResources[variant][resId] = mappedIndex;
}

void VariantResourceSet::assignResAndLoadType(int resIndex, int resType, LoadType loadType) {
	fResTypes[resIndex] = resType;
	fLoadTypes[resIndex] = loadType;
}

bool VariantResourceSet::addDirective(ResourceDirective* directive, VariantCondition* cond) {
	bool add = directive != NULL;
	if (add) {
		string variant = cond == NULL ? string() : cond->getVariantIdentifier(true);
		string resId = directive->getId();
//		printf("Adding %s to variant %s\n", resId.c_str(), getVariantStr(variant).c_str());
		if (variant.size() > 0) { // Is it a variant resource?
			fVariantResIds.push_back(resId);
		} else {
			fNonVariantResIds.push_back(resId);
		}
		fVariants.push_back(variant);
		if (getDirective(resId, variant) && variant.size() > 0) {
			error(directive->getFile().c_str(),
					directive->getLineNo(),
					(string("Duplicate definition of resource ") + resId + " for variant " + variant).c_str());
		}
		fDirectives[variant][resId] = directive;
		fMappedPriorities[variant][resId] = fCurrentPriority[resId];
		fCurrentPriority[resId] = fCurrentPriority[resId] + 1;
	}
	return add;
}

bool VariantResourceSet::assignPriority(ResourceDirective* directive, VariantCondition* cond, int priority) {
	bool assign = directive != NULL;
	if (assign) {
		string variant = cond == NULL ? string() : cond->getVariantIdentifier(true);
		string resId = directive->getId();
		fMappedPriorities[variant][resId] = priority;
//		printf("Setting priority %d to resource %s and variant %s\n", priority, resId.c_str(), variant.c_str());
	}
	return assign;
}

ResourceDirective* VariantResourceSet::getDirective(string resId, string variant) {
	return fDirectives[variant][resId];
}

// Removes duplicates, but keeps the order.
static vector<string> removeDuplicates(vector<string> v) {
	set<string> alreadyAdded;
	vector<string> result;
	for(vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
		string key = *it;
		if (alreadyAdded.count(key) == 0) {
			alreadyAdded.insert(key);
			result.push_back(key);
		}
	}
	return result;
}

vector<string> VariantResourceSet::getAllVariants() {
	return removeDuplicates(fVariants);
}

vector<string> VariantResourceSet::getAllVariantResIds() {
	return removeDuplicates(fVariantResIds);
}

vector<string> VariantResourceSet::getAllNonVariantResIds() {
	vector<string> result;
	vector<string> variants = getAllVariants();
	for(vector<string>::iterator nonVariantResId = fNonVariantResIds.begin(); nonVariantResId != fNonVariantResIds.end(); ++nonVariantResId) {
		bool nonVariant = true;
		for (vector<string>::iterator variantIt = variants.begin(); variantIt != variants.end(); ++variantIt) {
			string variant = *variantIt;
			if (variant.size() > 0 && getDirective(*nonVariantResId, variant)) {
				nonVariant = false;
			}
		}
		if (nonVariant) {
			result.push_back(*nonVariantResId);
		}
	}
	return removeDuplicates(result);
}

static FileResourceDirective* createFileDirective(const char* tagName) {
	if (!strcmp(RES_BINARY, tagName)) {
		return new BinaryResourceDirective();
	} else if (!strcmp(RES_IMAGE, tagName)) {
		return new ImageResourceDirective();
	} else if (!strcmp(RES_MEDIA, tagName)) {
		return new MediaResourceDirective();
	} else if (!strcmp(RES_AUDIO, tagName)) {
		return new AudioResourceDirective();
	}
	return NULL;
}

static ResourceDirective* createDirective(const char* tagName) {
	FileResourceDirective* fileDirective = createFileDirective(tagName);
	if (fileDirective) {
		return fileDirective;
	} else if (!strcmp(RES_STRING, tagName)) {
		return new StringResourceDirective();
	} else if (!strcmp(RES_PLACEHOLDER, tagName)) {
		return new PlaceholderDirective();
	}
	return NULL;
}

static void disposeDirective(ResourceDirective* directive) {
	if (directive) {
		delete directive;
	}
}

static bool isResource(const char* tagName) {
	// Lazy me :)
	ResourceDirective* directive = createDirective(tagName);
	bool result = directive != NULL;
	disposeDirective(directive);
	return result;
}

static void xlstStart(void *data, const char *tagName, const char **attributes) {
	ParserState* state = (ParserState*) data;
	if (!strcmp(MAIN_TAG, tagName)) {
		state->started = true;
		return;
	}
	if (!state->started) {
		error(state, "The main tag must be <resources>");
	}

	if (!strcmp(TAG_CONDITION, tagName)) {
		VariantCondition condition = VariantCondition(state->platform);
		if (!state->conditionStack.empty()) {
			VariantCondition parent = state->conditionStack.top();
			condition.initFrom(parent);
		}
		for (int i = 0; attributes[i]; i += 2) {
			const char* name = attributes[i];
			const char* value = attributes[i + 1];
			condition.setCondition(name, value);
		}
		state->conditionStack.push(condition);
	} else {
		ResourceDirective* directive = createDirective(tagName);
		if (directive) {
			directive->initDirectiveFromAttributes(attributes);
			directive->setFile(state->fileName);
			directive->setLineNo(state->lineNo);
			string id = directive->getId();
			state->currentId = id;
			if (state->currentId.size() == 0) {
				error(state, "No id attribute in resource tag " + string(tagName) + " (or a parent resource tag)");
			}
			VariantCondition* cond = state->conditionStack.empty() ? NULL : &(state->conditionStack.top());
			bool shouldAdd = !cond || cond->isApplicable();
			if (shouldAdd && state->resourceSet->addDirective(directive, cond)) {
				state->currentDirective = directive;
			} else {
				// Just remove it at once.
				disposeDirective(directive);
			}
		} else {
			error(state, "Invalid tag: " + string(tagName));
		}
	}
}

static void xlstEnd(void *data, const char *tagName) {
	ParserState* state = (ParserState*) data;
	if (!strcmp(MAIN_TAG, tagName)) {
		state->started = false;
	} else if (!strcmp(TAG_CONDITION, tagName)) {
		state->conditionStack.pop();
	} else if (isResource(tagName)) {
		state->currentDirective = NULL;
	}
}

static void xlstCDATA(void *data, const char *content, int length) {
	ParserState* state = (ParserState*) data;
	ResourceDirective* directive = state->currentDirective;
	if (directive) {
		printf("%s", directive->getId().c_str());
		directive->initDirectiveFromCData(content, length);
	}
}

void VariantResourceSet::parseLSTX(string inputFile) {
	char buf[XML_BUFFER_SIZE];

	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, xlstStart, xlstEnd);
	XML_SetCharacterDataHandler(parser, xlstCDATA);
	ParserState* state = new ParserState();
	state->platform = fPlatform;
	state->started = false;
	state->resourceSet = this;
	state->currentDirective = NULL;
	XML_SetUserData(parser, (void*) state);

	string line;
	state->fileName = inputFile;
	state->lineNo = 0;
	ifstream fs(inputFile.c_str());
	if (fs.is_open()) {
		while (!fs.eof()) {
			state->lineNo++;
			getline(fs, line);
			if (!XML_Parse(parser, line.c_str(), line.length(), false)) {
				error(state, "XML malformatted!");
			}
		}
	} else {
		error(state, "Failed to read input LSTX file.");
	}

	XML_Parse(parser, buf, 0, true);
	XML_ParserFree(parser);

	delete state;
}

const char* mosyncdir() {
	static const char* md = NULL;
	if(!md) {
		md	= getenv("MOSYNCDIR");
		if(!md) {
			printf("MOSYNCDIR is not set!\n");
			exit(1);
		}
	}
	return md;
}

string VariantResourceSet::parseLST(string lstFile, string outputDir) {
	ostringstream pipetoolCmd;
	string output = outputDir + "/resources";
	string deps = outputDir + "/resources.deps";
	pipetoolCmd << mosyncdir() << "/bin/pipe-tool -R -depend=\"" << deps << "\" \"" << output << "\" \"" << lstFile << "\"";
	printf("%s\n", pipetoolCmd.str().c_str());
	int res = system(pipetoolCmd.str().c_str());
	if (res) {
		error(NULL, "Resource compilation failed");
	}
	return output;
}

const char* getDefaultVariantAttr(const char* resourceType) {
	if (!strcmp(RES_BINARY, resourceType)) {
		return ATTR_PLATFORM;
	} else if (!strcmp(RES_IMAGE, resourceType)) {
		return ATTR_SCREENSIZE;
	} else if (!strcmp(RES_MEDIA, resourceType)) {
		return ATTR_PLATFORM;
	} else if (!strcmp(RES_AUDIO, resourceType)) {
		return ATTR_PLATFORM;
	} else if (!strcmp(RES_STRING, resourceType)) {
		return ATTR_LOCALE;
	}
	return NULL;
}

bool isStandardVariantAttr(const char* variantAttr) {
	bool nope = strcmp(ATTR_PLATFORM, variantAttr) &&
			strcmp(ATTR_LOCALE, variantAttr) &&
			strcmp(ATTR_SCREENSIZE, variantAttr);
	return !nope;
}

static string toResName(string filename) {
	string resName(filename);
	int lastDot = filename.size();
	for (size_t i = 0; i < resName.size(); i++) {
		char ch = resName.at(i);
		resName[i] = toupper(ch);
		if (ch == '.') {
			lastDot = i;
		}
	}
	// TODO: Make sure these are valid C identifiers!
	return resName.substr(0, lastDot);
}

void VariantResourceSet::scanForResources(string directoryToScan) {
	scanForResources(directoryToScan, string(), string(), string(), LoadType_Startup, VariantCondition(fPlatform), 0);
}

static LoadType getLoadType(string lcaseFilename) {
	if (lcaseFilename.size() > 0 && lcaseFilename.at(0) == 'u' && isResource(lcaseFilename.substr(1, lcaseFilename.size() - 1).c_str())) {
		return LoadType_Unloaded;
	}
	return LoadType_Startup;
}

static string getResourceType(string lcaseFilename) {
	if (getLoadType(lcaseFilename) == LoadType_Unloaded) {
		return lcaseFilename.substr(1, lcaseFilename.size() - 1);
	}
	return lcaseFilename;
}

bool VariantResourceSet::isAmbiguous(string resId) {
	return false;
}

void VariantResourceSet::scanForResources(string directoryToScan,
		string resourceType,
		string variantAttrName,
		string variantAttrValue,
		LoadType loadType,
		VariantCondition condition,
		int priority) {
	// The structure is like this:
	//
	// /
	// +--/image <-- The image's default variant attr is 'screenSize'
	// |  |
	// |  +--/320x240 <-- Here, the variant is screenSize:320x240
	// |  |
	// |  +--/locale <-- But a subdir can indicate another variant attr
	// |     |
	// |     +--/en
	// |     |
	// |     ...
	// |
	// +--/string
	// |
	// ...

	bool resTypeAssigned = resourceType.size() > 0;
//	printf("Loadtype for dir %s: %d\n", directoryToScan.c_str(), loadType);
	File dir = File(directoryToScan);
	list<File> files = dir.listFiles();
	for (list<File>::iterator fileIt = files.begin(); fileIt != files.end(); fileIt++) {
		File file = *fileIt;
		string filename = file.getName();
		string lcaseName = file.getName();
		// Lower-case it.
		for (size_t i = 0; i < lcaseName.size(); i++) {
			lcaseName[i] = tolower(lcaseName.at(i));
		}
		if (file.isDirectory()) {
			string newDirectory = directoryToScan + F_SEPERATOR + filename;
			LoadType newLoadType = getLoadType(lcaseName);
			string potentialResourceType = getResourceType(lcaseName);
			if (!resTypeAssigned && isResource(potentialResourceType.c_str())) {
				// Is it the top level, ie image, binary, etc?
				loadType = newLoadType;
				resourceType = potentialResourceType;
				variantAttrName = getDefaultVariantAttr(resourceType.c_str());
			} else if (isStandardVariantAttr(lcaseName.c_str())) {
				// Or is it a standard variant attr?
				variantAttrName = lcaseName;
			} else {
				// Otherwise, it is the variant attr value.
				variantAttrValue = lcaseName;
			}
			VariantCondition newCondition = VariantCondition(condition);
			if (variantAttrName.size() > 0 && variantAttrValue.size() > 0) {
				// Important to check BOTH of these; top-level resources in
				// for example the image dir should have no variant identifier!
				newCondition.setCondition(variantAttrName, variantAttrValue);
			}
			scanForResources(newDirectory, resourceType, variantAttrName, variantAttrValue, loadType, newCondition, priority + 1);
		} else if (resTypeAssigned) {
			// And if it is not a directory, then we've got the actual resources!
			FileResourceDirective* directive = createFileDirective(resourceType.c_str());
			if (directive) {
				string resId = toResName(filename);
				string variant = condition.getVariantIdentifier(true);
				directive->setId(resId);
				directive->setFile(file.getAbsolutePath());
				directive->setResource(filename);
				directive->setLoadType(loadType);
				bool shouldAdd = condition.isApplicable();
				if (shouldAdd && addDirective(directive, &condition)) {
					assignPriority(directive, &condition, priority);
					if (isAmbiguous(resId)) {
						error(NULL, string("Ambiguous resource: ") + resId);
					}
				} else {
					disposeDirective(directive);
				}
			}
		}
	}
}

void VariantResourceSet::setPlatform(string platform) {
	VariantResourceSet::fPlatform = platform;
}

void VariantResourceSet::writeResources(string lstOutput) {
	int resId = 1; // First resource.

	ostringstream lstFileOutput;
	lstFileOutput << "// This file has been generated by the ResComp tool\n";

	// First, we write all the placeholders.
	vector<string> variantResIds = getAllVariantResIds();
	for (vector<string>::const_iterator variantId = variantResIds.begin(); variantId != variantResIds.end(); variantId++) {
		lstFileOutput << ".res " << (*variantId).c_str() << " // ID: " << resId << "\n.placeholder\n";
		assignVirtualIndex(*variantId, resId);
		assignResAndLoadType(resId, ResType_PlaceHolder, LoadType_Startup);
		resId++;
	}

	// Next, the non-variant resources - ie those that ONLY are defined
	// for the "empty" fallback variant.
	vector<string> nonVariantResIds = getAllNonVariantResIds();
	for (vector<string>::const_iterator nonVariantResId = nonVariantResIds.begin(); nonVariantResId != nonVariantResIds.end(); nonVariantResId++) {
		ResourceDirective* directive = getDirective(*nonVariantResId, string());
		if (directive) {
			directive->writeDirectives(lstFileOutput, false);
			assignMappedIndex(*nonVariantResId, string(), resId);
			assignResAndLoadType(resId, directive->getResourceTypeAsInt(), directive->getLoadType());
			resId++;
		}
	}

	lstFileOutput << "\n// *** End of non-variant resources\n\n";

	// Finally, we write the variant resources.
	// The map here just makes sure that we don't
	// write any file resource or string resource twice
	map<string, int> cachedResources;
	vector<string> ids = getAllVariantResIds();
	vector<string> variants = getAllVariants();
	for(vector<string>::iterator id = ids.begin(); id != ids.end(); ++id) {
		for (vector<string>::iterator variantIt = variants.begin(); variantIt != variants.end(); ++variantIt) {
			ResourceDirective* directive = getDirective(*id, *variantIt);
			if (directive) {
				int resIdToUse = resId;
				string uniqueToken = directive->getUniqueToken();
				int cachedId = cachedResources[uniqueToken];
				lstFileOutput << "// " << *id << " - " << getVariantStr(*variantIt) << '\n';
				if (uniqueToken.size() > 0 && cachedId > 0) {
					lstFileOutput << "// Using cached resource #" << cachedId << '\n';
					resIdToUse = cachedId;
				} else {
					lstFileOutput << "// Resource #" << resIdToUse << '\n';
					directive->writeDirectives(lstFileOutput, true);
					cachedResources[uniqueToken] = resId;
					resId++;
				}
				assignMappedIndex(*id, *variantIt, resIdToUse);
				assignResAndLoadType(resIdToUse, directive->getResourceTypeAsInt(), directive->getLoadType());

				// We *must* have a fallback resource!
				if (getDirective(*id, string()) == NULL) {
					error(NULL, string("No fallback resource for id ") + *id);
				}
			}
		}
	}

	lstFileOutput << createResMap();
	lstFileOutput << createResTypeList();

	ofstream lst(lstOutput.c_str(), ios::binary);
	lst << lstFileOutput.str();
	printf("Wrote resource file %s\n", lstOutput.c_str());
}

static void writeByte(char* array, int& offset, int value) {
	array[offset] = value & 0xff;
	offset +=1;
}

static void writeWord(char* array, int& offset, int value) {
	// MoSync default: little-endian
	array[offset] = value & 0xff;
	array[offset + 1] = (value >> 8) & 0xff;
	offset += 2;
}

static void writePStr(char* array, int& offset, const char* str, size_t len) {
	writeByte(array, offset, len);
	for (size_t i = 0; i < len; i++) {
		writeByte(array, offset, str[i]);
	}
}

void addLabelDirective(ostringstream& output, const char* label) {
	output << ".label \"" << label << "\"\n";
}

int VariantResourceSet::computeResAndLoadType(int resType, LoadType loadType) {
	int loadTypeAsInt = loadType == LoadType_Unloaded ? 0x40 : 0x00;
	int resAndLoadType = resType | loadTypeAsInt;
	return resAndLoadType;
}

int VariantResourceSet::computeResAndLoadType(ResourceDirective* directive) {
	int resType = directive->getResourceTypeAsInt();
	LoadType loadType = directive->getLoadType();
	return computeResAndLoadType(resType, loadType);
}

string VariantResourceSet::createResTypeList() {
	ostringstream resultStr;
	resultStr << ".res\n";
	addLabelDirective(resultStr, "res-types");
	resultStr << ".res\n";
	resultStr << ".bin\n";

	int resTypeListSize = fResTypes.size();
	char* result = (char*) malloc(resTypeListSize + 2);

	int offset = 0;

	writeWord(result, offset, resTypeListSize);

	for (int i = 1; i <= resTypeListSize; i++) {
		int resType = fResTypes[i];
		LoadType loadType = fLoadTypes[i];
		int resAndLoadType = computeResAndLoadType(resType, loadType);
		writeByte(result, offset, resAndLoadType);
	}

	ResourceDirective::writeByteDirective(resultStr, result, 0, offset);
	free(result);
	return resultStr.str();
}

string VariantResourceSet::createResMap() {
	vector<string> variants = getAllVariants();
	int numVariants = variants.size();
	int numVariantResources = getAllVariantResIds().size();

	if (numVariantResources == 0) {
		return string();
	}

	ostringstream resultStr;
	resultStr << ".res\n";
	addLabelDirective(resultStr, "variant-mapping");
	resultStr << ".res\n";
	resultStr << ".bin\n";

	// We malloc enough. We just set max lengths for all variant ids
	int resMapSize = 3 + numVariants * (2 + 256 + 3 * numVariantResources);
	char* result = (char*) malloc(resMapSize);

	int offset = 0;

	// The number of variants that has specific resource sets
	//	u1 numberOfVariants;
	writeByte(result, offset, numVariants);

	// The number of resources that may have variants. The
	// resources in the index range [1,(numberOfVariantResources + 1]
	// are variant resources, the rest are not.
	// u2 numberOfVariantResources;
	writeWord(result, offset, numVariantResources);

	// We'll write here soon.
	int lenOffset = offset;
	offset += 2 * numVariants;

	for (int i = 0; i < numVariants; i++) {
		int startOffset = offset;
		string variant = variants.at(i);
		size_t variantStrLen = variant.length();
		if (variantStrLen > 0xff) {
			error(NULL, "Too complex variant definition.");
		}

		// The length of the variant description string, used to determine
		// what variant this resource set lookup should be used for.
		// Example: “screenSize:large”
		// u1 variantDescLength;;
		// The actual string
		// char[variantDescLength] variantDesc;
		writePStr(result, offset, variant.c_str(), variantStrLen);

		// The lookup to find the ‘internal’ resource of a variant resource.
		// To perform a lookup now only requires a simple array lookup:
		// lookup[resourceHandle - 1]
		// If lookup[resourceHandle - 1] is equal to the special value 0,
		// then there is no special resource defined for this variant and
		// the next matching variant should be used
		// u2[numberOfVariantResources] lookup;
		for (int index = 1; index <= numVariantResources; index++) {
			string resId = fVirtualResourceIndices[index];
//			printf("%d: %s\n", index, resId.c_str());
			int actualIndex = fMappedResources[variant][resId];
//			printf("Variant   %s: %d -> %d\n", variant.c_str(), index, actualIndex);
			writeWord(result, offset, actualIndex);
		}

		// The priority array contains the priority of a this variant’s
		// resource -- if several variants matches the currently running
		// runtime, this number is used to find which one to use
		// u1[numberOfVariantResources] priority;
		for (int index = 1; index <= numVariantResources; index++) {
			string resId = fVirtualResourceIndices[index];
			int priority = fMappedPriorities[variant][resId];
//			printf("Priority  %s: %d -> %d\n", variant.c_str(), index, priority);
			writeByte(result, offset, priority);
		}

		// The length for each ResourceSetLookup
		// u2[numberOfVariants] resourceSetLookupLengths;
		int len = offset - startOffset;
		writeWord(result, lenOffset, len);
	}

	// Some debug info
	resultStr << "/*\n";
	resultStr << "\t" << numVariants << " variants, " << numVariantResources << " variant resources\n";
	for (vector<string>::iterator variantIt = variants.begin(); variantIt != variants.end(); ++variantIt) {
		resultStr << "\t" << getVariantStr(*variantIt) << "\n";
	}
	resultStr << "*/\n";

	ResourceDirective::writeByteDirective(resultStr, result, 0, offset);
	free(result);
	return resultStr.str();
}

VariantCondition::VariantCondition(string platform) {
	VariantCondition::fPlatform = platform;
}

void VariantCondition::initFrom(VariantCondition& prototype) {
	fConditions.insert(prototype.fConditions.begin(), prototype.fConditions.end());
}

bool VariantCondition::setCondition(string condition, string value) {
	fConditions[condition] = value;
	return false;
}

string VariantCondition::getCondition(string condition) {
	return fConditions[condition];
}

bool VariantCondition::hasCondition(string condition) {
	return fConditions.find(condition) != fConditions.end();
}

// Returns an 'unqualifed' attribute, or removes the 'platform:' prefix
// before a platform specific attribute
static string getUnqualifiedAttr(string qualifiedAttr, string qualifier) {
	string qualifierWithColon = qualifier + ":";
	if (qualifiedAttr.substr(0, qualifierWithColon.size()) == qualifierWithColon) {
		return qualifiedAttr.substr(qualifierWithColon.size());
	}
	return qualifiedAttr;
}

static string getPrefix(string qualifiedAttr) {
	size_t index = qualifiedAttr.find_first_of(':', 0);
	return index == string::npos ? string() : qualifiedAttr.substr(0, index);
}

// Escapes attribute names & values -- anything with a backslash before it will be
// treated as a literal character, so colon (:), comma (,) and backslash (\)
// itself will be escaped, since they have a special meaning.
static string escape(string attr) {
	ostringstream escaped;
	for (size_t i = 0; i < attr.size(); i++) {
		const char ch = attr.at(i);
		if (ch == '\\' || ch == ':' || ch == ',') {
			escaped << '\\';
		}
		escaped << ch;
	}
	return escaped.str();
}

bool VariantCondition::isApplicable() {
	return !hasCondition(ATTR_PLATFORM) || getCondition(ATTR_PLATFORM) == fPlatform;
}

string VariantCondition::getVariantIdentifier(bool filtered) {
	ostringstream result;
	int attrCount = 0;
	// We filter out all other platform-specific conditions; but global resources are kept!
	if (isApplicable()) {
		for(map<string, string>::const_iterator entry = fConditions.begin(); entry != fConditions.end(); entry++) {
			string variantAttr = (*entry).first;
			string attrValue = (*entry).second;
			// We always filter out the platform attr:
			if (!filtered || strcmp(variantAttr.c_str(), ATTR_PLATFORM)) {
				if (attrCount > 0) {
					result << ",";
				}
				string prefix = getPrefix(variantAttr);
				if (prefix.size() == 0 || prefix == fPlatform) {
					string attrName = filtered ? getUnqualifiedAttr(variantAttr, fPlatform) : variantAttr;
					result << escape(attrName) << ":" << escape(attrValue);
					attrCount++;
				}
			}
		}
	}
	return result.str();
}
