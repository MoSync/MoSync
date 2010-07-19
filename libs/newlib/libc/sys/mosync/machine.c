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
#include <IX_FILE.h>
#include "maassert.h"
#include "mavsprintf.h"
#include "conprint.h"


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
	maExit(status);
}

int raise(int code) {
	//exit? panic?
	//maPanic(code, "raise()");
	return -1;
}

#define NOT PANIC_MESSAGE("not supported")

int getpid(void) {
  return 1;
}

uid_t getuid(void) {
	return 0;
}

struct passwd* getpwuid(uid_t uid) {
	return NULL;
}

struct group* getgrgid(gid_t gid) {
	return NULL;
}

int fstat(int __fd, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int stat(const char *file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
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
};

#define NFD 32
static struct LOW_FD* sFda[NFD];
static struct LOW_FD sLfda[NFD];

static struct LOW_FD sLfConsole = { LOWFD_CONSOLE, 1 };
//static struct LOW_FD sLfWriteLog = { LOWFD_WRITELOG, 1 };

static int closeLfd(struct LOW_FD* plfd);

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
}

static struct LOW_FD* getLowFd(int __fd) {
	initFda();
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
#define LOWFD int lfd; struct LOW_FD* plfd = getLowFd(__fd); if(plfd == NULL) return -1; lfd = plfd->lowFd;

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
	return -1;
}


int dup(int __fd) {
	int newFd;
	LOWFD;
	newFd = findFreeFd();
	if(newFd < 0) {
		errno = EMFILE;
		return -1;
	}
	sFda[newFd] = plfd;
	plfd->refCount++;
	return newFd;
}

int dup2(int __fd, int newFd) {
	struct LOW_FD* newLfd;
	LOWFD;
	if(__fd == newFd)
		return newFd;
	if(newFd < 0 || newFd >= NFD) {
		errno = EBADF;
		return -1;
	}
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


off_t lseek(int __fd, off_t __offset, int __whence) {
	int res;
	LOWFD;
	res = maFileSeek(lfd - LOWFD_OFFSET, __offset, __whence);
	if(res < 0) {
		errno = EINVAL;
		return -1;
	}
	return res;
}

int ftruncate(int __fd, off_t __length) {
	int res;
	LOWFD;
	res = maFileTruncate(lfd - LOWFD_OFFSET, __length);
	if(res < 0) {
		errno = EINVAL;
		return -1;
	}
	return res;
}

int read(int __fd, void *__buf, size_t __nbyte) {
	int res;
	LOWFD;
	res = maFileRead(lfd - LOWFD_OFFSET, __buf, __nbyte);
	if(res != 0) {
		errno = EIO;
		return -1;
	}
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
		res = maFileWrite(lfd - LOWFD_OFFSET, __buf, __nbyte);
	}
	if(res < 0) {
		errno = EIO;
		return -1;
	}
	return __nbyte;
}

static int closeLfd(struct LOW_FD* plfd) {
	int res;
	plfd->refCount--;
	if(plfd->refCount == 0) {
		res = maFileClose(plfd->lowFd - LOWFD_OFFSET);
		if(res != 0) {
			errno = EIO;
			return -1;
		}
	}
	return 0;
}

int close(int __fd) {
	int res;
	LOWFD;
	res = closeLfd(plfd);
	sFda[__fd] = NULL;
	return res;
}

int isatty(int __fd) {
	LOWFD;
	return lfd == LOWFD_CONSOLE;
}

int open(const char * __filename, int __mode, ...) {
	int ma_mode;
	int res;
	int handle;
	int exists;
	int newFd;
	struct LOW_FD* newLfd;
	if(__mode & O_WRONLY) {
		ma_mode = MA_ACCESS_READ_WRITE;
	} else if(__mode & O_RDONLY) {
		ma_mode = MA_ACCESS_READ;
	} else {
		PANIC_MESSAGE("bad mode");
	}
	if(__mode & O_NONBLOCK) {
		PANIC_MESSAGE("unsupported mode: O_NONBLOCK");
	}
	if(__mode & O_APPEND) {
		PANIC_MESSAGE("unsupported mode: O_APPEND");
	}
	// O_SHLOCK, O_EXLOCK and O_NOATIME are unsupported. we drop them silently.

	// Find a spot in the descriptor array.
	newFd = findFreeFd();
	if(newFd < 0) {
		errno = EMFILE;
		return -1;
	}
	newLfd = findFreeLfd();

	res = maFileOpen(__filename, ma_mode);
	if(res < 0) {
		switch(res) {
		case MA_FERR_FORBIDDEN: errno = EACCES; break;
		case MA_FERR_NOTFOUND: errno = ENOENT; break;
		default: errno = EIO;
		}
		return -1;
	}
	handle = res;
	exists = maFileExists(handle);
	if(exists < 0) {
		errno = ENOTRECOVERABLE;
		return -1;
	}
	if(__mode & O_CREAT) {
		if((__mode & O_EXCL) && exists) {
			errno = EEXIST;
			return -1;
		}
		if(!exists) {
			res = maFileCreate(handle);
			if(res < 0) {
				errno = EACCES;
				return -1;
			}
		}
	} else if(!exists) {
		errno = ENOENT;
		return -1;
	}
	if(__mode & O_TRUNC) {
		res = maFileTruncate(handle, 0);
		if(res < 0) {
			errno = EIO;
			return -1;
		}
	}
	sFda[newFd]->lowFd = handle + LOWFD_OFFSET;
	sFda[newFd]->refCount = 1;
	return newFd;
}

int unlink(const char *name) {
	int dres, cres;
	int __fd;
	__fd = open(name, O_WRONLY);
	if(__fd < 0)
		return __fd;
	dres = maFileDelete(sFda[__fd]->lowFd - LOWFD_OFFSET);
	cres = close(__fd);
	if(dres < 0) {
		errno = EPERM;
		return -1;
	}
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
	errno = ENOSYS;
	return -1;
}

int setpgid(pid_t pid, pid_t pgid) {
	errno = ENOSYS;
	return -1;
}

int fork(void) {
	errno = ENOSYS;
	return -1;
}

_sig_func_ptr signal(int sig, _sig_func_ptr f) {
	errno = ENOSYS;
	return SIG_ERR;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset) {
	errno = ENOSYS;
	return -1;
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
	return -1;
}

int gettimeofday (struct timeval *tp, void *tzp) {
	tp->tv_sec = maLocalTime();
	tp->tv_usec = 0;
	if(tzp != NULL) {
		errno = ENOSYS;
		return -1;
	}
	return 0;
}

int nice(int incr) {
	errno = ENOSYS;
	return -1;
}
