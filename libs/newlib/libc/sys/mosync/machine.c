#include <sys/unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/stat.h>
#include <inttypes.h>

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

int fstat(int __fd, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
int stat(const char *file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

#define FHANDLE_OFFSET 4

off_t lseek(int __fd, off_t __offset, int __whence) {
	int res;
	if(__fd <= FHANDLE_OFFSET) {
		errno = EBADF;
		return -1;
	}
	res = maFileSeek(__fd - FHANDLE_OFFSET, __offset, __whence);
	if(res < 0) {
		errno = EINVAL;
		return -1;
	}
	return res;
}
int read(int __fd, void *__buf, size_t __nbyte) {
	int res;
	if(__fd <= FHANDLE_OFFSET) {
		errno = EBADF;
		return -1;
	}
	res = maFileRead(__fd - FHANDLE_OFFSET, __buf, __nbyte);
	if(res != 0) {
		errno = EIO;
		return -1;
	}
	return __nbyte;
}
int write(int __fd, const void *__buf, size_t __nbyte) {
	if(__fd == 1 || __fd == 3) {	//stdout or stderr
		WriteConsole(__buf, __nbyte);
		return __nbyte;
	} else if(__fd > FHANDLE_OFFSET) {
		int res = maFileWrite(__fd - FHANDLE_OFFSET, __buf, __nbyte);
		if(res < 0) {
			errno = EIO;
			return -1;
		}
		return __nbyte;
	} else {
		lprintfln("write(%i, %p, %zu)", __fd, __buf, __nbyte);
		NOT;
	}
}
int close(int __fd) {
	int res;
	if(__fd <= FHANDLE_OFFSET) {
		errno = EBADF;
		return -1;
	}
	res = maFileClose(__fd - FHANDLE_OFFSET);
	if(res != 0) {
		errno = EIO;
		return -1;
	}
	return 0;
}
int open(const char * __filename, int __mode, ...) {
	int ma_mode;
	int res;
	int handle;
	int exists;
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
	return handle + FHANDLE_OFFSET;
}
int unlink(const char *name) {
	int res;
	int handle;
	handle = open(name, O_WRONLY);
	if(handle < 0)
		return handle;
	res = maFileDelete(handle - FHANDLE_OFFSET);
	if(res < 0) {
		errno = EPERM;
		return -1;
	}
	return 0;
}
int isatty(int __fd) {
	return __fd >= 1 && __fd <= 3;
}

char* getenv(const char *__string) {
	return NULL;
}

char* _getenv_r(struct _reent *p, const char *__string) {
	return NULL;
}



clock_t times (struct tms *buffer) {
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
