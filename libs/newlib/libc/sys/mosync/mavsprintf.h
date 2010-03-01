#ifndef MAVSPRINTF_H
#define MAVSPRINTF_H

#include <stdio.h>
#include <ma.h>

#ifdef __cplusplus
extern "C" {
#endif

int lprintfln(const char* fmt, ...) GCCATTRIB(format(printf, 1, 2));
#ifdef __cplusplus
}
#endif

#endif	//MAVSPRINTF_H
