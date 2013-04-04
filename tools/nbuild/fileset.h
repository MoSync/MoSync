/*
 * fileset.h
 *
 *  Created on: 26 mar 2013
 *      Author: mattias
 */

#ifndef FILESET_H_
#define FILESET_H_

#include <string>
#include <vector>

using namespace std;

class FileFilter {
public:
	virtual ~FileFilter() { };
	virtual bool accept(string& filename) = 0;
};

class ExtensionFileFilter : public FileFilter {
private:
	vector<string> mExtensions;
	bool mIgnoreCase;
public:
	virtual ~ExtensionFileFilter() { };
	ExtensionFileFilter(vector<string>& extensions, bool ignoreCase = false) :
		mExtensions(extensions), mIgnoreCase(ignoreCase) { };
	virtual bool accept(string& filename);
};

class FileSet {
protected:
	FileFilter* mFileFilter;
	bool mIsExclusion;
public:
	FileSet(FileFilter* filter, bool isExclusion) : mFileFilter(filter), mIsExclusion(isExclusion) { };
	virtual ~FileSet() { };
	virtual void listFiles(vector<string>& files) = 0;
	virtual bool isExclusionSet() { return mIsExclusion; }
};

class FileSetList : public FileSet {
private:
	vector<FileSet*> mFilesets;
public:
	FileSetList() : FileSet(NULL, false) { };
	void addFileSet(FileSet* fileset);
	virtual void listFiles(vector<string>& files);
};

class DefaultFileSet : public FileSet {
	string mRoot;
	string mName;
private:
	void internalListFiles(string& name, bool recursive, vector<string>& files);
	string getActualName(string& name);
	bool isRecursive(string& name);
	bool isWildcard(string& name);
public:
	DefaultFileSet(string root, string name, bool isExclusion, FileFilter* filter) :
		FileSet(filter, isExclusion), mRoot(root), mName(name) { };
	virtual void listFiles(vector<string>& files);
};

string normalizeFileName(string& filename);


#endif /* FILESET_H_ */
