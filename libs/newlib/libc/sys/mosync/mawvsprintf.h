#ifndef MAVSPRINTF_H
#define MAVSPRINTF_H

#include <stdio.h>
#include <ma.h>

#ifdef __cplusplus
extern "C" {
#endif

int wlprintfln(const wchar_t* fmt, ...);
#ifdef __cplusplus
}
#endif

#endif	//MAVSPRINTF_H
