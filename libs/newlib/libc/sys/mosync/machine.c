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
#include "maassert.h"
#include "mavsprintf.h"


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

off_t lseek(int __fildes, off_t __offset, int __whence) {
	NOT;
}
int read(int __fd, void *__buf, size_t __nbyte) {
	NOT;
}
int write(int __fd, const void *__buf, size_t __nbyte) {
	if(__fd == 1 || __fd == 3) {	//stdout or stderr
		return maWriteLog(__buf, __nbyte);
	} else {
		lprintfln("write(%i, %p, %zu)", __fd, __buf, __nbyte);
		NOT;
	}
}
int close(int __fd) {
	NOT;
}
int open(const char * __filename, int __mode, ...) {
	NOT;
}
int unlink(const char *name) {
	NOT;
} 
int isatty(int __fd) {
	return 1;
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
