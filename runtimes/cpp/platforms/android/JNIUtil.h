/*
 * mautf.h
 *
 *  Created on: 16 apr 2013
 *      Author: mattias
 */

#ifndef JNIUTIL_H_
#define JNIUTIL_H_

#include <stdlib.h>
#include <wchar.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MOSYNC_NATIVE
#include <jni.h>
// First we'll define an internal function, next we'll let the macro call that.
// Native wchar on android is 4 bytes.
jstring mawstojs(JNIEnv* env, const wchar_t* src);
#define WCHAR_TO_JCHAR(env, str) mawstojs(env, str)
#else
// We kind of assume UTF16, since that's what previous versions
// have been using. There is, however, a need for better support
// of 1) UTF-8 and 2) other locales (iconv comes to mind)
#include <helpers/cpp_defs.h>
int wcslen_m(const wchar* maStr);
#define WCHAR_TO_JCHAR(env, str) env->NewString((jchar*) str, wcslen_m(str))
#endif

#ifdef __cplusplus
}
#endif

#endif /* JNIUTIL_H_ */
