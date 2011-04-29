#include <ma.h>
#include "MAHeaders.h"
#include <maassert.h>

#define MAX_PATH 256
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAT_CUSTOM(func, check) do { int _res = (func); if(check) { printf("Error %i @ %s:%i\n",\
	_res, __FILE__, __LINE__); return false; } } while(0)
#define MAT(func) MAT_CUSTOM(func, _res < 0)

static bool cleanOut(const char* dir);
static bool makeDir(char* root, int rootLen, const char* name, int bufLen);
static bool tryToMake(const char* dir);

// each record has this format: {
// byte isFile;
// char name[];	// null-terminated
// byte data[];	// terminated by end of record.
// }
void setup_filesystem() {
	// first, we must find a temporary directory which we can work out of.
	// we'll probably want to chroot() to it, too.
	char newRoot[MAX_PATH] = "";
	MAASSERT(makeDir(newRoot, 0, "mosync_tmp", sizeof(newRoot)));
	MAASSERT(chdir(buf) == 0);
	MAASSERT(chroot(".") == 0);
	
	for(MAHandle i=RES_START; i<RES_END; i++) {
		char buf[MAX_PATH];
		int size = maGetDataSize(i);
		int pathlen = MIN(MAX_PATH, size);
		maReadData(i, buf, 0, pathlen);
		bool isFile = !!buf[0];
		const char* name = buf+1;
		int namelen = strlen(name);
		MAASSERT(namelen < MAX_PATH);
		int dataOffset = namelen+2;
		
		// if(!isFile), then it's a directory that must be created.
	}
}

// uses root buffer as scratch space.
// on success, writes full path of created directory to root.
// \param root buffer that, on entry, contains directory path where searching should start.
// \param rootLen length of string in root, excluding the terminating zero.
static bool makeDir(char* root, int rootLen, const char* name, int bufLen) {
	// find a root path
	printf("makeDir(%s)\n", root);
	MAHandle list = maFileListStart(root, "*")
	MAT(list);
	int freeBufSpace = bufLen - rootLen;
	while(1) {
		int res;
		res = maFileListNext(list, root + rootLen, freeBufSpace);
		MAT(res, (_res < 0 || _res >= freeBufSpace));
		if(res == 0)
			return false;
		int resLen = rootLen + res;
		if(file[resLen-1] == '/') {
			rootLen = resLen;
			MAASSERT(rootLen + strlen(name) < bufLen);
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
	MAHandle file = maFileOpen(filename.c_str(), MA_ACCESS_READ_WRITE);
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
	printf("Done.\n");
	return true;
}

static bool cleanOut(const char* dir) {
	printf("cleanOut(%s)\n", dir);
	MAHandle list = maFileListStart(dir, "*")
	MAT(list);
	char buf[2048];
	int dirLen = strlen(dir);
	strcpy(buf, dir);
	int freeBufSpace = bufLen - dirLen;
	while(1) {
		int res;
		MAHandle fh;
		char* fileName = buf + dirLen;
		res = maFileListNext(list, fileName, freeBufSpace);
		MAT(res, (_res < 0 || _res >= freeBufSpace));
		if(res == 0)
			return true;
		if(fileName[res-1] == '/') {
			if(!cleanOut(buf))
				return false;
		}
		MAT(file = maFileOpen(buf));
		res = maFileDelete(file);
		MAASSERT(maFileClose(file) == 0);
		MAT(res);
	}
	MAASSERT(maFileListClose(list) == 0);
	return true;
}
