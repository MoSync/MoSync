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
	VariantCondition(string platform);
	void initFrom(VariantCondition& prototype);
	bool setCondition(string condition, string value);
	string getCondition(string condition);
	bool hasCondition(string condition);
	bool isApplicable();
	string getVariantIdentifier(bool filtered);
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
	// The last directive added
	ResourceDirective* currentDirective;
	// Do we pass through the main tag?
	bool started;
	// Line #
	int lineNo;
	// Current file
	string fileName;
};

const char* findAttr(const char* name, const char** attributes);

#endif /* XLSTCOMP_H_ */
