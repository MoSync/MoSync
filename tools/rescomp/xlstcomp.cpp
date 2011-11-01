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
#include "xlstcomp.h"
#include "resdirectives.h"

using namespace std;

#define XML_BUFFER_SIZE 4096
#define MAIN_TAG "resources"
#define TAG_CONDITION "condition"
#define ATTR_PLATFORM "platform"

#define RES_IMAGE "image"
#define RES_AUDIO "audio"
#define RES_MEDIA "media"
#define RES_STRING "string"
#define RES_BINARY "binary"
#define RES_PLACEHOLDER "placeholder"

static void error(ParserState* state, const char* msg) {
	ostringstream errMsg;
	if (state) {
		errMsg << state->fileName << ":" << state->lineNo << ": ";
	}
	errMsg << msg << '\n';
	printf(errMsg.str().c_str(), "");
	exit(1);
}

static void error(ParserState* state, string msg) {
	error(state, msg.c_str());
}

VariantResourceSet::VariantResourceSet() {
}

VariantResourceSet::~VariantResourceSet() {

}

void VariantResourceSet::assignVirtualIndex(string resId, int virtualIndex) {
	fResourceIndices[virtualIndex] = resId;
}

void VariantResourceSet::assignMappedIndex(string resId, string variant, int mappedIndex) {
	fMappedResources[variant][resId] = mappedIndex;
}

void VariantResourceSet::addDirective(ResourceDirective* directive, string variant) {
	if (directive) {
		string resId = directive->getId();
		if (variant.size() > 0) { // Is it a variant resource?
			fVariantResIds.push_back(resId);
		} else {
			fNonVariantResIds.push_back(resId);
		}
		fVariants.push_back(variant);
		if (getDirective(resId, variant) && variant.size() > 0) {
			error(NULL, (string("Duplicate definition of resource ") + resId + " for variant" + variant).c_str());
		}
		fDirectives[variant][resId] = directive;
		fMappedPriorities[variant][resId] = fCurrentPriority[resId];
		fCurrentPriority[resId] = fCurrentPriority[resId] + 1;
	}
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
	return result;
}

static ResourceDirective* createDirective(const char* tagName) {
	if (!strcmp(RES_BINARY, tagName)) {
		return new BinaryResourceDirective();
	} else if (!strcmp(RES_IMAGE, tagName)) {
		return new ImageResourceDirective();
	} else if (!strcmp(RES_MEDIA, tagName)) {
		return new MediaResourceDirective();
	} else if (!strcmp(RES_AUDIO, tagName)) {
		return new AudioResourceDirective();
	} else if (!strcmp(RES_STRING, tagName)) {
		return new StringResourceDirective();
	} else if (!strcmp(RES_PLACEHOLDER, tagName)) {
		return new PlaceholderDirective();
	}
	return NULL;
}

static void disposeDirective(ResourceDirective* directive) {
	delete directive;
}

static bool isResource(const char* tagName) {
	// Lazy me :)
	ResourceDirective* directive = createDirective(tagName);
	bool result = directive != NULL;
	disposeDirective(directive);
	return result;
}


bool shouldAddDirective(VariantCondition* condition) {
	return !condition || condition->getVariantIdentifier(false).size() > 0;
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
			string id = directive->getId();
			state->currentId = id;
			if (state->currentId.size() == 0) {
				error(state, "No id attribute in resource tag " + string(tagName) + " (or a parent resource tag)");
			}
			VariantCondition* cond = state->conditionStack.empty() ? NULL : &(state->conditionStack.top());
			if (shouldAddDirective(cond)) {
				string variant = cond == NULL ? string() : cond->getVariantIdentifier(true);
//				printf("%s : %s", id.c_str(), variant.c_str());
				state->resourceSet->addDirective(directive, variant);
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
		directive->initDirectiveFromCData(content, length);
	}
}

string VariantResourceSet::parseLSTX(string inputFile, string outputDir) {
	char buf[XML_BUFFER_SIZE];

	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, xlstStart, xlstEnd);
	XML_SetCharacterDataHandler(parser, xlstCDATA);
	ParserState* state = new ParserState();
	state->platform = fPlatform;
	state->started = false;
	state->resourceSet = this;
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

	string lstFile = outputDir + "/tmpres.lst";
	writeResources(lstFile);

	delete state;
	return lstFile;
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
	if (!res) {
		error(NULL, "Resource compilation failed");
	}
	return output;
}

string VariantResourceSet::scanForResources(string resourceDir, string outputDir) {
return string();
}
void VariantResourceSet::setPlatform(string platform) {
	VariantResourceSet::fPlatform = platform;
}

string getVariantStr(string variant) {
	if (variant.size() == 0) {
		return string("(fallback variant)");
	} else {
		return variant;
	}
}

void VariantResourceSet::writeResources(string lstFile) {
	int resId = 1; // First resource.

	ostringstream lstFileOutput;

	// First, we write all the placeholders.
	vector<string> variantResIds = getAllVariantResIds();
	for (vector<string>::const_iterator variantId = variantResIds.begin(); variantId != variantResIds.end(); variantId++) {
		lstFileOutput << ".res " << (*variantId).c_str() << " // ID: " << resId << "\n.placeholder\n";
		assignVirtualIndex(*variantId, resId);
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
			resId++;
		}
	}

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
			}
		}
	}

	lstFileOutput << createResMap();

	// Some debug info
	lstFileOutput << "\n/*\n";
	lstFileOutput << "\t" << getAllVariants().size() << " variants:\n";
	for (vector<string>::iterator variantIt = variants.begin(); variantIt != variants.end(); ++variantIt) {
		lstFileOutput << "\t" << getVariantStr(*variantIt) << "\n";
	}
	lstFileOutput << "*/\n";

	ofstream lst(lstFile.c_str(), ios::binary);
	lst << lstFileOutput.str();
	printf("Wrote resource file %s\n", lstFile.c_str());
}

static void writeByte(char* array, int& offset, int value) {
	array[offset] = value & 0xff;
	offset +=1;
}

static void writeWord(char* array, int& offset, int value) {
	array[offset] = (value >> 8) & 0xff;
	array[offset + 1] = value & 0xff;
	offset += 2;
}

static void writePStr(char* array, int& offset, const char* str, size_t len) {
	writeByte(array, offset, len);
	for (size_t i = 0; i < len; i++) {
		writeByte(array, offset, str[i]);
	}
}

string VariantResourceSet::createResMap() {
	vector<string> variants = getAllVariants();
	int numVariants = variants.size();
	int numVariantResources = getAllVariantResIds().size();

	if (numVariantResources == 0) {
		return string();
	}

	ostringstream resultStr;
	resultStr << ".label \"variant-mapping\"\n.res\n";

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
			string resId = fResourceIndices[index];
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
			string resId = fResourceIndices[index];
			int priority = fMappedPriorities[variant][resId];
//			printf("Priority  %s: %d -> %d\n", variant.c_str(), index, priority);
			writeByte(result, offset, priority);
		}

		// The length for each ResourceSetLookup
		// u2[numberOfVariants] resourceSetLookupLengths;
		int len = offset - startOffset;
		writeWord(result, lenOffset, len);
	}
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

string VariantCondition::getVariantIdentifier(bool filtered) {
	ostringstream result;
	int attrCount = 0;
	// We filter out all other platform-specific conditions; but global resources are kept!
	if (!hasCondition(ATTR_PLATFORM) || getCondition(ATTR_PLATFORM) == fPlatform) {
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
