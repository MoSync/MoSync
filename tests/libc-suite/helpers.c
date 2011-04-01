#include <ma.h>
#include "error.h"
#include "mavsprintf.h"
#include "maassert.h"
#include <mastdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

static void install_stdmalloc_hooks();
int main(int argc, const char** argv);
extern const char* gArgv[];
extern const int gArgc;

int MAMain() {
	// switch stdout from console to maWriteLog.
	int wlfd = open_maWriteLog();
	dup2(wlfd, 1);
	dup2(wlfd, 2);
	close(wlfd);

	printf("MAMain()\n");
	install_stdmalloc_hooks();
	return main(gArgc, gArgv);
}

static void std_malloc_handler(int size) {
	errno = ENOMEM;
	lprintfln("malloc failed: %i", size);
};

static void install_stdmalloc_hooks() {
	set_malloc_handler(std_malloc_handler);
}

void error(int __status, int __errnum, __const char* __format, ...)
{
	va_list args;
	va_start(args, __format);
	char buffer[2048];
	vsprintf(buffer, __format, args);
	va_end(args);
	lprintfln("error(%i, %i, %s)\n", __status, __errnum, buffer);
	maExit(1);
}

int nanosleep(const struct timespec *requested_time, struct timespec *remaining) {
	BIG_PHAT_ERROR;
}

int kill(int pid, int sig) {
	BIG_PHAT_ERROR;
	errno = ENOSYS;	// not implemented
	return -1;
}

pid_t waitpid(pid_t pid, int* status_ptr, int options) {
	BIG_PHAT_ERROR;
	errno = ENOSYS;
	return -1;
}

const char* strsignal(int __signo) {
	BIG_PHAT_ERROR;
	errno = ENOSYS;
	return NULL;
}

unsigned sleep(unsigned s) {
	lprintfln("sleep(%i)", s);
	const int start = maGetMilliSecondCount();
	const int end = start + s * 1000;
	do {
		int left = end - maGetMilliSecondCount();
		int res;
		MAEvent e;
		if(left <= 0)
			break;
		while(maGetEvent(&e)) {
			if(e.type = EVENT_TYPE_CLOSE) {
				lprintfln("EVENT_TYPE_CLOSE");
				exit(42);
			}
		}
		maWait(left);
	} while(1);
	return 0;
}

FILE *popen(const char *s, const char * mode) {
	errno = ENOSYS;
	return NULL;
}

int pclose(FILE* file) {
	errno = ENOSYS;
	return -1;
}

int mcheck (void (*__abortfunc) (int)) {
	return 0;
}
int mcheck_pedantic (void (*__abortfunc) (int)) {
	return 0;
}
void mcheck_check_all (void) {
}
void mtrace (void) {
}
void muntrace (void) {
}

long int random (void) {
	return rand();
}
char *initstate(unsigned seed, char *state, size_t size) {
	return NULL;
}
void srandom(unsigned seed) {
}
void * setstate (void *state) {
	return NULL;
}

double nexttoward(double x, double y) {
	return nextafter(x, y);
}

float nexttowardf(float x, double y) {
	return nextafterf(x, y);
}

char* strdupa(char* str) {
	// not a valid implementation, but it'll work for these small test programs.
	return strdup(str);
}

void re_set_syntax(int a) {
}

const char *re_compile_pattern (const char *__pattern, size_t __length,
	struct re_pattern_buffer *__buffer)
{
	int res = regcomp(__buffer, __pattern, __length);
	if(res == 0)
		return NULL;
	else
		return "regcomp error";
}


#define MAXPATHLEN 1024
char *
basename(const char *path)
{
	static char bname[MAXPATHLEN];
	size_t len;
	const char *endp, *startp;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		bname[0] = '.';
		bname[1] = '\0';
		return (bname);
	}

	/* Strip any trailing slashes */
	endp = path + strlen(path) - 1;
	while (endp > path && *endp == '/')
		endp--;

	/* All slashes becomes "/" */
	if (endp == path && *endp == '/') {
		bname[0] = '/';
		bname[1] = '\0';
		return (bname);
	}

	/* Find the start of the base */
	startp = endp;
	while (startp > path && *(startp - 1) != '/')
		startp--;

	len = endp - startp + 1;
	if (len >= sizeof(bname)) {
		errno = ENAMETOOLONG;
		return (NULL);
	}
	memcpy(bname, startp, len);
	bname[len] = '\0';
	return (bname);
}
char *
dirname(const char *path)
{
	static char dname[MAXPATHLEN];
	size_t len;
	const char *endp;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		dname[0] = '.';
		dname[1] = '\0';
		return (dname);
	}

	/* Strip any trailing slashes */
	endp = path + strlen(path) - 1;
	while (endp > path && *endp == '/')
		endp--;

	/* Find the start of the dir */
	while (endp > path && *endp != '/')
		endp--;

	/* Either the dir is "/" or there are no slashes */
	if (endp == path) {
		dname[0] = *endp == '/' ? '/' : '.';
		dname[1] = '\0';
		return (dname);
	} else {
		/* Move forward past the separating slashes */
		do {
			endp--;
		} while (endp > path && *endp == '/');
	}

	len = endp - path + 1;
	if (len >= sizeof(dname)) {
		errno = ENAMETOOLONG;
		return (NULL);
	}
	memcpy(dname, path, len);
	dname[len] = '\0';
	return (dname);
}

char* strchrnul(const char* str, int c) {
	char* res = strchr(str, c);
	if(res == NULL)
		return (char*)str + strlen(str);
	else
		return res;
}

char* index(const char* str, int c) {
	return strchr(str, c);
}

char* rindex(const char* str, int c) {
	return strrchr(str, c);
}

void * memrchr (const void *block, int c, size_t size) {
	char* p = (char*)block + size;
	while(p > (char*)block) {
		p--;
		if(*p == c)
			return p;
	}
	return NULL;
}

void* rawmemchr(const void *block, int c) {
	char* p = (char*)block;
	while(*p != c) {
		p++;
	}
	return p;
}

void* mmap(void* address, size_t length, int protect, int flags, int filedes, off_t offset) {
	if((flags & MAP_FIXED) != 0 || (flags & MAP_ANON) == 0) {
		errno = ENOSYS;
		return NULL;
	}
	return malloc(length);
}

int mprotect(void* a, size_t b, int c) {
	return 0;
}

int munmap(void* address, size_t size) {
	free(address);
	return 0;
}


#define MAX_IMPLS 64
typedef struct
{
  const char *name;
  void (*fn) (void);
  long test;
} impl_t;
impl_t __start_impls[MAX_IMPLS];
impl_t* __stop_impls = __start_impls;

int ffsl(long int i) {
	return ffs(i);
}

int ffsll(long long int i) {
	unsigned long long int x = i & -i;

	if (x <= 0xffffffff)
		return ffs (i);
	else
		return 32 + ffs (i >> 32);
}

long int sysconf (int parameter) {
	errno = ENOSYS;
	return -1;
}

wchar_t* wmempcpy (wchar_t* wto, const wchar_t* wfrom, size_t size) {
	return (wchar_t *)mempcpy(wto, wfrom, size * sizeof(wchar_t));
}
