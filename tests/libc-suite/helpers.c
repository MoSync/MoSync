#include <ma.h>
#include "error.h"
#include "mavsprintf.h"
#include "maassert.h"
#include <errno.h>

int main();

int MAMain() {
	return main();
}

void error(int __status, int __errnum, __const char* __format, ...)
{
	va_list args;
	va_start(args, __format);
	char buffer[2048];
	vsprintf(buffer, __format, args);
	va_end(args);
	lprintfln("error(%i, %i, %s)\n", buffer);
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

int mcheck (void (*__abortfunc) (enum mcheck_status)) {
	return 0;
}
int mcheck_pedantic (void (*__abortfunc) (enum mcheck_status)) {
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

double nexttoward(double x, double y) {
	return nextafter(x, y);
}

float nexttowardf(float x, double y) {
	return nextafterf(x, y);
}
