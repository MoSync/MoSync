#include <ma.h>
#include <maassert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_PATH 256
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAT_CUSTOM(func, check) do { int _res = (func); if(check) { printf("Error %i @ %s:%i\n",\
	_res, __FILE__, __LINE__); return false; } } while(0)
#define MAT(func) MAT_CUSTOM(func, _res < 0)

// delete all files and subdirectories in a directory.
// the root directory must exist.
static bool cleanOut(const char* dir);

static bool makeDir(char* root, int rootLen, const char* name, int bufLen);
static bool tryToMake(const char* dir);
static bool writeFile(MAHandle fh, MAHandle data, int dataOffset, int dataLen);

// each record has this format: {
// char name[];	// null-terminated
// byte data[];	// terminated by end of record.
// }
void setup_filesystem() {
	printf("setup_filesystem\n");
	// first, we must find a temporary directory which we can work out of.
	// we'll probably want to chroot() to it, too.
	char newRoot[MAX_PATH] = "";
	MAASSERT(makeDir(newRoot, 0, "mosync_root/", sizeof(newRoot)));
	int newRootLen = strlen(newRoot);
	MAASSERT(chdir(newRoot) == 0);
	MAASSERT(chroot(".") == 0);
	// now we have the root of a unix file-system.
	
	const MAHandle start = maFindLabel("start");
	const MAHandle end = maFindLabel("end");
	MAASSERT(start > 0 && end > 0);
	printf("%i files:\n", end - (start+1));
	
	for(MAHandle i=start+1; i<end; i++) {
		char buf[MAX_PATH];
		int size = maGetDataSize(i);
		int pathlen = MIN(MAX_PATH, size);
		maReadData(i, buf, 0, pathlen);
		const char* name = buf;
		int namelen = strlen(name);
		MAASSERT(namelen < MAX_PATH);
		int dataOffset = namelen+1;
		int dataLen = size - dataOffset;
		printf("%i: %s (%i bytes)\n", i, name, dataLen);
		
		bool isDirectory = name[namelen-1] == '/';
		if(isDirectory) {
			// there can be no data in a directory.
			MAASSERT(dataLen == 0);
			MAASSERT(!mkdir(name, 0755));
		} else {
			// Because we want to use maFileWriteFromData(), we can't use open() here.
			char realName[MAX_PATH];
			MAHandle fh;
			bool res;
			
			memcpy(realName, newRoot, newRootLen);
			// overwrite the slash in newRoot, so we don't get double slashes.
			memcpy(realName + newRootLen - 1, name, namelen + 1);
			
			MAASSERT((fh = maFileOpen(realName, MA_ACCESS_READ_WRITE)) > 0);
			res = writeFile(fh, i, dataOffset, dataLen);
			MAASSERT(maFileClose(fh) >= 0);
			MAASSERT(res);
		}
	}
}

static bool writeFile(MAHandle fh, MAHandle data, int dataOffset, int dataLen) {
	int exists;

	MAT(exists = maFileExists(fh));
	if(exists) {
		MAT(maFileTruncate(fh, 0));
	} else {
		MAT(maFileCreate(fh));
	}
	MAT(maFileWriteFromData(fh, data, dataOffset, dataLen));
	return true;
}

// Uses root buffer as scratch space.
// On success, writes full path of created directory to root.
// \param root Buffer that, on entry, contains directory path where searching should start.
// \param rootLen Length of string in root, excluding the terminating zero.
// \param name Name of the directory to create. Must end with '/'.
// \param bufLen Length of the buffer pointed to by \a root, in bytes.
static bool makeDir(char* root, int rootLen, const char* name, int bufLen) {
	// find a root path
	printf("makeDir(%s)\n", root);
	MAHandle list = maFileListStart(root, "*");
	MAT(list);
	int freeBufSpace = bufLen - rootLen;
	while(1) {
		int res;
		res = maFileListNext(list, root + rootLen, freeBufSpace);
		MAT_CUSTOM(res, _res < 0 || _res >= freeBufSpace);
		if(res == 0)
			return false;
		int resLen = rootLen + res;
		if(root[resLen-1] == '/') {
			rootLen = resLen;
			MAASSERT(rootLen + (int)strlen(name) < bufLen);
			//printf("Dir: '%s'\n", file.c_str());
			strcpy(root + rootLen, name);
			if(tryToMake(root))
				return true;
			root[rootLen] = 0;
			if(makeDir(root, rootLen, name, bufLen))
				return true;
		}
	}
}

static bool tryToMake(const char* dir) {
	MAHandle file = maFileOpen(dir, MA_ACCESS_READ_WRITE);
	MAT(file);
	int res = maFileExists(file);
	MAASSERT(res >= 0);
	if(res) {
		printf("Dir exists.\n");
		// delete everything inside.
		return cleanOut(dir);
	} else {
		printf("Creating dir...\n");
		MAT(maFileCreate(file));
	}
	printf("Closing...\n");
	res = maFileClose(file);
	MAASSERT(res == 0);
	printf("Done: %s\n", dir);
	return true;
}

static bool cleanOut(const char* dir) {
	printf("cleanOut(%s)\n", dir);
	MAHandle list = maFileListStart(dir, "*");
	MAT(list);
	char buf[2048];
	int dirLen = strlen(dir);
	strcpy(buf, dir);
	int freeBufSpace = sizeof(buf) - dirLen;
	while(1) {
		int res;
		MAHandle fh;
		char* fileName = buf + dirLen;
		res = maFileListNext(list, fileName, freeBufSpace);
		MAT_CUSTOM(res, (_res < 0 || _res >= freeBufSpace));
		if(res == 0)
			return true;
		if(fileName[res-1] == '/') {
			if(!cleanOut(buf))
				return false;
		}
		MAT(fh = maFileOpen(buf, MA_ACCESS_READ_WRITE));
		res = maFileDelete(fh);
		MAASSERT(maFileClose(fh) == 0);
		MAT(res);
	}
	MAASSERT(maFileListClose(list) == 0);
	return true;
}
