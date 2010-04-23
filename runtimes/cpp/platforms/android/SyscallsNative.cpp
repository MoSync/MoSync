#include "config_platform.h"
#include "Syscall.h"

SYSCALL(void*,  __memset(void* dst, int val, ulong size))
{
	return NULL;
}

SYSCALL(void*,  __memcpy(void* dst, const void* src, ulong size))
{
	return NULL;
}

SYSCALL(int,  __strcmp(const char* str1, const char* str2))
{
	return -1;
}

SYSCALL(char*,  __strcpy(char* dst, const char* src))
{
	return 0;
}

SYSCALL(double,  __adddf3(double a, double b))
{
	return 0;
}

SYSCALL(double,  __subdf3(double a, double b))
{
	return 0;
}

SYSCALL(double,  __muldf3(double a, double b))
{
	return 0;
}

SYSCALL(double,  __divdf3(double a, double b))
{
	return 0;
}

SYSCALL(double,  __negdf2(double a))
{
	return 0;
}

SYSCALL(int,  __fixdfsi(double a))
{
	return 0;
}

SYSCALL(double,  __floatsidf(int a))
{
	return 0;
}

SYSCALL(double,  f2d(float a))
{
	return 0;
}

SYSCALL(int,  dcmp(double a, double b))
{
	return 0;
}

SYSCALL(float,  __addsf3(float a, float b))
{
	return 0;
}

SYSCALL(float,  __subsf3(float a, float b))
{
	return 0;
}

SYSCALL(float,  __mulsf3(float a, float b))
{
	return 0;
}

SYSCALL(float,  __divsf3(float a, float b))
{
	return 0;
}

SYSCALL(float,  __negsf2(float a))
{
	return 0;
}

SYSCALL(int,  __fixsfsi(float a))
{
	return 0;
}

SYSCALL(float,  __floatsisf(int a))
{
	return 0;
}

SYSCALL(float,  d2f(double a))
{
	return 0;
}

SYSCALL(int,  fcmp(float a, float b))
{
	return 0;
}

SYSCALL(double,  sin(double x))
{
	return 0;
}

SYSCALL(double,  cos(double x))
{
	return 0;
}

SYSCALL(double,  tan(double x))
{
	return 0;
}

SYSCALL(double,  sqrt(double x))
{
	return 0;
}
