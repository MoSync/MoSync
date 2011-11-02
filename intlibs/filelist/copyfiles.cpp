#include <stack>
#include <string>
#include "filelist.h"
#include "../helpers/mkdir.h"
#include "copyfiles.h"

using namespace std;

static stack<string> sDestinationDirectory;
static stack<string> sSourceDirectory;

void copyFilesRecurse();

// returns total amount of bytes written.
// on failure, returns an error code, and errno will be set.
extern "C" int copyFile(const char *src, const char* dst) {
	char buffer[1024];
	size_t bytesRead, bytesWritten;
	size_t bufferIndex = 0;
	int res = 0;
	FILE *fd_src, *fd_dst;

	fd_src = fopen(src, "rb");
	if(fd_src==NULL) return ERR_COPY_SRC_COULD_NOT_BE_OPENED;
	fd_dst = fopen(dst,"wb");
	if(fd_dst==NULL) {
		fclose(fd_src);
		return ERR_COPY_DST_COULD_NOT_BE_OPENED;
	}

	int eof = 0;
	while(!eof) {
		if((bytesRead = fread(buffer, 1, 1024, fd_src)) != 1024) {
			if(!feof(fd_src)) {
				res = ERR_COPY_READ_FAILED;
				break;
			} else {
				eof = 1;
			}
		}
		if((bytesWritten=fwrite(&buffer[bufferIndex], 1, bytesRead, fd_dst)) != bytesRead) {
			res = ERR_COPY_WRITE_FAILED;
			break;
		}
		res += (int)bytesWritten;
	}

	fclose(fd_src);
	fclose(fd_dst);
	return res;
}

void copyFilesCallback(const char *path) {
	if(path[0] == '.') return;
	const char* slash = strrchr(path, '/');
	const char *fn;
	if(slash)
		fn = slash + 1;
	else fn = path;

	string p = sSourceDirectory.top() + string("/") + fn;
	path = p.c_str();

	int isDir = isDirectory(path);
	if(isDir < 0) {
		//error("Could not scan source file/dir %s\n", path);
		throw ERR_COPY_SRC_COULD_NOT_BE_OPENED;
	} else if(isDir == 1) {
		sDestinationDirectory.push(sDestinationDirectory.top() + string("/") + fn);
#ifdef WIN32
		sSourceDirectory.push(sSourceDirectory.top() + string("/") + fn);
#else
		sSourceDirectory.push(path);
#endif

		copyFilesRecurse();
	} else {
		string srcFile = path;
		string dstFile = sDestinationDirectory.top() + string("/") + fn;
		int errCode;
		if((errCode = copyFile(srcFile.c_str(), dstFile.c_str()))<0) {
			throw errCode;
			//error("Could not copy file %s. Error code: %d, errno: %i (%s)\n", path, errCode, errno, strerror(errno));
		}
	}
}

void copyFilesRecurse() {
	_mkdir(sDestinationDirectory.top().c_str());
	printf("src: %s\n", sSourceDirectory.top().c_str());
	printf("dst: %s\n", sDestinationDirectory.top().c_str());

	if(scanDirectory((sSourceDirectory.top()+"/*").c_str(), copyFilesCallback) != 0)
		//error("Could not scan source directory.\n");
		throw ERR_COPY_SRC_COULD_NOT_BE_OPENED;
	sSourceDirectory.pop();
	sDestinationDirectory.pop();
}


extern "C" int copyFilesRecursively(const char *src_dir, const char *dst_dir) {
	sSourceDirectory.push(src_dir);
	sDestinationDirectory.push(dst_dir);

	try
	{
		copyFilesRecurse();
	}
	catch(int err)
	{
		return err;
	}

	return 0;
}
