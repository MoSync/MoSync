/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
