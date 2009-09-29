#ifndef RUBY_MOSYNC_H
#define RUBY_MOSYNC_H

//#undef __STDC__

#define WNOHANG 01

#include <maarg.h>
#include <string.h>
#include <stdlib.h>

#ifdef __GNUC__
#define NORETURN(x) x __attribute((noreturn))
#else
#define NORETURN(x) x
#endif

#define	FD_SETSIZE	1024

/*
 * We don't want to pollute the namespace with select(2) internals.
 * Non-underscore versions are exposed later #if __BSD_VISIBLE
 */
#define	__NBBY	8				/* number of bits in a byte */
typedef int	__fd_mask;
#define __NFDBITS ((unsigned)(sizeof(__fd_mask) * __NBBY)) /* bits per mask */
#define	__howmany(x, y)	(((x) + ((y) - 1)) / (y))

typedef	struct fd_set {
	__fd_mask fds_bits[__howmany(FD_SETSIZE, __NFDBITS)];
} fd_set;

#define	FD_SET(n, p) \
	((p)->fds_bits[(n) / __NFDBITS] |= (1 << ((n) % __NFDBITS)))
#define	FD_CLR(n, p) \
	((p)->fds_bits[(n) / __NFDBITS] &= ~(1 << ((n) % __NFDBITS)))
#define	FD_ISSET(n, p) \
	((p)->fds_bits[(n) / __NFDBITS] & (1 << ((n) % __NFDBITS)))
#define	FD_COPY(f, t)	memcpy(t, f, sizeof(*(f)))
#define	FD_ZERO(p)	memset(p, 0, sizeof(*(p)))

void* alloca(size_t size);
#ifdef MAPIP
double pow(double base, double exp);
NORETURN(void abort());
NORETURN(void exit(int code));
void* realloc(void* ptr, int size);
void sleep(int duration);
unsigned long strtoul(const char* string, char** end, int base);
char* getenv(const char* name);	//returns value
int getpid();
double frexp(double num, int *exp);
void _exit(int code);
int getcwd(char* buf, int size);
#endif	//MAPIP

#ifdef _WIN32
typedef int LONG;
#include <matime.h>
#endif	//_WIN32

#define MLOG //printf//maUpdateScreen()//

#if 0
#define MOSYNC_TIME(statement) { int startTime = maGetMilliSecondCount(); (statement); int endTime = maGetMilliSecondCount();\
	printf("%s took %i ms (0x%08x to 0x%08x)\n", #statement, endTime - startTime, startTime, endTime); }
#else
#define MOSYNC_TIME(statement) statement
#endif

#endif	//RUBY_MOSYNC_H
