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
#include "mavsprintf.h"
#include "conprint.h"
#include "realpath.h"


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

struct LOW_FD {
	int lowFd;
	int refCount;
	int flags;
};

#define NFD 32
static struct LOW_FD* sFda[NFD];
static struct LOW_FD sLfda[NFD];

static struct LOW_FD sLfConsole = { LOWFD_CONSOLE, 1, O_APPEND };
//static struct LOW_FD sLfWriteLog = { LOWFD_WRITELOG, 1, O_APPEND };

static int closeLfd(struct LOW_FD* plfd);

static char sCwd[2048] = "/";

// returns 1 if it's a directory, 0 if it's not.
static int getRealPath(char *buf, const char* path, int size) {
#if 0
	if(path[0]!='/') {
		strncpy(buf, sCwd, size);
		strncat(buf, path, size);
		path = buf;
	} else {
		strncpy(buf, path, size);
	}
#else
	if(realpath(path, buf) == NULL) {
		PANIC_MESSAGE("realpath");
	}
#endif
	size=strlen(buf);
	if(!size || buf[size-1]=='/') return 1;
	else return 0;
}

static void initFda(void) {
	static int initialized = 0;
	if(initialized)
		return;
	initialized = 1;

	memset(sFda, 0, sizeof(sFda));
	memset(sLfda, 0, sizeof(sLfda));
	sFda[1] = &sLfConsole;
	sFda[2] = &sLfConsole;
	sFda[3] = &sLfConsole;
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

static MAHandle errnoFileOpen(const char* path, int ma_mode) {
	MAHandle h = maFileOpen(path, ma_mode);
	if(h < 0) {
		switch(h) {
		case MA_FERR_FORBIDDEN: errno = EACCES; break;
		case MA_FERR_NOTFOUND: errno = ENOENT; break;
		default: errno = EIO;
		}
		STDFAIL;
	}
	return h;
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

static int baseStat(MAHandle h, struct stat* st, int checkSize) {
	if(checkSize) {
		CHECK(st->st_size = maFileSize(h), EIO);
	} else {
		st->st_size = 0;
	}
	CHECK(st->st_mtime = maFileDate(h), EIO);
	
	// we don't support the rest of the fields, but let's give them dummy values anyway.
	st->st_ino = 0;
	st->st_dev = 0;
	st->st_nlink = 1;
	st->st_uid = 0;
	st->st_gid = 0;
	st->st_atime = 0;
	st->st_ctime = 0;
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
		st->st_mode = S_IFREG;
		return baseStat(h, st, TRUE);
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
	TEST(baseStat(h, st, mode != S_IFDIR));
	return 1;
}

int stat(const char *file, struct stat *st) {
	MAHandle h;
	int res;
	char temp[2048];
	int length;
	getRealPath(temp, file, 2046);
	LOGD("stat(%s)", temp);
	
	// check if it's a directory.
	length = strlen(temp);
	if(temp[length-1] != '/') {
		strncat(temp, "/", sizeof(temp));
		length++;
	}
	h = errnoFileOpen(temp, MA_ACCESS_READ);
	if(h > 0) {
		res = postStat(h, st, S_IFDIR);
		if(res < 0) {
			CHECK(maFileClose(h), EIO);
			STDFAIL;
		}
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
	if(res < 0) {
		CHECK(maFileClose(h), EIO);
		STDFAIL;
	}
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
	int ma_mode;
	int handle;
	int newFd;
	struct LOW_FD* newLfd;

	char temp[2048];
	if(getRealPath(temp, __filename, 2048) == 1) {
		errno = ENOENT;
		STDFAIL;
	}
	LOGD("open(%s, 0x%x)", temp, __mode);
	
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

	// Find a spot in the descriptor array.
	CHECK(newFd = findFreeFd(), EMFILE);
	newLfd = findFreeLfd();

	TEST(handle = errnoFileOpen(temp, ma_mode));
	if(postOpen(handle, __mode) < 0) {
		CHECK(maFileClose(handle), EIO);
		STDFAIL;
	}
	
	newLfd->lowFd = handle + LOWFD_OFFSET;
	newLfd->refCount = 1;
	newLfd->flags = __mode;
	sFda[newFd] = newLfd;
	return newFd;
}

int unlink(const char *name) {
	int dres, cres;
	int __fd;
	char temp[2048];

	getRealPath(temp, name, 2048);
	LOGD("unlink(%s)", temp);

	__fd = open(temp, O_WRONLY);
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
	int ret = 1;
	char temp[2048];
	getRealPath(temp, filename, 2048);
	LOGD("chdir(%s)", temp);
	
	length = strlen(temp);
	if(temp[length-1]!='/')
		strncat(temp, "/", sizeof(sCwd));
	
	CHECK(file = maFileOpen(sCwd, MA_ACCESS_READ), EIO);
	ret = maFileExists(file);
	if(ret > 0) {
		strcpy(sCwd, temp);
		ret = 0;
	}
	CHECK(maFileClose(file), EIO);
	CHECK(ret, EIO);
	return ret;
}

char* getcwd(char *__buf, size_t __size ) {
	strncpy(__buf, sCwd, __size);
	return __buf;
}

int setpgid(pid_t pid, pid_t pgid) {
	errno = ENOSYS;
	STDFAIL;
}

int fork(void) {
	errno = ENOSYS;
	STDFAIL;
}

_sig_func_ptr signal(int sig, _sig_func_ptr f) {
	errno = ENOSYS;
	return SIG_ERR;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset) {
	errno = ENOSYS;
	STDFAIL;
}

unsigned int alarm(unsigned int seconds) {
	BIG_PHAT_ERROR;
}


int mallopt(int parameter, int value) {
	errno = ENOSYS;
	return 0;
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
