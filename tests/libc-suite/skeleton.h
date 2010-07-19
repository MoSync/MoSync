#include <sys/wait.h>
#include <maheap.h>
#include <math.h>
#include <regex.h>
#include <sys/stat.h>
#include <limits.h>

struct qelem
{
	struct qelem *q_forw;
	struct qelem *q_back;
	char q_data[1];
};

#define M_PERTURB	    -6

// very weird one; not defined anywhere in glibc, yet used by some test programs.
#define OBJPFX

#define WEOF ((wchar_t)-1)

// enables use of insque()
#define _KERNEL
#include <sys/queue.h>

#define TEMP_FAILURE_RETRY(a) a

#define NO_LONG_DOUBLE

#define RE_DEBUG 0

#define getline __getline

#define FNM_EXTMATCH	0x20	/* Not implemented. */

struct timespec;

typedef size_t _IO_ssize_t;

#define re_pattern_buffer regex_t
#define RE_SYNTAX_POSIX_BASIC 0
#define RE_DOT_NEWLINE 0
#define RE_HAT_LISTS_NOT_NEWLINE 0
#define RE_ICASE 0
#define RE_BACKSLASH_ESCAPE_IN_LISTS 0
#define RE_SYNTAX_POSIX_EXTENDED 0
#define RE_SYNTAX_POSIX_EGREP 0
typedef int reg_errcode_t;

typedef off_t off64_t;

#define stat64 stat
#define fstat64 fstat
#define fopen64 fopen
#define fgetc_unlocked fgetc
#define fgets_unlocked fgets

#define __GNUC_PREREQ(major, minor) 1

#define strtold strtod
#define __strtod_internal(a,b,c) strtod(a,b)

#define WORD_BIT 32
#define LONG_BIT 64

#define daylight _daylight
#define timezone _timezone

int *getdate_err(void);
struct tm *	getdate(const char *); 

const char *re_compile_pattern (const char *__pattern, size_t __length,
	struct re_pattern_buffer *__buffer);

int kill(int pid, int sig);
int nanosleep(const struct timespec *requested_time, struct timespec *remaining);
int kill(int pid, int sig);
pid_t waitpid(pid_t pid, int* status_ptr, int options);
const char* strsignal(int __signo);
void exit(int status);

char *initstate(unsigned seed, char *state, size_t size);
void * setstate (void *state);
long int random(void);
void srandom(unsigned seed);

double nexttoward(double x, double y);
float nexttowardf(float x, double y);

void re_set_syntax(int a);

char* strchrnul(const char*, int c);
char* index(const char *, int);
char* rindex(const char *, int);
void * rawmemchr (const void *block, int c);
void * memrchr (const void *block, int c, size_t size);
char* strdupa(char* str);
