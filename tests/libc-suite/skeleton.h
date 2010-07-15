#include <sys/wait.h>
#include <maheap.h>
#include <math.h>

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

struct timespec;

int kill(int pid, int sig);
int nanosleep(const struct timespec *requested_time, struct timespec *remaining);
int kill(int pid, int sig);
pid_t waitpid(pid_t pid, int* status_ptr, int options);
const char* strsignal(int __signo);
void exit(int status);

long int random(void);

double nexttoward(double x, double y);
float nexttowardf(float x, double y);
