/*
 * Copyright 2007 Doxological Ltd.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FAXPP__BUFFER_H
#define __FAXPP__BUFFER_H

#include <faxpp/error.h>
#include <faxpp/transcode.h>

typedef struct FAXPP_Buffer_s FAXPP_Buffer;
typedef void (*FAXPP_BufferResizeCallback)(void *userData, FAXPP_Buffer *buffer, void *newFAXPP_Buffer);

/// Implementation of a resizing buffer
struct FAXPP_Buffer_s {
  void *buffer;
  unsigned int length;
  void *cursor;

  FAXPP_BufferResizeCallback callback;
  void *userData;
};

FAXPP_Error FAXPP_init_buffer(FAXPP_Buffer *buffer, unsigned int initialSize,
                              FAXPP_BufferResizeCallback callback, void *userData);
void FAXPP_free_buffer(FAXPP_Buffer *buffer);

#define FAXPP_reset_buffer(buf) (buf)->cursor = (buf)->buffer

FAXPP_Error FAXPP_resize_buffer(FAXPP_Buffer *buffer, unsigned int minSize);
FAXPP_Error FAXPP_buffer_append(FAXPP_Buffer *buffer, void *ptr, unsigned int len);
FAXPP_Error FAXPP_buffer_append_ch(FAXPP_Buffer *buffer, FAXPP_EncodeFunction encode, Char32 ch);

#endif
