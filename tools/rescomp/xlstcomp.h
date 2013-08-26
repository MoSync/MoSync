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

#ifndef XLSTCOMP_H_
#define XLSTCOMP_H_

#include <string>
#include <vector>
#include <stack>
#include <map>

#include "resdirectives.h"

#define DEFAULT_VARIANT ""

using namespace std;

class VariantCondition {
private:
	map<string, string> fConditions;
	string fPlatform;
public:
	VariantCondition();
	VariantCondition(string platform);
	void initFrom(VariantCondition& prototype);
	bool setCondition(string condition, string value);
	string getCondition(string condition);
	bool hasCondition(string condition);
	bool isApplicable();
	string getVariantIdentifier(bool filtered);
	string validate();
};

class VariantResourceSet {
private:
	string fPlatform;
	map<int, string> fVirtualResourceIndices;
	map<int, int> fResTypes;
	map<int, LoadType> fLoadTypes;
	map<string, map<string, int> > fMappedResources;
	map<string, map<string, int> > fMappedPriorities;
	map<string, int> fCurrentPriority;
	map<string, map<string, ResourceDirective*> > fDirectives;
	vector<string> fVariantResIds;
	vector<string> fNonVariantResIds;
	vector<string> fVariants;
	int fCurrentId;
	void assignVirtualIndex(string resId, int virtualIndex);
	void assignMappedIndex(string resId, string variant, int mappedIndex);
	bool assignPriority(ResourceDirective* directive, VariantCondition* condition, int priority);
	bool isAmbiguous(string resId);
	void assignResAndLoadType(int resIndex, int resType, LoadType loadType);
	static int computeResAndLoadType(int resType, LoadType loadType);
	static int computeResAndLoadType(ResourceDirective* directive);
	void scanForResources(string directoryToScan,
			string resourceType,
			string variantAttrName,
			string variantAttrValue,
			LoadType loadType,
			VariantCondition currentCondition,
			int priority);
	string createResMap();
	string createResTypeList();
public:
	VariantResourceSet();
	virtual ~VariantResourceSet();
	bool addDirective(ResourceDirective* directive, VariantCondition* condition);
	vector<string> getAllVariants();
	vector<string> getAllVariantResIds();
	vector<string> getAllNonVariantResIds();
	ResourceDirective* getDirective(string resId, string variant);
	void scanForResources(string directoryToScan);
	void parseLSTX(string inputFile);
	string parseLST(string lstFile, string outputDir);
	void writeResources(string lstOutput);
	void setPlatform(string platform);
};

struct ParserState {
	// The current platform used for compiling resources,
	// eg 'Android' or 'iOS'
	string platform;
	// For convenience, nested resource tags inherit the
	// parent's id
	string currentId;
	// A stack for holding the current set of conditions
	stack<VariantCondition> conditionStack;
	// The resource set
	VariantResourceSet* resourceSet;
	// The stack of directives, with the top being the last one added
	stack<ResourceDirective*> directiveStack;
	// Do we pass through the main tag?
	bool started;
	// Line #
	int lineNo;
	// Current file
	string fileName;
};

const char* findAttr(const char* name, const char** attributes);
bool equalsIgnoreCase(const char* str1, const char* str2);
void disposeDirective(ResourceDirective* directive);

#endif /* XLSTCOMP_H_ */
