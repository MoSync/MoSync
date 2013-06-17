/*
 * fileset.cpp
 *
 *  Created on: 26 mar 2013
 *      Author: mattias
 */

#include <cstring>
#include <string>
#include <algorithm>

#include <filelist/filelist.h>

#include "util.h"
#include "fileset.h"

using namespace std;

string normalizeFileName(string& filename) {
	string copy(filename);
	toSlashes(copy);
	if (!copy.empty() && copy.at(copy.size() - 1) == '/') {
		copy = copy.substr(0, copy.size() - 1);
	}
	return copy;
}

void FileSetList::addFileSet(FileSet* fileset) {
	mFilesets.push_back(fileset);
}

void FileSetList::listFiles(vector<string>& files) {
	set<string> normalizedFiles;
	vector<string> unfilteredResult;
	for (size_t i = 0; i < mFilesets.size(); i++) {
		FileSet* fileset = mFilesets[i];
		if (fileset) {
			vector<string> tmp;
			bool isExcluding = fileset->isExclusionSet();
			fileset->listFiles(tmp);
			if (isExcluding) {
				for (size_t j = 0; j < tmp.size(); j++) {
					normalizedFiles.erase(normalizeFileName(tmp[j]));
				}
			} else {
				for (size_t j = 0; j < tmp.size(); j++) {
					normalizedFiles.insert(normalizeFileName(tmp[j]));
				}
				unfilteredResult.insert(unfilteredResult.end(), tmp.begin(), tmp.end());
			}
		}
	}

	vector<string> result;
	for (size_t i = 0; i < unfilteredResult.size(); i++) {
		string file = normalizeFileName(unfilteredResult[i]);
		if (normalizedFiles.count(file) > 0) {
			result.push_back(file);
		}
	}

	files.insert(files.end(), result.begin(), result.end());
}


static vector<string>* gInternalScanFileList;
static void internalListFilesCb(const char* filename) {
	if (gInternalScanFileList && strcmp(".", filename) && strcmp("..", filename)) {
		gInternalScanFileList->push_back(filename);
	}
}

void DefaultFileSet::listFiles(vector<string>& files) {
	internalListFiles(mName, isRecursive(mName), files);
}

void DefaultFileSet::internalListFiles(string& name, bool recursive, vector<string>& files) {
	string filename = getActualName(name);
	string root = mRoot;
	toDir(root);

	string fullPath = root + filename;
	file(fullPath);

	if (isDirectory(fullPath.c_str())) {
		vector<string> result;
		vector<string> subDirs;
		gInternalScanFileList = &result;

		toDir(fullPath);

		// Then add some wildcard...
		fullPath = fullPath + "*";
		// ...scan and filter...
		scanDirectory(fullPath.c_str(), internalListFilesCb);
		gInternalScanFileList = NULL;

		string dir = filename;
		toDir(filename);
		for (size_t i = 0; i < result.size(); i++) {
			string scannedFile = file(filename + result[i]);
			if (recursive) {
				if (isDirectory((root + scannedFile).c_str())) {
					internalListFiles(scannedFile, recursive, files);
				}
			}
			if (!mFileFilter || mFileFilter->accept(result[i])) {
				files.push_back(scannedFile);
			}
		}
	} else {
		files.push_back(filename);
	}
}

bool DefaultFileSet::isRecursive(string& name) {
	return name.size() >= 2 && name.substr(name.size() - 2) == "**";
}

bool DefaultFileSet::isWildcard(string& name) {
	return name.size() >= 1 && name.substr(name.size() - 1) == "*";
}

string DefaultFileSet::getActualName(string& name) {
	if (isRecursive(name)) {
		string result = name.substr(0, name.size() - 2);
		if (result.empty()) {
			return ".";
		} else {
			return result;
		}
	} else if (isWildcard(name)) {
		string result = name.substr(0, name.size() - 1);
		if (result.empty()) {
			return ".";
		} else {
			return result;
		}
	} else {
		return name;
	}
}


bool ExtensionFileFilter::accept(string& filename) {
	for (size_t i = 0; i < mExtensions.size(); i++) {
		string extension = "." + mExtensions[i];
		if (mIgnoreCase) {
			transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
		}
		if (filename.size() >= extension.size() &&
				extension == filename.substr(filename.size() - extension.size())) {
			return true;
		}
	}

	return false;
}
