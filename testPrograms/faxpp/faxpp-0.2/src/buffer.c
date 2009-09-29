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

 /*
#include <string.h>
#include <stdlib.h>
*/
#include <mastring.h>
#include <mastdlib.h>
#include <maheap.h>

#include "buffer.h"

FAXPP_Error FAXPP_init_buffer(FAXPP_Buffer *buffer, unsigned int initialSize,
                              FAXPP_BufferResizeCallback callback, void *userData)
{
  buffer->buffer = malloc(initialSize);
  if(!buffer->buffer) return OUT_OF_MEMORY;
  buffer->length = initialSize;
  buffer->cursor = buffer->buffer;
  buffer->callback = callback;
  buffer->userData = userData;
  return NO_ERROR;
}

void FAXPP_free_buffer(FAXPP_Buffer *buffer)
{
  if(buffer->buffer) free(buffer->buffer);
}

FAXPP_Error FAXPP_resize_buffer(FAXPP_Buffer *buffer, unsigned int minSize)
{
  unsigned int newLength = buffer->length << 1;
  while(newLength < minSize) {
    newLength = newLength << 1;
  }

  void* newFAXPP_Buffer;
  
  newFAXPP_Buffer = realloc(buffer->buffer, newLength);
  if(!newFAXPP_Buffer) return OUT_OF_MEMORY;

  if(newFAXPP_Buffer != buffer->buffer) {
    if(buffer->callback) buffer->callback(buffer->userData, buffer, newFAXPP_Buffer);
    buffer->cursor += newFAXPP_Buffer - buffer->buffer;
    buffer->buffer = newFAXPP_Buffer;
  }

  buffer->length = newLength;

  return NO_ERROR;
}

FAXPP_Error FAXPP_buffer_append(FAXPP_Buffer *buffer, void *ptr, unsigned int len)
{
  FAXPP_Error err;
  if(buffer->cursor + len > buffer->buffer + buffer->length) {
    err = FAXPP_resize_buffer(buffer, (buffer->cursor + len) - buffer->buffer);
    if(err != 0) return err;
  }

  memcpy(buffer->cursor, ptr, len);
  buffer->cursor += len;

  return NO_ERROR;
}

FAXPP_Error FAXPP_buffer_append_ch(FAXPP_Buffer *buffer, FAXPP_EncodeFunction encode, Char32 ch)
{
  FAXPP_Error err;
  unsigned int len;
  while((len = encode(buffer->cursor, buffer->buffer + buffer->length, ch))
        == TRANSCODE_PREMATURE_END_OF_BUFFER) {
    err = FAXPP_resize_buffer(buffer, 0);
    if(err != 0) return err;
  }

  buffer->cursor += len;

  return NO_ERROR;
}

