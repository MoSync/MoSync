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

#include "xml_parser.h"
#include "char_classes.h"
#include "tokenizer_states.h"
#include "config.h"

#define INITIAL_ATTRS_SIZE 4
#define INITIAL_NS_SIZE 6

/// Must be at least 4 for encoding sniffing
/// Must be a multiple of 4
#define READ_BUFFER_SIZE 16 * 1024

#define INITIAL_EVENT_BUFFER_SIZE 256
#define INITIAL_STACK_BUFFER_SIZE 1024

FAXPP_Error init_tokenizer_internal(FAXPP_TokenizerEnv *env, FAXPP_EncodeFunction encode);
void free_tokenizer_internal(FAXPP_TokenizerEnv *env);

static FAXPP_Error nc_next_event(FAXPP_ParserEnv *env);
static FAXPP_Error nc_unsupported_encoding_next_event(FAXPP_ParserEnv *env);

static FAXPP_Error wf_next_event(FAXPP_ParserEnv *env);

static void p_change_event_buffer(void *userData, FAXPP_Buffer *buffer, void *newBuffer);
static void p_change_stack_buffer(void *userData, FAXPP_Buffer *buffer, void *newBuffer);

FAXPP_Parser *FAXPP_create_parser(FAXPP_ParseMode mode, FAXPP_EncodeFunction encode)
{
  FAXPP_ParserEnv *env = malloc(sizeof(FAXPP_ParserEnv));
  memset(env, 0, sizeof(FAXPP_ParserEnv));

  env->max_attr_count = INITIAL_ATTRS_SIZE;
  env->attrs = (FAXPP_Attribute*)malloc(sizeof(FAXPP_Attribute) * INITIAL_ATTRS_SIZE);
  if(!env->attrs) {
    FAXPP_free_parser(env);
    return 0;
  }

  if(FAXPP_init_buffer(&env->event_buffer, INITIAL_EVENT_BUFFER_SIZE, p_change_event_buffer, env) == OUT_OF_MEMORY) {
    FAXPP_free_parser(env);
    return 0;
  }

  if(FAXPP_init_buffer(&env->stack_buffer, INITIAL_STACK_BUFFER_SIZE, p_change_stack_buffer, env) == OUT_OF_MEMORY) {
    FAXPP_free_parser(env);
    return 0;
  }

  if(init_tokenizer_internal(&env->tenv, encode) == OUT_OF_MEMORY) {
    FAXPP_free_parser(env);
    return 0;
  }

  switch(mode) {
  case NO_CHECKS_PARSE_MODE:
    env->main_next_event = nc_next_event;
    FAXPP_set_normalize_attrs(env, 0);
    break;
  case WELL_FORMED_PARSE_MODE:
    env->main_next_event = wf_next_event;
    FAXPP_set_normalize_attrs(env, 1);
    break;
  }

  env->next_event = env->main_next_event;

  return env;
}

void FAXPP_free_parser(FAXPP_Parser *env)
{
  FAXPP_AttrValue *at;
  FAXPP_ElementInfo *el;
  FAXPP_NamespaceInfo *ns;

  if(env->attrs) free(env->attrs);

  while(env->av_dealloc) {
    at = env->av_dealloc;
    env->av_dealloc = at->dealloc_next;
    free(at);
  }

  while(env->element_info_stack) {
    el = env->element_info_stack;
    env->element_info_stack = el->prev;
    free(el);
  }

  while(env->namespace_stack) {
    ns = env->namespace_stack;
    env->namespace_stack = ns->prev;
    free(ns);
  }

  while(env->element_info_pool) {
    el = env->element_info_pool;
    env->element_info_pool = el->prev;
    free(el);
  }

  while(env->namespace_pool) {
    ns = env->namespace_pool;
    env->namespace_pool = ns->prev;
    free(ns);
  }

  if(env->read_buffer) free(env->read_buffer);

  FAXPP_free_buffer(&env->event_buffer);
  FAXPP_free_buffer(&env->stack_buffer);

  free_tokenizer_internal(&env->tenv);
  free(env);
}

void FAXPP_set_null_terminate(FAXPP_Parser *parser, unsigned int boolean)
{
  parser->null_terminate = boolean != 0;
}

void FAXPP_set_normalize_attrs(FAXPP_Parser *parser, unsigned int boolean)
{
  parser->tenv.normalize_attrs = boolean != 0;
}

void FAXPP_set_encode(FAXPP_Parser *parser, FAXPP_EncodeFunction encode)
{
  parser->tenv.encode = encode;
}


FAXPP_DecodeFunction FAXPP_get_decode(const FAXPP_Parser *parser)
{
  return parser->tenv.decode;
}

void FAXPP_set_decode(FAXPP_Parser *parser, FAXPP_DecodeFunction decode)
{
  FAXPP_set_tokenizer_decode(&parser->tenv, decode);
  if(parser->next_event == nc_unsupported_encoding_next_event) {
    parser->next_event = parser->main_next_event;
  }
}

static FAXPP_Error p_reset_parser(FAXPP_ParserEnv *env, int allocate_buffer)
{
  // Reset the stack buffer cursor
  FAXPP_reset_buffer(&env->stack_buffer);

  if(allocate_buffer && !env->read_buffer) {
    env->read_buffer = malloc(READ_BUFFER_SIZE);
    if(!env->read_buffer) return OUT_OF_MEMORY;
    env->read_buffer_length = READ_BUFFER_SIZE;
  }

  env->next_event = env->main_next_event;

  return NO_ERROR;
}

FAXPP_Error FAXPP_init_parse(FAXPP_Parser *env, void *buffer, unsigned int length, unsigned int done)
{
  FAXPP_Error err = p_reset_parser(env, /*allocate_buffer*/0);
  if(err != 0) return err;

  env->read = 0;
  env->read_user_data = 0;

  return FAXPP_init_tokenize(&env->tenv, buffer, length, done);
}

static unsigned int p_file_read_callback(void *userData, void *buffer, unsigned int length)
{
  /*return fread(buffer, 1, length, (FILE*)userData);*/
  return 0;
}

/*
FAXPP_Error FAXPP_init_parse_file(FAXPP_Parser *env, FILE *file)
{
  return FAXPP_init_parse_callback(env, p_file_read_callback, (void*)file);
}
*/

FAXPP_Error FAXPP_init_parse_callback(FAXPP_Parser *env, FAXPP_ReadCallback callback, void *userData)
{
  FAXPP_Error err = p_reset_parser(env, /*allocate_buffer*/1);
  if(err != 0) return err;

  env->read = callback;
  env->read_user_data = userData;

  unsigned int len = env->read(env->read_user_data, env->read_buffer, env->read_buffer_length);

  return FAXPP_init_tokenize(&env->tenv, env->read_buffer, len, /*done*/len != env->read_buffer_length);
}

FAXPP_Error FAXPP_next_event(FAXPP_Parser *env)
{
  return env->next_event(env);
}

const FAXPP_Event *FAXPP_get_current_event(const FAXPP_Parser *parser)
{
  return &parser->event;
}

#define p_find_ns_info(env, prefix, uri) (((env)->namespace_stack) ? p_find_ns_info_impl((env), (prefix), (uri)) : (((prefix)->len == 0) ? NO_ERROR : NO_URI_FOR_PREFIX))

static FAXPP_Error p_find_ns_info_impl(const FAXPP_ParserEnv *env, const FAXPP_Text *prefix, FAXPP_Text *uri);

FAXPP_Error FAXPP_lookup_namespace_uri(const FAXPP_Parser *parser, const FAXPP_Text *prefix, FAXPP_Text *uri)
{
  uri->ptr = 0;
  uri->len = 0;
  return p_find_ns_info(parser, prefix, uri);
}

unsigned int FAXPP_get_nesting_level(const FAXPP_Parser *parser)
{
  return parser->tenv.nesting_level;
}

unsigned int FAXPP_get_error_line(const FAXPP_Parser *parser)
{
  return parser->err_line;
}

unsigned int FAXPP_get_error_column(const FAXPP_Parser *parser)
{
  return parser->err_column;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void p_text_change_buffer(FAXPP_Buffer *buffer, void *newBuffer, FAXPP_Text *text)
{
  if(text->ptr >= buffer->buffer && text->ptr < (buffer->buffer + buffer->length)) {
    text->ptr += newBuffer - buffer->buffer;
  }
}

static void p_change_event_buffer(void *userData, FAXPP_Buffer *buffer, void *newBuffer)
{
  unsigned int i;
  FAXPP_AttrValue *atval;

  FAXPP_ParserEnv *env = (FAXPP_ParserEnv*)userData;

  p_text_change_buffer(buffer, newBuffer, &env->event.prefix);
  p_text_change_buffer(buffer, newBuffer, &env->event.uri);
  p_text_change_buffer(buffer, newBuffer, &env->event.name);
  p_text_change_buffer(buffer, newBuffer, &env->event.value);
  p_text_change_buffer(buffer, newBuffer, &env->event.version);
  p_text_change_buffer(buffer, newBuffer, &env->event.encoding);
  p_text_change_buffer(buffer, newBuffer, &env->event.standalone);

  for(i = 0; i < env->event.attr_count; ++i) {
    p_text_change_buffer(buffer, newBuffer, &env->event.attrs[i].prefix);
    p_text_change_buffer(buffer, newBuffer, &env->event.attrs[i].uri);
    p_text_change_buffer(buffer, newBuffer, &env->event.attrs[i].name);

    atval = &env->event.attrs[i].value;
    while(atval) {
      p_text_change_buffer(buffer, newBuffer, &atval->value);
      atval = atval->next;
    }
  }
}

static void p_change_stack_buffer(void *userData, FAXPP_Buffer *buffer, void *newBuffer)
{
  FAXPP_ParserEnv *env = (FAXPP_ParserEnv*)userData;

  FAXPP_ElementInfo *el = env->element_info_stack;
  while(el) {
    p_text_change_buffer(buffer, newBuffer, &el->prefix);
    p_text_change_buffer(buffer, newBuffer, &el->uri);
    p_text_change_buffer(buffer, newBuffer, &el->name);

    el->prev_stack_cursor += newBuffer - buffer->buffer;

    el = el->prev;
  }

  FAXPP_NamespaceInfo *ns = env->namespace_stack;
  while(ns) {
    p_text_change_buffer(buffer, newBuffer, &ns->prefix);
    p_text_change_buffer(buffer, newBuffer, &ns->uri);

    ns = ns->prev;
  }
}

#define p_move_text_to_event_buffer(env, text) \
{ \
  if((text)->ptr >= (env)->tenv.buffer && (text)->ptr < (env)->tenv.buffer_end) { \
    void *newPtr = (env)->event_buffer.cursor; \
    FAXPP_Error err = FAXPP_buffer_append(&(env)->event_buffer, (text)->ptr, (text)->len); \
    if((env)->null_terminate && err == 0) \
      err = FAXPP_buffer_append_ch(&(env)->event_buffer, (env)->tenv.encode, 0); \
    if(err != 0) return err; \
    (text)->ptr = newPtr; \
  } \
}

FAXPP_Error FAXPP_release_buffer(FAXPP_Parser *env, void **buffer_position)
{
  unsigned int i;
  FAXPP_AttrValue *atval;
  FAXPP_Error err;

  err = FAXPP_tokenizer_release_buffer(&env->tenv, buffer_position);
  if(err != 0) return err;

  // Copy any strings in the event which point to the old buffer
  // into the event_buffer
  p_move_text_to_event_buffer(env, &env->event.prefix);
  p_move_text_to_event_buffer(env, &env->event.uri);
  p_move_text_to_event_buffer(env, &env->event.name);
  p_move_text_to_event_buffer(env, &env->event.value);
  p_move_text_to_event_buffer(env, &env->event.version);
  p_move_text_to_event_buffer(env, &env->event.encoding);
  p_move_text_to_event_buffer(env, &env->event.standalone);

  for(i = 0; i < env->event.attr_count; ++i) {
    p_move_text_to_event_buffer(env, &env->event.attrs[i].prefix);
    p_move_text_to_event_buffer(env, &env->event.attrs[i].uri);
    p_move_text_to_event_buffer(env, &env->event.attrs[i].name);

    atval = &env->event.attrs[i].value;
    while(atval) {
      p_move_text_to_event_buffer(env, &atval->value);
      atval = atval->next;
    }
  }

  return NO_ERROR;
}

FAXPP_Error FAXPP_continue_parse(FAXPP_Parser *env, void *buffer,
                                 unsigned int length, unsigned int done)
{
  return FAXPP_continue_tokenize(&env->tenv, buffer, length, done);
}

static FAXPP_Error p_read_more(FAXPP_ParserEnv *env)
{
  unsigned int len = 0;
  unsigned int readlen;
  FAXPP_Error err;

  err = FAXPP_release_buffer(env, 0);
  if(err != 0) return err;

  if(env->tenv.position < env->tenv.buffer_end) {
    // We're half way through a charcter, so we need to copy
    // the partial char to the begining of the buffer to keep
    // it for the next parse
    len = env->tenv.buffer_end - env->tenv.position;
    memmove(env->read_buffer, env->tenv.position, len);
  }

  readlen = env->read(env->read_user_data, env->read_buffer, env->read_buffer_length - len);
  if(readlen == 0)
    return PREMATURE_END_OF_BUFFER;

  len += readlen;
  return FAXPP_continue_parse(env, env->read_buffer, len, /*done*/len != env->read_buffer_length);
}

#define p_check_err(err, env) \
{ \
  if((err) != NO_ERROR) { \
    if((err) == PREMATURE_END_OF_BUFFER && (env)->read) { \
      (err) = p_read_more((env)); \
      if((err) != NO_ERROR) { \
        set_err_info_from_tokenizer((env)); \
        return (err); \
      } \
    } else { \
      set_err_info_from_tokenizer((env)); \
      return (err); \
    } \
  } \
}

#define BUF_SIZE 50

/* static void p_print_token(FAXPP_ParserEnv *env) */
/* { */
/*   char buf[BUF_SIZE + 1]; */
/*   if(env->token.value.ptr != 0) { */
/*     if(env->token.value.len > BUF_SIZE) { */
/*       strncpy(buf, env->token.value.ptr, BUF_SIZE - 3); */
/*       buf[BUF_SIZE - 3] = '.'; */
/*       buf[BUF_SIZE - 2] = '.'; */
/*       buf[BUF_SIZE - 1] = '.'; */
/*       buf[BUF_SIZE] = 0; */
/*     } */
/*     else { */
/*       strncpy(buf, env->token.value.ptr, env->token.value.len); */
/*       buf[env->token.value.len] = 0; */
/*     } */
/*     printf("%03d:%03d Token ID: %s, Token: \"%s\"\n", env->token.line, env->token.column, FAXPP_token_to_string(&env->token), buf); */
/*   } */
/*   else { */
/*     printf("%03d:%03d Token ID: %s\n", env->token.line, env->token.column, FAXPP_token_to_string(&env->token)); */
/*   } */
/* } */

#define p_next_token(err, env) \
{ \
  (env)->tenv.result_token.type = NO_TOKEN; \
  while((env)->tenv.result_token.type == NO_TOKEN) { \
    (err) = (env)->tenv.state(&(env)->tenv); \
    p_check_err((err), (env)); \
  } \
/*   p_print_token(env); */ \
}

#define p_set_text_from_text(text, o) \
{ \
  (text)->ptr = (o)->ptr; \
  (text)->len = (o)->len; \
}

#define p_copy_text_from_event(text, o, env) \
{ \
  /* Always copy the string to the stack_buffer, to avoid complications */ \
  /* when we have to swap buffers */ \
/*   if(((o)->ptr >= (env)->event_buffer.buffer && */ \
/*       (o)->ptr < ((env)->event_buffer.buffer + (env)->event_buffer.length)) || */ \
/*      (env)->null_terminate) { */ \
    (text)->ptr = (env)->stack_buffer.cursor; \
    (text)->len = (o)->len; \
    FAXPP_Error err = FAXPP_buffer_append(&(env)->stack_buffer, (o)->ptr, (o)->len); \
    if((env)->null_terminate && err == 0) \
      err = FAXPP_buffer_append_ch(&(env)->stack_buffer, (env)->tenv.encode, 0); \
    if(err != 0) return err; \
/*   } else { */ \
/*     p_set_text_from_text((text), (o)); */ \
/*   } */ \
}

#define p_copy_text_from_attr_value(text, attrval, env) \
{ \
  if((attrval)->next == 0) { \
    p_copy_text_from_event((text), &(attrval)->value, (env)); \
  } \
  else { \
    FAXPP_Error err = p_normalize_attr_value((text), &(env)->stack_buffer, (attrval), (env)); \
    if(err != NO_ERROR) return err; \
  } \
}

FAXPP_Error p_normalize_attr_value(FAXPP_Text *text, FAXPP_Buffer *buffer, const FAXPP_AttrValue *value, const FAXPP_ParserEnv *env)
{
  FAXPP_Error err;

  text->ptr = buffer->cursor;

  while(value) {
    err = FAXPP_buffer_append(buffer, value->value.ptr, value->value.len);
    if(err != NO_ERROR) return err;
    value = value->next;
  }
  
  text->len = buffer->cursor - text->ptr;

  if(env->null_terminate)
    return FAXPP_buffer_append_ch(buffer, env->tenv.encode, 0);

  return NO_ERROR;
}

#define p_copy_text_from_token(text, env, useTokenBuffer) \
{ \
  /* TBD null terminate in tokenizer - jpcs */ \
  (text)->len = (env)->tenv.result_token.value.len; \
  if(((useTokenBuffer) || (env)->tenv.result_token.value.ptr != (env)->tenv.token_buffer.buffer) && !(env)->null_terminate) { \
    (text)->ptr = (env)->tenv.result_token.value.ptr; \
  } else { \
    (text)->ptr = (env)->event_buffer.cursor; \
    FAXPP_Error err = FAXPP_buffer_append(&(env)->event_buffer, (env)->tenv.result_token.value.ptr, (env)->tenv.result_token.value.len); \
    if((env)->null_terminate && err == 0) \
      err = FAXPP_buffer_append_ch(&(env)->event_buffer, (env)->tenv.encode, 0); \
    if(err != 0) return err; \
  } \
}

#define p_set_location_from_token(env) \
{ \
  if((env)->event.line == 0) { \
    (env)->event.line = (env)->tenv.result_token.line; \
    (env)->event.column = (env)->tenv.result_token.column; \
  } \
}

static FAXPP_Error p_next_attr(FAXPP_ParserEnv *env)
{
  if(env->event.attr_count == env->max_attr_count) {
    env->max_attr_count = env->max_attr_count << 1;
    env->attrs = (FAXPP_Attribute*)realloc(env->attrs, sizeof(FAXPP_Attribute) * env->max_attr_count);
    if(!env->attrs) return OUT_OF_MEMORY;
  }
  env->event.attrs = env->attrs;
  env->current_attr = env->event.attrs + env->event.attr_count;
  env->event.attr_count += 1;

  memset(env->current_attr, 0, sizeof(FAXPP_Attribute));

  return NO_ERROR;
}

static FAXPP_AttrValue *p_next_attr_value(FAXPP_ParserEnv *env)
{
  FAXPP_AttrValue *atval;

  if(env->av_ptr) {
    atval = env->av_ptr;
    env->av_ptr = atval->dealloc_next;

    memset(atval, 0, sizeof(FAXPP_AttrValue));
    atval->dealloc_next = env->av_ptr;
  } else {
    atval = (FAXPP_AttrValue*)malloc(sizeof(FAXPP_AttrValue));
    if(!atval) return 0;

    memset(atval, 0, sizeof(FAXPP_AttrValue));
    atval->dealloc_next = env->av_dealloc;
    env->av_dealloc = atval;
  }

  return atval;
}

static void p_set_location_from_token_a(FAXPP_Attribute *attr, FAXPP_ParserEnv *env)
{
  if(attr->line == 0) {
    attr->line = env->tenv.result_token.line;
    attr->column = env->tenv.result_token.column;
  }
}

static FAXPP_Error p_set_attr_value(FAXPP_Attribute *attr, FAXPP_ParserEnv *env, FAXPP_EventType type)
{
  FAXPP_AttrValue *newatval;

  if(attr->value.value.ptr == 0) {
    newatval = &attr->value;
  }
  else {
    newatval = p_next_attr_value(env);
    if(!newatval) return OUT_OF_MEMORY;

    /* Add newatval to the end of the linked list */
    FAXPP_AttrValue *atval = &attr->value;
    while(atval->next) atval = atval->next;
    atval->next = newatval;
  }

  p_copy_text_from_token(&newatval->value, env, /*useTokenBuffer*/0);
  newatval->type = type;

  return NO_ERROR;
}

#define p_set_text_to_char(text, env, ch) \
{ \
  (text)->ptr = (env)->event_buffer.cursor; \
  FAXPP_Error err = FAXPP_buffer_append_ch(&(env)->event_buffer, (env)->tenv.encode, (ch)); \
  (text)->len = (env)->event_buffer.cursor - (text)->ptr; \
  if((env)->null_terminate && err == 0) \
    err = FAXPP_buffer_append_ch(&(env)->event_buffer, (env)->tenv.encode, 0); \
  if(err != 0) return err; \
}

static FAXPP_Error p_set_attr_value_name(FAXPP_Attribute *attr, FAXPP_ParserEnv *env, FAXPP_EventType type, Char32 ch)
{
  FAXPP_AttrValue *newatval;

  if(attr->value.value.ptr == 0) {
    newatval = &attr->value;
  }
  else {
    newatval = p_next_attr_value(env);
    if(!newatval) return OUT_OF_MEMORY;

    /* Add newatval to the end of the linked list */
    FAXPP_AttrValue *atval = &attr->value;
    while(atval->next) atval = atval->next;
    atval->next = newatval;
  }

  p_copy_text_from_token(&newatval->name, env, /*useTokenBuffer*/0);
  newatval->type = type;

  if(ch != 0) {
    p_set_text_to_char(&newatval->value, env, ch);
  }

  return NO_ERROR;
}

static void p_reset_event(FAXPP_ParserEnv *env)
{
  // Reset the attribute value store
  env->av_ptr = env->av_dealloc;

  // Reset the event buffer cursor
  FAXPP_reset_buffer(&env->event_buffer);

  // Clear the event
  env->event.type = NO_EVENT;

  env->event.prefix.ptr = 0;
  env->event.prefix.len = 0;
  env->event.uri.ptr = 0;
  env->event.uri.len = 0;
  env->event.name.ptr = 0;
  env->event.name.len = 0;
  env->event.value.ptr = 0;
  env->event.value.len = 0;

  env->event.attr_count = 0;

  env->event.version.ptr = 0;
  env->event.version.len = 0;
  env->event.encoding.ptr = 0;
  env->event.encoding.len = 0;
  env->event.standalone.ptr = 0;
  env->event.standalone.len = 0;

  env->event.line = 0;
}

static void set_err_info_from_tokenizer(FAXPP_ParserEnv *env)
{
  env->err_line = env->tenv.line;
  env->err_column = env->tenv.column;
}

static void set_err_info_from_event(FAXPP_ParserEnv *env)
{
  env->err_line = env->event.line;
  env->err_column = env->event.column;
}

static void set_err_info_from_attr(FAXPP_ParserEnv *env, const FAXPP_Attribute *attr)
{
  env->err_line = attr->line;
  env->err_column = attr->column;
}

// Needs upper case strings passed to it
static int p_case_insensitive_equals(const char *str, FAXPP_EncodeFunction encode, const FAXPP_Text *text)
{
  // No encoding represents a character with as many as 10 bytes
  uint8_t encode_buffer[10];
  unsigned int encode_len;

  void *text_ptr = text->ptr;
  void *text_end = text_ptr + text->len;

  while(*str != 0) {
    if(text_ptr >= text_end) return 0;

    encode_len = encode(encode_buffer, encode_buffer + sizeof(encode_buffer), *str);
    if((text_end - text_ptr) < encode_len || memcmp(encode_buffer, text_ptr, encode_len) != 0) {
      if(*str >= 'A' && *str <= 'Z') {
        // Try the lower case letter as well
        encode_len = encode(encode_buffer, encode_buffer + sizeof(encode_buffer), (*str) - 'A' + 'a');
        if((text_end - text_ptr) < encode_len || memcmp(encode_buffer, text_ptr, encode_len) != 0)
          return 0;
      }
      else return 0;
    }

    text_ptr += encode_len;
    ++str;
  }

  return text_ptr == text_end;
}

static FAXPP_Error nc_unsupported_encoding_next_event(FAXPP_ParserEnv *env)
{
  return UNSUPPORTED_ENCODING;
}

static FAXPP_Error nc_next_event(FAXPP_ParserEnv *env)
{
  FAXPP_Error err = 0;

  p_reset_event(env);

  while(1) {
    p_next_token(err, env);

    switch(env->tenv.result_token.type) {
    case XML_DECL_VERSION_TOKEN:
      p_copy_text_from_token(&env->event.version, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case XML_DECL_ENCODING_TOKEN:
      p_copy_text_from_token(&env->event.encoding, env, /*useTokenBuffer*/0);
      break;
    case XML_DECL_STANDALONE_TOKEN:
      p_copy_text_from_token(&env->event.standalone, env, /*useTokenBuffer*/0);
      break;
    case XML_DECL_END_TOKEN:
      env->next_event = nc_unsupported_encoding_next_event;
      env->event.type = START_DOCUMENT_EVENT;

      // Check the encoding string against our internally supported encodings
      if(env->event.encoding.ptr == 0) {
          env->next_event = env->main_next_event;
      }
      else if(p_case_insensitive_equals("UTF-8", env->tenv.encode, &env->event.encoding)) {
        env->next_event = env->main_next_event;
        if(env->tenv.decode != FAXPP_utf8_decode)
          return BAD_ENCODING;
      }
      else if(p_case_insensitive_equals("UTF-16", env->tenv.encode, &env->event.encoding)) {
        env->next_event = env->main_next_event;
        if(env->tenv.decode != FAXPP_utf16_le_decode &&
           env->tenv.decode != FAXPP_utf16_be_decode &&
           env->tenv.decode != FAXPP_utf16_native_decode)
          return BAD_ENCODING;
      }
      else if(p_case_insensitive_equals("UTF-16LE", env->tenv.encode, &env->event.encoding)) {
        env->next_event = env->main_next_event;
        if(env->tenv.decode != FAXPP_utf16_le_decode
#ifndef WORDS_BIGENDIAN
           && env->tenv.decode != FAXPP_utf16_native_decode
#endif
           )
          return BAD_ENCODING;
      }
      else if(p_case_insensitive_equals("UTF-16BE", env->tenv.encode, &env->event.encoding)) {
        env->next_event = env->main_next_event;
        if(env->tenv.decode != FAXPP_utf16_be_decode
#ifdef WORDS_BIGENDIAN
           && env->tenv.decode != FAXPP_utf16_native_decode
#endif
           )
          return BAD_ENCODING;
      }
      else if(p_case_insensitive_equals("ISO-10646-UCS-4", env->tenv.encode, &env->event.encoding)) {
        env->next_event = env->main_next_event;
        if(env->tenv.decode != FAXPP_ucs4_le_decode &&
           env->tenv.decode != FAXPP_ucs4_be_decode &&
           env->tenv.decode != FAXPP_ucs4_native_decode)
          return BAD_ENCODING;
      }
      else if(p_case_insensitive_equals("ISO-8859-1", env->tenv.encode, &env->event.encoding)) {
        FAXPP_set_decode(env, FAXPP_iso_8859_1_decode);
      }
    
      return NO_ERROR;
    case START_ELEMENT_PREFIX_TOKEN:
      p_copy_text_from_token(&env->event.prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case START_ELEMENT_NAME_TOKEN:
      p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case XMLNS_PREFIX_TOKEN:
      err = p_next_attr(env);
      if(err != 0) {
        set_err_info_from_tokenizer(env);
        return err;
      }

      p_copy_text_from_token(&env->current_attr->prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      env->current_attr->xmlns_attr = 1;
      break;
    case XMLNS_NAME_TOKEN:
      err = p_next_attr(env);
      if(err != 0) {
        set_err_info_from_tokenizer(env);
        return err;
      }

      p_copy_text_from_token(&env->current_attr->name, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      env->current_attr->xmlns_attr = 1;
      break;
    case XML_PREFIX_TOKEN:
      err = p_next_attr(env);
      if(err != 0) {
        set_err_info_from_tokenizer(env);
        return err;
      }

      p_copy_text_from_token(&env->current_attr->prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      env->current_attr->xml_attr = 1;
      break;
    case ATTRIBUTE_PREFIX_TOKEN:
      err = p_next_attr(env);
      if(err != 0) {
        set_err_info_from_tokenizer(env);
        return err;
      }

      p_copy_text_from_token(&env->current_attr->prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      break;
    case ATTRIBUTE_NAME_TOKEN:
      if(!env->current_attr || env->current_attr->name.ptr != 0) {
        err = p_next_attr(env);
        if(err != 0) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      }
      p_copy_text_from_token(&env->current_attr->name, env, /*useTokenBuffer*/0);
      p_set_location_from_token_a(env->current_attr, env);
      break;
    case ATTRIBUTE_VALUE_TOKEN:
      err = p_set_attr_value(env->current_attr, env, CHARACTERS_EVENT);
      if(err) {
        set_err_info_from_tokenizer(env);
        return err;
      }
      break;
    case START_ELEMENT_END_TOKEN:
      env->event.type = START_ELEMENT_EVENT;
      env->current_attr = 0;
      return NO_ERROR;
    case SELF_CLOSING_ELEMENT_TOKEN:
      env->event.type = SELF_CLOSING_ELEMENT_EVENT;
      env->current_attr = 0;
      return NO_ERROR;
    case END_ELEMENT_PREFIX_TOKEN:
      p_copy_text_from_token(&env->event.prefix, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case END_ELEMENT_NAME_TOKEN:
      p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      env->event.type = END_ELEMENT_EVENT;
      return NO_ERROR;
    case CHARACTERS_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = CHARACTERS_EVENT;
      return NO_ERROR;
    case CDATA_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = CDATA_EVENT;
      return NO_ERROR;
    case IGNORABLE_WHITESPACE_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = IGNORABLE_WHITESPACE_EVENT;
      return NO_ERROR;
    case COMMENT_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/1);
      p_set_location_from_token(env);
      env->event.type = COMMENT_EVENT;
      return NO_ERROR;
    case PI_NAME_TOKEN:
      p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/0);
      p_set_location_from_token(env);
      break;
    case PI_VALUE_TOKEN:
      p_copy_text_from_token(&env->event.value, env, /*useTokenBuffer*/0);
      env->event.type = PI_EVENT;
      return NO_ERROR;
    case AMP_ENTITY_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, ENTITY_REFERENCE_EVENT, '&');
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_text_to_char(&env->event.value, env, '&');
        p_set_location_from_token(env);
        env->event.type = ENTITY_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case APOS_ENTITY_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, ENTITY_REFERENCE_EVENT, '\'');
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_text_to_char(&env->event.value, env, '\'');
        p_set_location_from_token(env);
        env->event.type = ENTITY_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case GT_ENTITY_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, ENTITY_REFERENCE_EVENT, '>');
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_text_to_char(&env->event.value, env, '>');
        p_set_location_from_token(env);
        env->event.type = ENTITY_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case LT_ENTITY_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, ENTITY_REFERENCE_EVENT, '<');
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_text_to_char(&env->event.value, env, '<');
        p_set_location_from_token(env);
        env->event.type = ENTITY_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case QUOT_ENTITY_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, ENTITY_REFERENCE_EVENT, '"');
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_text_to_char(&env->event.value, env, '"');
        p_set_location_from_token(env);
        env->event.type = ENTITY_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;

    case ENTITY_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, ENTITY_REFERENCE_EVENT, 0);
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_location_from_token(env);
        env->event.type = ENTITY_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case DEC_CHAR_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, DEC_CHAR_REFERENCE_EVENT, 0);
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_location_from_token(env);
        env->event.type = DEC_CHAR_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case HEX_CHAR_REFERENCE_TOKEN:
      if(env->current_attr) {
        err = p_set_attr_value_name(env->current_attr, env, HEX_CHAR_REFERENCE_EVENT, 0);
        if(err) {
          set_err_info_from_tokenizer(env);
          return err;
        }
      } else {
        p_copy_text_from_token(&env->event.name, env, /*useTokenBuffer*/1);
        p_set_location_from_token(env);
        env->event.type = HEX_CHAR_REFERENCE_EVENT;
        return NO_ERROR;
      }
      break;
    case END_OF_BUFFER_TOKEN:
      p_set_location_from_token(env);
      env->event.type = END_DOCUMENT_EVENT;
      return NO_ERROR;
    case NO_TOKEN:
      break;
    }
  }

  // Never happens
  return NO_ERROR;
}

#define p_compare_text(a, b) (((a)->len == (b)->len) ? memcmp((a)->ptr, (b)->ptr, (a)->len) : ((a)->len - (b)->len))

/* static int p_compare_text(const FAXPP_Text *a, const FAXPP_Text *b) */
/* { */
/*   int cmp = a->len - b->len; */
/*   if(cmp != 0) return cmp; */

/*   return memcmp(a->ptr, b->ptr, a->len); */
/* } */

static FAXPP_Error p_add_ns_info(FAXPP_ParserEnv *env, const FAXPP_Attribute *attr)
{
  FAXPP_NamespaceInfo *nsinfo = env->namespace_pool;

  if(nsinfo == 0) {
    nsinfo = (FAXPP_NamespaceInfo*)malloc(sizeof(FAXPP_NamespaceInfo));
    if(!nsinfo) return OUT_OF_MEMORY;
  }
  else {
    env->namespace_pool = nsinfo->prev;
  }

  memset(nsinfo, 0, sizeof(FAXPP_NamespaceInfo));
  nsinfo->prev = env->namespace_stack;
  env->namespace_stack = nsinfo;

  p_copy_text_from_attr_value(&nsinfo->uri, &attr->value, env);

  if(attr->prefix.len != 0) {
    p_copy_text_from_event(&nsinfo->prefix, &attr->name, env);
  }

  return NO_ERROR;
}

static FAXPP_Error p_find_ns_info_impl(const FAXPP_ParserEnv *env, const FAXPP_Text *prefix, FAXPP_Text *uri)
{
  const FAXPP_NamespaceInfo *nsinfo;

  nsinfo = env->namespace_stack;
  while(nsinfo != 0) {
    if(p_compare_text(prefix, &nsinfo->prefix) == 0) {
      if(nsinfo->prefix.len != 0 && nsinfo->uri.len == 0)
        return NO_URI_FOR_PREFIX;
      p_set_text_from_text(uri, &nsinfo->uri);
      return NO_ERROR;
    }
    nsinfo = nsinfo->prev;
  }

  if(prefix->len == 0) {
    /* The default namespace is implicitly set to no namespace */
    return NO_ERROR;
  }

  // The prefix "xml" is always bound to the namespace URI "http://www.w3.org/XML/1998/namespace"
  // TBD implement this properly - jpcs

  return NO_URI_FOR_PREFIX;
}

static FAXPP_Error p_push_element(FAXPP_ParserEnv *env)
{
  FAXPP_ElementInfo *einfo = env->element_info_pool;

  if(einfo == 0) {
    einfo = (FAXPP_ElementInfo*)malloc(sizeof(FAXPP_ElementInfo));
    if(!einfo) return OUT_OF_MEMORY;
  }
  else {
    env->element_info_pool = einfo->prev; 
  }

  einfo->prev = env->element_info_stack;
  env->element_info_stack = einfo;

  /* Store the current place in the namespace stack */
  einfo->prev_ns = env->namespace_stack;

  /* Store the current place in the stack buffer */
  einfo->prev_stack_cursor = env->stack_buffer.cursor;

  einfo->uri.ptr = 0; einfo->uri.len = 0;
  p_copy_text_from_event(&einfo->prefix, &env->event.prefix, env);
  p_copy_text_from_event(&einfo->name, &env->event.name, env);

  return NO_ERROR;
}

static void p_pop_element(FAXPP_ParserEnv *env)
{
  FAXPP_ElementInfo *einfo;
  FAXPP_NamespaceInfo *ns;

  einfo = env->element_info_stack;

  /* Take the namespace_stack back to it's
     position before this element */
  while(env->namespace_stack != einfo->prev_ns) {
    ns = env->namespace_stack;
    env->namespace_stack = ns->prev;

    /* Put the FAXPP_NamespaceInfo object back in the pool */
    ns->prev = env->namespace_pool;
    env->namespace_pool = ns;
  }

  /* Return the stack_buffer back to it's state before this element */
  env->stack_buffer.cursor = einfo->prev_stack_cursor;

  env->element_info_stack = einfo->prev;

  /* Put the FAXPP_ElementInfo object back in the pool */
  einfo->prev = env->element_info_pool;
  env->element_info_pool = einfo;
}

static Char32 p_dec_char_ref_value(const FAXPP_Text *text, FAXPP_ParserEnv *env)
{
  Char32 ch, result = 0;

  void *text_ptr = text->ptr;
  void *text_end = text_ptr + text->len;

  while(text_ptr < text_end) {
    text_ptr += env->tenv.decode(text_ptr, text_end, &ch);
    result *= 10;
    if(ch <= '9' && ch >= '0')
      result += ch - '0';
  }

  return result;
}

static Char32 p_hex_char_ref_value(const FAXPP_Text *text, FAXPP_ParserEnv *env)
{
  Char32 ch, result = 0;

  void *text_ptr = text->ptr;
  void *text_end = text_ptr + text->len;

  while(text_ptr < text_end) {
    text_ptr += env->tenv.decode(text_ptr, text_end, &ch);
    result <<= 4;
    if(ch <= '9' && ch >= '0')
      result += ch - '0';
    else if(ch <= 'F' && ch >= 'A')
      result += ch - 'A' + 10;
    else if(ch <= 'f' && ch >= 'a')
      result += ch - 'a' + 10;
  }

  return result;
}

static FAXPP_Error wf_next_event(FAXPP_ParserEnv *env)
{
  int i, j;
  FAXPP_Attribute *attr, *attr2;
  FAXPP_AttrValue *attrVal;
  FAXPP_Text tmpText;
  Char32 ch;

  FAXPP_Error err = nc_next_event(env);
  if(err != 0) return err;

  switch(env->event.type) {
  case START_DOCUMENT_EVENT:
    break;
  case PI_EVENT:
    break;
  case END_DOCUMENT_EVENT:
    break;
  case START_ELEMENT_EVENT:
  case SELF_CLOSING_ELEMENT_EVENT:
    /* Store the element name */
    err = p_push_element(env);
    if(err != 0) {
      set_err_info_from_event(env);
      return err;
    }

    for(i = 0; i < env->event.attr_count; ++i) {
      attr = &env->event.attrs[i];

      /* Check that the character references are valid */
      for(attrVal = &attr->value; attrVal != 0; attrVal = attrVal->next) {
        switch(attrVal->type) {
        case ENTITY_REFERENCE_EVENT:
          /* [WFC: Entity Declared] */
          /* [WFC: Parsed Entity] */
          /* [WFC: No Recursion] */
          // TBD
          if(attrVal->value.ptr == 0) {
            set_err_info_from_attr(env, attr);
            return UNDEFINED_ENTITY;
          }
          
          break;
        case DEC_CHAR_REFERENCE_EVENT:
          /* [WFC: Legal Character] */
          ch = p_dec_char_ref_value(&attrVal->name, env);
          if((FAXPP_char_flags(ch) & env->tenv.non_restricted_char) == 0) {
            set_err_info_from_attr(env, attr);
            return RESTRICTED_CHAR;
          }

          attrVal->value.ptr = env->event_buffer.cursor;
          err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, ch);
          attrVal->value.len = env->event_buffer.cursor - attrVal->value.ptr;

          if(env->null_terminate && err == 0)
            err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, 0);
          if(err != 0) return err;
          break;
        case HEX_CHAR_REFERENCE_EVENT:
          /* [WFC: Legal Character] */
          ch = p_hex_char_ref_value(&attrVal->name, env);
          if((FAXPP_char_flags(ch) & env->tenv.non_restricted_char) == 0) {
            set_err_info_from_attr(env, attr);
            return RESTRICTED_CHAR;
          }

          attrVal->value.ptr = env->event_buffer.cursor;
          err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, ch);
          attrVal->value.len = env->event_buffer.cursor - attrVal->value.ptr;

          if(env->null_terminate && err == 0)
            err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, 0);
          if(err != 0) return err;
          break;
        default: break;
        }
      }

      /* Normalize the attribute values if required */
      if(env->tenv.normalize_attrs && attr->value.next != 0) {
        err = p_normalize_attr_value(&tmpText, &env->event_buffer, &attr->value, env);
        if(err != 0) return err;

        attr->value.type = CHARACTERS_EVENT;
        attr->value.name.ptr = 0;
        attr->value.name.len = 0;
        attr->value.value.ptr = tmpText.ptr;
        attr->value.value.len = tmpText.len;
        attr->value.next = 0;
      }

      /* Check for namespace attributes */
      if(attr->xmlns_attr) {
        err = p_add_ns_info(env, attr);
        if(err != 0) {
          set_err_info_from_attr(env, attr);
          return err;
        }
      }
    }

    /* Resolve the element's URI */
    err = p_find_ns_info(env, &env->event.prefix, &env->event.uri);
    if(err != 0) {
      set_err_info_from_event(env);
      return err;
    }

    /* Copy the element's URI to the element stack */
    /* No need to use the stack_buffer, since the value already comes from there */
    p_set_text_from_text(&env->element_info_stack->uri, &env->event.uri);

    for(i = 0; i < env->event.attr_count; ++i) {
      attr = &env->event.attrs[i];
      /* Resolve the attributes' URIs */
      if(!attr->xmlns_attr && !attr->xml_attr && attr->prefix.len != 0) {
        err = p_find_ns_info(env, &attr->prefix, &attr->uri);
        if(err != 0) {
          set_err_info_from_attr(env, attr);
          return err;
        }
      }
    }

    for(i = 0; i < env->event.attr_count; ++i) {
      attr = &env->event.attrs[i];
      /* [WFC: Unique Att Spec] */
      /* Nested loop check for duplicate attributes */
      /* TBD make this a better algorithm? - jpcs */
      for(j = i + 1; j < env->event.attr_count; ++j) {
        attr2 = &env->event.attrs[j];
        if(attr2->xmlns_attr == attr->xmlns_attr &&
           p_compare_text(&attr2->name, &attr->name) == 0 &&
           p_compare_text(&attr2->uri, &attr->uri) == 0) {
          set_err_info_from_attr(env, attr);
          return DUPLICATE_ATTRIBUTES;
        }
      }
    }

    if(env->event.type == SELF_CLOSING_ELEMENT_EVENT) {
      /* Do this to remove the FAXPP_NamespaceInfo objects
         from the namespace stack */
      p_pop_element(env);
    }
    break;
  case END_ELEMENT_EVENT:
    /* [WFC: Element Type Match] */
    if(p_compare_text(&env->element_info_stack->name, &env->event.name) != 0 ||
       p_compare_text(&env->element_info_stack->prefix, &env->event.prefix) != 0) {
      set_err_info_from_event(env);
      err = ELEMENT_NAME_MISMATCH;
    }

    /* Copy the element's URI from the element stack */
    p_set_text_from_text(&env->event.uri, &env->element_info_stack->uri);

    p_pop_element(env);
    break;
  case CHARACTERS_EVENT:
    break;
  case CDATA_EVENT:
    break;
  case IGNORABLE_WHITESPACE_EVENT:
    break;
  case COMMENT_EVENT:
    break;
  case ENTITY_REFERENCE_EVENT:
    /* [WFC: Entity Declared] */
    /* [WFC: Parsed Entity] */
    /* [WFC: No Recursion] */
    // TBD
    if(env->event.value.ptr == 0) {
      set_err_info_from_event(env);
      return UNDEFINED_ENTITY;
    }

    break;
  case DEC_CHAR_REFERENCE_EVENT:
    /* [WFC: Legal Character] */
    ch = p_dec_char_ref_value(&env->event.name, env);
    if((FAXPP_char_flags(ch) & env->tenv.non_restricted_char) == 0) {
      set_err_info_from_event(env);
      return RESTRICTED_CHAR;
    }

    env->event.value.ptr = env->event_buffer.cursor;
    err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, ch);
    env->event.value.len = env->event_buffer.cursor - env->event.value.ptr;

    if(env->null_terminate && err == 0)
      err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, 0);
    if(err != 0) return err;

    break;
  case HEX_CHAR_REFERENCE_EVENT:
    /* [WFC: Legal Character] */
    ch = p_hex_char_ref_value(&env->event.name, env);
    if((FAXPP_char_flags(ch) & env->tenv.non_restricted_char) == 0) {
      set_err_info_from_event(env);
      return RESTRICTED_CHAR;
    }

    env->event.value.ptr = env->event_buffer.cursor;
    err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, ch);
    env->event.value.len = env->event_buffer.cursor - env->event.value.ptr;

    if(env->null_terminate && err == 0)
      err = FAXPP_buffer_append_ch(&env->event_buffer, env->tenv.encode, 0);
    if(err != 0) return err;

    break;
  case NO_EVENT: break;
  }

  return err;
}

