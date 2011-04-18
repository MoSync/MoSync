#include <sys/unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <pwd.h>
#include <grp.h>
#include <utime.h>
#include <sys/signal.h>
#include <string.h>
#include <getopt.h>

#include <maapi.h>
#include "maassert.h"
#include "mastdlib.h"
#include "mavsprintf.h"
#include "conprint.h"
#include "realpath.h"
#include "dirent.h"


#ifdef MOSYNCDEBUG
#define LOGD(...) lprintfln(__VA_ARGS__)
#else
#define LOGD(...)
#endif

#define LOGFAIL LOGD("Failure @ %s:%i", __FILE__, __LINE__)
#define STDFAIL { LOGFAIL; return -1; }
#define ERRNOFAIL(_errno) { LOGFAIL; errno = _errno; return -1; }

#define CHECK(val, _errno) FAILIF((val) < 0, _errno)
#define FAILIF(bool, _errno) if((bool)) { errno = (_errno); STDFAIL; }
#define TEST(val) { int _temp = (val); if(_temp < 0) { LOGFAIL; return _temp; } }


typedef void (*voidfunc)(void);
void crt_tor_chain(int* ptr);
void crt_tor_chain(int* ptr) {
	if(ptr == NULL)
		return;
	while(*ptr) {
		voidfunc fp = (voidfunc)*ptr;
		fp();
		ptr++;
	}
}


void _exit(int status) {
	LOGD("exit(%i)\n", status);
	fcloseall();
	maExit(status);
}

int raise(int code) {
	//exit? panic?
	//maPanic(code, "raise()");
	STDFAIL;
}

#define NOT PANIC_MESSAGE("not supported")

int getpid(void) {
  return 1;
}

uid_t getuid(void) {
	return 0;
}

struct passwd* getpwuid(uid_t uid) {
	//quick hack
	static struct passwd p = { (char*)"mosync", (char*)"", 0, 0, (char*)"", NULL, NULL, NULL };
	if(uid == 0)
		return &p;
	else
		return NULL;
}

struct group* getgrgid(gid_t gid) {
	//quick hack
	static char* users[] = { (char*)"mosync", NULL };
	static struct group g = { (char*)"mosync", (char*)"", 0, users };
	if(gid == 0)
		return &g;
	else
		return NULL;
}

// Define file descriptors.
// In this scheme, a file descriptor is an index into this array.
// Each value in this array is a pointer to a struct, which is stored in yet another array.
// This struct has a low-level descriptor, which means this:
// 0 is invalid. 1 is the console. 2 is maWriteLog().
// 3 and above are file handles, as returned by maFileOpen(), plus LOWFD_OFFSET.
// With this scheme, dup() and dup2() should be possible to implement.
// The struct also has a reference counter.

#define LOWFD_CONSOLE 0
#define LOWFD_WRITELOG 1
#define LOWFD_OFFSET 2

// check include/sys/_default_fcntl.h to make sure this flag doesn't clash.
#define O_DIRECTORY 0x1000000

struct LOW_FD {
	int lowFd;
	int refCount;
	int flags;
	
	// valid only if flags & O_DIRECTORY.
	char* name;
	int listHandle;
};

#define NFD 32
static struct LOW_FD* sFda[NFD];
static struct LOW_FD sLfda[NFD];

static struct LOW_FD sLfConsole = { LOWFD_CONSOLE, 1, O_APPEND, NULL, 0 };
static struct LOW_FD sLfWriteLog = { LOWFD_WRITELOG, 1, O_APPEND, NULL, 0 };

static int closeLfd(struct LOW_FD* plfd);
static void lowRewindDir(struct LOW_FD* plfd);

static char sCwdBuf[2048] = "/";
static char* sCwd = sCwdBuf;

static void initFda(void) {
	static int initialized = 0;
	if(initialized)
		return;
	initialized = 1;

	memset(sFda, 0, sizeof(sFda));
	memset(sLfda, 0, sizeof(sLfda));
	sFda[0] = &sLfConsole;	// stdin
	sFda[1] = &sLfConsole;	// stdout
	sFda[2] = &sLfConsole;	// stderr
	sLfConsole.refCount = 3;
}

static struct LOW_FD* getLowFd(int __fd) {
	initFda();
	//LOGD("getLowFd(%i)", __fd);
	if(__fd <= 0 || __fd >= NFD) {
		errno = EBADF;
		return NULL;
	}
	if(sFda[__fd] == NULL) {
		errno = EBADF;
		return NULL;
	}
	MAASSERT(sFda[__fd]->refCount > 0);
	return sFda[__fd];
}
#define LOWFD int lfd; struct LOW_FD* plfd; \
	/*LOGD("LOWFD(%i) in %s", __fd, __FUNCTION__);*/ \
	plfd = getLowFd(__fd); if(plfd == NULL) { LOGD("Bad fd: %i\n", __fd); STDFAIL; } lfd = plfd->lowFd;

static struct LOW_FD* findFreeLfd(void) {
	initFda();
	for(int i=0; i<NFD; i++) {
		if(sLfda[i].refCount == 0)
			return sLfda + i;
	}
	BIG_PHAT_ERROR;
}

static int findFreeFd(void) {
	initFda();
	for(int i=4; i<NFD; i++) {
		if(sFda[i] == NULL)
			return i;
	}
	STDFAIL;
}

// returns -1 and sets errno on failure.
static int getRealPath(int __fd, char *buf, const char* path, int size) {
	if(__fd != AT_FDCWD) {
		// temporarily change cwd.
		LOWFD;
		FAILIF(!(plfd->flags & O_DIRECTORY), ENOTDIR);
		sCwd = plfd->name;
	}
	if(realpath(path, buf) == NULL) {
		STDFAIL;
	}
	if(__fd != AT_FDCWD) {
		// restore original cwd.
		sCwd = sCwdBuf;
	}
	FAILIF(buf[0] == 0, ENOENT);
	return 0;
}

// converts maFile error codes to errno.
// returns -1 on error.
static int checkMAResult(int res) {
	if(res < 0) {
		switch(res) {
		case MA_FERR_FORBIDDEN: errno = EACCES; break;
		case MA_FERR_NOTFOUND: errno = ENOENT; break;
		default: errno = EIO;
		}
		STDFAIL;
	}
	return res;
}

static MAHandle errnoFileOpen(const char* path, int ma_mode) {
	MAHandle h = maFileOpen(path, ma_mode);
	return checkMAResult(h);
}

int dup(int __fd) {
	int newFd;
	LOWFD;
	newFd = findFreeFd();
	CHECK(newFd, EMFILE);
	sFda[newFd] = plfd;
	plfd->refCount++;
	return newFd;
}

int dup2(int __fd, int newFd) {
	struct LOW_FD* newLfd;
	LOWFD;
	LOGD("dup2(%i, %i)", __fd, newFd);
	if(__fd == newFd)
		return newFd;
	FAILIF(newFd < 0 || newFd >= NFD, EBADF);
	newLfd = getLowFd(newFd);
	if(newLfd != NULL) {
		int res = closeLfd(newLfd);
		if(res < 0)
			return res;
	}
	sFda[newFd] = plfd;
	plfd->refCount++;
	return newFd;
}

#ifdef __LARGE64_FILES
#error gha
#endif

static int baseStat(MAHandle h, struct stat* st) {
	if(st->st_mode == S_IFREG) {
		CHECK(st->st_size = maFileSize(h), EIO);
	} else {
		st->st_size = 4*1024;	// default directory size.
	}
	CHECK(st->st_mtime = maFileDate(h), EIO);
	
	// we don't support the rest of the fields, but let's give them dummy values anyway.
	st->st_ino = 0;
	st->st_dev = 0;
	st->st_nlink = 1;
	st->st_uid = 0;
	st->st_gid = 0;
	st->st_atime = st->st_mtime;
	st->st_ctime = st->st_mtime;
	st->st_blocks = (st->st_size / 512) + 1;
	st->st_blksize = 1024 * 4;	// arbitrary
	
	return 0;
}

int fstat(int __fd, struct stat *st) {
	LOWFD;
	LOGD("fstat(%i)\n", __fd);
	if(lfd == LOWFD_CONSOLE || lfd == LOWFD_WRITELOG) {
		st->st_mode = S_IFCHR;
		return 0;
	} else {
		MAHandle h = lfd - LOWFD_OFFSET;
		st->st_mode = (plfd->flags & O_DIRECTORY) ? S_IFDIR : S_IFREG;
		return baseStat(h, st);
	}
}

// returns the value from maFileExist(), or <0 in case of another error.
static int postStat(MAHandle h, struct stat *st, int mode) {
	int exists;
	CHECK(exists = maFileExists(h), ENOTRECOVERABLE);
	if(!exists) {
		return 0;
	}
	st->st_mode = mode;
	TEST(baseStat(h, st));
	return 1;
}

int stat(const char *file, struct stat *st) {
	return fstatat(AT_FDCWD, file, st, 0);
}

int fstatat(int __fd, const char* path, struct stat* st, int flag) {
	MAHandle h;
	int res;
	char temp[2048];
	int length;
	LOWFD;

	LOGD("fstatat(%i, %s)", __fd, path);
	TEST(getRealPath(__fd, temp, path, 2046));
	
	// check if it's a directory.
	length = strlen(temp);
	if(temp[length-1] != '/') {
		temp[length] = '/';
		length++;
		temp[length] = 0;
	}
	h = errnoFileOpen(temp, MA_ACCESS_READ);
	if(h > 0) {
		res = postStat(h, st, S_IFDIR);
		CHECK(maFileClose(h), EIO);
		TEST(res);
		if(res > 0)	// it was an existing directory.
			return 0;
	} else {
		if(errno != ENOENT)
			STDFAIL;
	}
	
	// see if we can find a regular file.
	temp[length-1] = 0;	// remove the ending slash
	TEST(h = errnoFileOpen(temp, MA_ACCESS_READ));
	res = postStat(h, st, S_IFREG);
	CHECK(maFileClose(h), EIO);
	TEST(res);
	if(!res) {
		ERRNOFAIL(ENOENT);
	}
	return 0;
}

off_t lseek(int __fd, off_t __offset, int __whence) {
	MAHandle file;
	int res;
	LOWFD;
	LOGD("lseek(%i, %li, %i)\n", __fd, __offset, __whence);
	if(plfd->flags & O_DIRECTORY) {
		// seeking in a directory is dangerous.
		// for now, we only support this most simple case.
		if(__offset == 0 && __whence == SEEK_SET) {
			lowRewindDir(plfd);
			return 0;
		} else {
			NOT;
		}
	}
	if(lfd < LOWFD_OFFSET) {
		// seeking on LOWFD_CONSOLE and LOWFD_WRITELOG is permitted only in a few special cases.
		// also, we don't keep track on how much has been written to them.
		if((__whence == SEEK_CUR || __whence == SEEK_END) && __offset == 0)
			return 0;
		else
			ERRNOFAIL(EINVAL);
	}
	file = lfd - LOWFD_OFFSET;
	switch(__whence) {
		case SEEK_SET: __whence = MA_SEEK_SET; break;
		case SEEK_CUR: __whence = MA_SEEK_CUR; break;
		case SEEK_END: __whence = MA_SEEK_END; break;
		default: ERRNOFAIL(EINVAL);
	}
	CHECK(res = maFileSeek(file, __offset, __whence), EINVAL);
	return res;
}

int ftruncate(int __fd, off_t __length) {
	LOWFD;
	CHECK(maFileTruncate(lfd - LOWFD_OFFSET, __length), EINVAL);
	return 0;
}

int read(int __fd, void *__buf, size_t __nbyte) {
	int remaining, fileSize, fileTell;
	MAHandle file;
	LOWFD;
	file = lfd - LOWFD_OFFSET;
	LOGD("read(%i, %zu)\n", __fd, __nbyte);

	CHECK(fileSize = maFileSize(file), EIO);
	CHECK(fileTell = maFileTell(file), EIO);
 
	remaining = fileSize - fileTell;
	// seeking past the end of the file is allowed. it's treated like ordinary EOF.
	if(remaining <= 0) return 0;
	//FAILIF(remaining < 0, EIO);
	if(remaining < __nbyte)
		__nbyte = remaining;
	
	CHECK(maFileRead(file, __buf, __nbyte), EIO);
	return __nbyte;
}

int write(int __fd, const void *__buf, size_t __nbyte) {
	int res;
	LOWFD;
	if(lfd == LOWFD_CONSOLE) {
		WriteConsole(__buf, __nbyte);
		return __nbyte;
	} else if(lfd == LOWFD_WRITELOG) {
		res = maWriteLog(__buf, __nbyte);
	} else {
		MAHandle h = lfd - LOWFD_OFFSET;
		LOGD("write(%i, %zu)\n", __fd, __nbyte);
		if(plfd->flags & O_APPEND) {
			int oldPos = maFileTell(h);
			int fileSize = maFileSize(h);
			FAILIF(oldPos < 0 || fileSize < 0, EIO);
			CHECK(maFileSeek(h, 0, MA_SEEK_END), EIO);
			CHECK(maFileWrite(h, __buf, __nbyte), EIO);
			if(oldPos != fileSize) {
				res = maFileSeek(h, oldPos, MA_SEEK_SET);
			}
		} else {
			res = maFileWrite(h, __buf, __nbyte);
		}
	}
	CHECK(res, EIO);
	return __nbyte;
}

static int closeLfd(struct LOW_FD* plfd) {
	LOGD("closeLfd(%i, %i)", plfd->lowFd, plfd->refCount);
	plfd->refCount--;
	if(plfd->refCount == 0 && plfd->lowFd > LOWFD_OFFSET) {
		CHECK(maFileClose(plfd->lowFd - LOWFD_OFFSET), EIO);
	}
	return 0;
}

int close(int __fd) {
	int res;
	LOWFD;
	LOGD("close(%i)", __fd);
	res = closeLfd(plfd);
	sFda[__fd] = NULL;
	return res;
}

int isatty(int __fd) {
	LOWFD;
	return lfd == LOWFD_CONSOLE || lfd == LOWFD_WRITELOG;
}

static int postOpen(MAHandle handle, int __mode) {
	int exists;
	CHECK(exists = maFileExists(handle), ENOTRECOVERABLE);
	LOGD("exists: %i", exists);
	if(__mode & O_CREAT) {
		FAILIF((__mode & O_EXCL) && exists, EEXIST);
		if(!exists) {
			CHECK(maFileCreate(handle), EACCES);
		}
	} else {
		FAILIF(!exists, ENOENT);
	}
	if(__mode & O_TRUNC) {
		CHECK(maFileTruncate(handle, 0), EIO);
	}
	return 0;
}

int open(const char * __filename, int __mode, ...) {
	return openat(AT_FDCWD, __filename, __mode);
}

int openat(int __fd, const char * __filename, int __mode, ...) {
	int ma_mode;
	int handle;
	int newFd;
	struct LOW_FD* newLfd;
	int length;

	char temp[2048];
	TEST(getRealPath(__fd, temp, __filename, 2046));
	LOGD("openat(%i, %s, 0x%x)", __fd, temp, __mode);
	
	if((__mode & 3) == O_RDWR || (__mode & 3) == O_WRONLY) {
		ma_mode = MA_ACCESS_READ_WRITE;
	} else if((__mode & 3) == O_RDONLY) {
		ma_mode = MA_ACCESS_READ;
	} else {
		PANIC_MESSAGE("bad mode");
	}
	if(__mode & O_NONBLOCK) {
		PANIC_MESSAGE("unsupported mode: O_NONBLOCK");
	}
	// O_SHLOCK, O_EXLOCK and O_NOATIME are unsupported. we drop them silently.
	
	// Check to see if we're opening a directory.
	__mode &= ~O_DIRECTORY;
	length = strlen(temp);
	if(temp[length-1] == '/') {
		__mode |= O_DIRECTORY;
	}
	
	// Find a spot in the descriptor array.
	CHECK(newFd = findFreeFd(), EMFILE);
	newLfd = findFreeLfd();
	
	if(!(__mode & O_DIRECTORY)) {
		handle = errnoFileOpen(temp, ma_mode);
		if(handle < 0) {
			// Try opening the requested file as a directory.
			temp[length] = '/';
			length++;
			temp[length] = 0;
			__mode |= O_DIRECTORY;
		}
	}
	if(__mode & O_DIRECTORY) {
		handle = errnoFileOpen(temp, ma_mode);
	}
	TEST(handle);
	if(postOpen(handle, __mode) < 0) {
		CHECK(maFileClose(handle), EIO);
		STDFAIL;
	}
	
	newLfd->lowFd = handle + LOWFD_OFFSET;
	newLfd->refCount = 1;
	newLfd->flags = __mode;
	if(__mode & O_DIRECTORY) {
		newLfd->name = malloc(length+1);
		FAILIF(newLfd->name == NULL, ENOMEM);
		memcpy(newLfd->name, temp, length+1);
		newLfd->listHandle = 0;
	}
	sFda[newFd] = newLfd;
	return newFd;
}

int mkdir(const char* path, mode_t mode) {
	MAHandle handle;
	char temp[2048];
	int length;
	
	TEST(getRealPath(AT_FDCWD, temp, path, 2046));
	length = strlen(temp);
	if(temp[length-1]!='/') {
		temp[length] = '/';
		length++;
		temp[length] = 0;
	}
	TEST(handle = errnoFileOpen(temp, MA_ACCESS_READ_WRITE));
	if(postOpen(handle, O_CREAT | O_EXCL) < 0) {
		int exists;
		int orig_err = errno;
		CHECK(maFileClose(handle), EIO);
		// check if a file with the same name exists.
		// if so, fail with EEXIST.
		length--;
		temp[length] = 0;
		TEST(handle = errnoFileOpen(temp, MA_ACCESS_READ_WRITE));
		exists = maFileExists(handle);
		CHECK(maFileClose(handle), EIO);
		CHECK(exists, ENOTRECOVERABLE);
		FAILIF(exists, EEXIST);
		ERRNOFAIL(orig_err);
	}
	return 0;
}

int rmdir(const char* name) {
	return unlink(name);
}

int open_maWriteLog(void) {
	int newFd;
	CHECK(newFd = findFreeFd(), EMFILE);
	sFda[newFd] = &sLfWriteLog;
	sLfWriteLog.refCount++;
	return newFd;
}

int unlink(const char *name) {
	return unlinkat(AT_FDCWD, name, 0);
}

int unlinkat(int fd, const char *name, int flag) {
	int dres, cres;
	int __fd;

	__fd = openat(fd, name, O_RDWR);
	if(__fd < 0)
		return __fd;
	dres = maFileDelete(sFda[__fd]->lowFd - LOWFD_OFFSET);
	cres = close(__fd);
	CHECK(dres, EPERM);
	return cres;
}

int truncate(const char *path, off_t length) {
	int tres, cres;
	int __fd;

	__fd = open(path, O_WRONLY);
	if(__fd < 0)
		return __fd;
	tres = ftruncate(__fd, length);
	cres = close(__fd);
	if(tres < 0)
		return tres;
	return cres;
}

int chdir(const char *filename) {
	MAHandle file;
	int length;
	int ret;
	char temp[2048];
	TEST(getRealPath(AT_FDCWD, temp, filename, 2046));
	LOGD("chdir(%s)", temp);
	
	length = strlen(temp);
	if(temp[length-1]!='/') {
		// Check if it's a regular file. If so, ENOTDIR.
		file = maFileOpen(temp, MA_ACCESS_READ);
		if(file > 0) {
			ret = maFileExists(file);
			CHECK(maFileClose(file), EIO);
			CHECK(ret, EIO);
			FAILIF(ret > 0, ENOTDIR);
		}
		
		temp[length] = '/';
		length++;
		temp[length] = 0;
	}
	
	CHECK(file = maFileOpen(temp, MA_ACCESS_READ), EIO);
	ret = maFileExists(file);
	CHECK(maFileClose(file), EIO);
	CHECK(ret, EIO);
	FAILIF(ret == 0, ENOENT);

	strcpy(sCwd, temp);
	return 0;
}

char* getcwd(char* __buf, size_t __size) {
	// GNU functionality.
	size_t len = strlen(sCwd);
	if(__buf == NULL) {
		if(__size == 0) {
			__size = len + 1;
		}
	}
	if(__size <= len) {
		errno = ERANGE;
		return NULL;
	}
	if(__buf == NULL) {
		__buf = malloc(__size);
		if(__buf == NULL) {
			errno = ENOMEM;
			return NULL;
		}
	}
	strncpy(__buf, sCwd, __size);
	return __buf;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset) {
	errno = ENOSYS;
	STDFAIL;
}

int getpagesize(void) {
	return 1024;	//arbitrary
}


clock_t times (struct tms *buffer) {
	errno = ENOSYS;
	STDFAIL;
}

int gettimeofday (struct timeval *tp, void *tzp) {
	tp->tv_sec = maLocalTime();
	tp->tv_usec = 0;
	FAILIF(tzp != NULL, ENOSYS);
	return 0;
}

int nice(int incr) {
	errno = ENOSYS;
	STDFAIL;
}

int getdents(int __fd, dirent* dp, int count) {
	//int res;
	LOWFD;
	MAASSERT(count >= sizeof(dirent));
	LOGD("getdents(%i)\n", __fd);
	
	// if we don't have an open list, open a list.
	if(plfd->listHandle <= 0) {
		MAASSERT(plfd->name != NULL);
		plfd->listHandle = maFileListStart(plfd->name, "*");
		TEST(checkMAResult(plfd->listHandle));
	}
	CHECK(dp->d_namlen = maFileListNext(plfd->listHandle, dp->d_name, sizeof(dp->d_name)), EIO);
	LOGD("namlen: %i\n", dp->d_namlen);
	FAILIF(dp->d_namlen >= sizeof(dp->d_name), EINVAL);
	if(dp->d_name[dp->d_namlen-1] == '/') {
		LOGD("getdents: directory.\n");
		dp->d_type = DT_DIR;
		dp->d_name[dp->d_namlen-1] = 0;
		dp->d_namlen--;
	} else {
		dp->d_type = DT_REG;
	}
	return dp->d_namlen;
}

void rewinddir(DIR* dir) {
	struct LOW_FD* plfd = getLowFd(dir->dd_fd);
	MAASSERT(plfd);
	lowRewindDir(plfd);
}

static void lowRewindDir(struct LOW_FD* plfd) {
	if(plfd->listHandle > 0) {
		int res = maFileListClose(plfd->listHandle);
		MAASSERT(res == 0);
		plfd->listHandle = 0;
	}
}

int dirfd(DIR* dir) {
	return dir->dd_fd;
}

DIR* opendir(const char *name) {
	DIR *dirp;
	int fd;

	if ((fd = open(name, 0)) == -1)
		return NULL;
	dirp = fdopendir(fd);
	if(dirp == NULL) {
		close(fd);
	}
	return dirp;
}

DIR* fdopendir(int __fd) {
	DIR *dirp;
	int rc = 0;
	
	struct LOW_FD* plfd = getLowFd(__fd);
	if(!plfd) {
		LOGFAIL;
		return NULL;
	}
	if(!(plfd->flags & O_DIRECTORY)) {
		LOGFAIL;
		errno = ENOTDIR;
		return NULL;
	}
	
	if (rc == -1 ||
	    (dirp = (DIR *)malloc(sizeof(DIR))) == NULL) {
		return NULL;
	}
	/*
	 * If CLSIZE is an exact multiple of DIRBLKSIZ, use a CLSIZE
	 * buffer that it cluster boundary aligned.
	 * Hopefully this can be a big win someday by allowing page trades
	 * to user space to be done by getdirentries()
	 */
	dirp->dd_buf = malloc (512);
	dirp->dd_len = 512;

	if (dirp->dd_buf == NULL) {
		free (dirp);
		return NULL;
	}
	dirp->dd_fd = __fd;

	return dirp;
}

int fchdir(int __fd) {
	LOWFD;
	FAILIF(!(plfd->flags & O_DIRECTORY), ENOTDIR);
	return chdir(plfd->name);
}
