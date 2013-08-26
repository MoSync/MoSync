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

#ifndef COPYFILES_H
#define COPYFILES_H

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_COPY_SRC_COULD_NOT_BE_OPENED -1
#define ERR_COPY_DST_COULD_NOT_BE_OPENED -2
#define ERR_COPY_READ_FAILED -3
#define ERR_COPY_WRITE_FAILED -4

// copies a single file
// returns < 0 on error
int copySingleFile(const char *src, const char* dst);

// copies files recursively
// returns < 0 on error
int copyFilesRecursively(const char* srddir, const char* dstdir);

#ifdef __cplusplus
}
#endif

#endif	//COPYFILES_H
