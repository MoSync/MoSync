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

#ifndef __FAXPP__XML_PARSER_H
#define __FAXPP__XML_PARSER_H

/*#include <stdio.h>*/

#include "xml_tokenizer.h"
#include "buffer.h"
#include <faxpp/parser.h>

/*********************
 *
 *  Parser Environment
 *
 *********************/

typedef struct FAXPP_NamespaceInfo_s {
  FAXPP_Text prefix;
  FAXPP_Text uri;

  struct FAXPP_NamespaceInfo_s *prev;
} FAXPP_NamespaceInfo;

typedef struct FAXPP_ElementInfo_s {
  FAXPP_Text prefix;
  FAXPP_Text uri;
  FAXPP_Text name;

  FAXPP_NamespaceInfo *prev_ns;
  void *prev_stack_cursor;

  struct FAXPP_ElementInfo_s *prev;
} FAXPP_ElementInfo;

typedef struct FAXPP_ParserEnv_s FAXPP_ParserEnv;

typedef FAXPP_Error (*FAXPP_NextEvent)(FAXPP_ParserEnv *env);

struct FAXPP_ParserEnv_s {
  FAXPP_NextEvent next_event;
  FAXPP_NextEvent main_next_event;

  FAXPP_ReadCallback read;
  void *read_user_data;

  void *read_buffer;
  unsigned int read_buffer_length;

  FAXPP_TokenizerEnv tenv;
  unsigned int null_terminate:1;

  unsigned int err_line;
  unsigned int err_column;

  FAXPP_Event event;

  unsigned int max_attr_count;
  FAXPP_Attribute *attrs;
  FAXPP_Attribute *current_attr;

  FAXPP_AttrValue *av_ptr;
  FAXPP_AttrValue *av_dealloc;

  FAXPP_ElementInfo *element_info_stack;
  FAXPP_NamespaceInfo *namespace_stack;

  FAXPP_ElementInfo *element_info_pool;
  FAXPP_NamespaceInfo *namespace_pool;

  FAXPP_Buffer event_buffer;
  FAXPP_Buffer stack_buffer;
};

#endif
