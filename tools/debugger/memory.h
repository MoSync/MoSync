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

#ifndef _MEMORY_H_
#define _MEMORY_H_

extern int gMemSize;
extern char* gMemBuf;

/**
 * Sets the size of the memory being cached and initializes the memory module.
 *
 * @param size Size of the memory.
 */
void setMemSize(int size);

/**
 * Clears the all cached memory locations.
 */
void clearMemoryCacheBits();

/**
 * Checks if the given memory locations are cached. After this call the 
 * specified memory locations will be marked as cached, i.e. assuming a call
 * to chache them will be made after this call.
 *
 * @param src Address of the beginning of the cached locations.
 * @param len The range of the bytes being cached.
 * @return True if the specified memory locations was cached, false otherwise.
 */
bool isMemoryCached(int src, int len);

#endif /* _MEMORY_H_ */
