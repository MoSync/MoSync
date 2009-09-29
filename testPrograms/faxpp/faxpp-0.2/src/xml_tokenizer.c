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

#include "xml_tokenizer.h"
#include "tokenizer_states.h"
#include "char_classes.h"
#include "config.h"
#include <faxpp/token.h>

/*********************
 *
 *  Tokenizer Functions
 *
 *********************/

#define INITIAL_TOKEN_BUFFER_SIZE 64

FAXPP_Error
sniff_encoding(FAXPP_TokenizerEnv *env)
{
  // Make initial judgement on the encoding
  unsigned char *buf = (unsigned char*)env->position;

/*   printf("First bytes: %02X %02X %02X %02X\n", *buf, *(buf + 1), */
/*          *(buf + 2), *(buf + 3)); */

  switch(*buf++) {
  case 0x00:
    switch(*buf++) {
    case 0x00:
      switch(*buf++) {
      case 0x00:
        switch(*buf) {
        case 0x3C:
          /* 00 00 00 3C  UCS-4, big-endian machine (1234 order) */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_native_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_be_decode);
#endif
          break;
        }
        break;
      case 0x3C:
        switch(*buf) {
        case 0x00:
          /* 00 00 3C 00  UCS-4, unusual octet order (2143) */
          return UNSUPPORTED_ENCODING;
        }
        break;
      case 0xFE:
        switch(*buf) {
        case 0xFF:
          /* 00 00 FE FF  UCS-4, big-endian machine (1234 order) */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_native_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_be_decode);
#endif
          // Skip BOM
          env->position += 4;
          break;
        }
        break;
      case 0xFF:
        switch(*buf) {
        case 0xFE:
          /* 00 00 FF FE  UCS-4, unusual octet order (2143) */
          return UNSUPPORTED_ENCODING;
        }
        break;
      }
      break;
    case 0x3C:
      switch(*buf++) {
      case 0x00:
        switch(*buf) {
        case 0x00:
          /* 00 3C 00 00  UCS-4, unusual octet order (3412) */
          return UNSUPPORTED_ENCODING;
        case 0x3F:
          /* 00 3C 00 3F  UTF-16, big-endian */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_native_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_be_decode);
#endif
          break;
        }
        break;
      }
      break;
    }
    break;
  case 0x3C:
    switch(*buf++) {
    case 0x00:
      switch(*buf++) {
      case 0x00:
        switch(*buf) {
        case 0x00:
          /* 3C 00 00 00  UCS-4, little-endian machine (4321 order) */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_le_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_native_decode);
#endif
          break;
        }
        break;
      case 0x3F:
        switch(*buf) {
        case 0x00:
          /* 3C 00 3F 00  UTF-16, little-endian */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_le_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_native_decode);
#endif
          break;
        }
        break;
      }
      break;
    case 0x3F:
      switch(*buf++) {
      case 0x78:
        switch(*buf) {
        case 0x6D:
          /* 3C 3F 78 6D  UTF-8, ISO 646, ASCII, some part of ISO 8859, Shift-JIS, EUC, etc. */
          FAXPP_set_tokenizer_decode(env, FAXPP_utf8_decode);
          break;
        }
        break;
      }
      break;
    }
    break;
  case 0x4C:
    switch(*buf++) {
    case 0x6F:
      switch(*buf++) {
      case 0xA7:
        switch(*buf) {
        case 0x94:
          /* 4C 6F A7 94 EBCDIC */
          return UNSUPPORTED_ENCODING;
        }
        break;
      }
      break;
    }
    break;
  case 0xEF:
    switch(*buf++) {
    case 0xBB:
      switch(*buf++) {
      case 0xBF:
        /* EF BB BF  UTF-8 with byte order mark */
        FAXPP_set_tokenizer_decode(env, FAXPP_utf8_decode);
        // Skip BOM
        env->position += 3;
      }
      break;
    }
    break;
  case 0xFE:
    switch(*buf++) {
    case 0xFF:
      switch(*buf++) {
      case 0x00:
        switch(*buf) {
        case 0x00:
          /* FE FF 00 00  UCS-4, unusual octet order (3412) */
          return UNSUPPORTED_ENCODING;
        default:
          /* FE FF ## ##  UTF-16, big-endian */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_native_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_be_decode);
#endif
          // Skip BOM
          env->position += 2;
          break;
        }
        break;
      default:
        /* FE FF ## ##  UTF-16, big-endian */
#ifdef WORDS_BIGENDIAN
        FAXPP_set_tokenizer_decode(env, FAXPP_utf16_native_decode);
#else
        FAXPP_set_tokenizer_decode(env, FAXPP_utf16_be_decode);
#endif
        // Skip BOM
        env->position += 2;
        break;
      }
      break;
    }
    break;
  case 0xFF:
    switch(*buf++) {
    case 0xFE:
      switch(*buf++) {
      case 0x00:
        switch(*buf) {
        case 0x00:
          /* FF FE 00 00  UCS-4, little-endian machine (4321 order) */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_le_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_ucs4_native_decode);
#endif
          // Skip BOM
          env->position += 4;
          break;
        default:
          /* FF FE ## ##  UTF-16, little-endian */
#ifdef WORDS_BIGENDIAN
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_le_decode);
#else
          FAXPP_set_tokenizer_decode(env, FAXPP_utf16_native_decode);
#endif
          // Skip BOM
          env->position += 2;
          break;
        }
        break;
      default:
        /* FF FE ## ##  UTF-16, little-endian */
#ifdef WORDS_BIGENDIAN
        FAXPP_set_tokenizer_decode(env, FAXPP_utf16_le_decode);
#else
        FAXPP_set_tokenizer_decode(env, FAXPP_utf16_native_decode);
#endif
        // Skip BOM
        env->position += 2;
        break;
      }
      break;
    }
    break;
  }

  if(env->decode == 0) {
    // Default encoding is UTF-8
    FAXPP_set_tokenizer_decode(env, FAXPP_utf8_decode);
  }
  return NO_ERROR;
}

FAXPP_DecodeFunction
FAXPP_get_tokenizer_decode(const FAXPP_Tokenizer *tokenizer)
{
  return tokenizer->decode;
}

void
FAXPP_set_tokenizer_decode(FAXPP_Tokenizer *tokenizer, FAXPP_DecodeFunction decode)
{
  if(decode == FAXPP_utf16_native_decode ||
#ifdef WORDS_BIGENDIAN
     decode == FAXPP_utf16_be_decode
#else
     decode == FAXPP_utf16_le_decode
#endif
     ) {
    tokenizer->decode = FAXPP_utf16_native_decode;

    if(tokenizer->encode == FAXPP_utf16_native_encode)
      tokenizer->do_encode = 0;

    tokenizer->start_element_name_state = utf16_start_element_name_state;
    tokenizer->element_content_state = utf16_element_content_state;
  }
  else if(decode == FAXPP_utf8_decode) {
    tokenizer->decode = FAXPP_utf8_decode;

    if(tokenizer->encode == FAXPP_utf8_encode)
      tokenizer->do_encode = 0;

    tokenizer->start_element_name_state = utf8_start_element_name_state;
    tokenizer->element_content_state = utf8_element_content_state;
  }
  else if(decode == FAXPP_ucs4_native_decode ||
#ifdef WORDS_BIGENDIAN
     decode == FAXPP_ucs4_be_decode
#else
     decode == FAXPP_ucs4_le_decode
#endif
     ) {
    tokenizer->decode = FAXPP_ucs4_native_decode;

    tokenizer->start_element_name_state = default_start_element_name_state;
    tokenizer->element_content_state = default_element_content_state;
  }
  else {
    tokenizer->decode = decode;

    tokenizer->start_element_name_state = default_start_element_name_state;
    tokenizer->element_content_state = default_element_content_state;
  }
}

void change_token_buffer(void *userData, FAXPP_Buffer *buffer, void *newBuffer)
{
  FAXPP_TokenizerEnv *env = (FAXPP_TokenizerEnv*)userData;

  env->token_position1 += newBuffer - buffer->buffer;
  env->token_position2 += newBuffer - buffer->buffer;

  env->token.value.ptr = newBuffer;
}

FAXPP_Error
init_tokenizer_internal(FAXPP_TokenizerEnv *env, FAXPP_EncodeFunction encode)
{
  memset(env, 0, sizeof(FAXPP_TokenizerEnv));
  env->encode = encode;
  return FAXPP_init_buffer(&env->token_buffer, INITIAL_TOKEN_BUFFER_SIZE, change_token_buffer, env);
}

void
free_tokenizer_internal(FAXPP_TokenizerEnv *env)
{
  FAXPP_free_buffer(&env->token_buffer);
}

FAXPP_Tokenizer *
FAXPP_create_tokenizer(FAXPP_EncodeFunction encode)
{
  FAXPP_TokenizerEnv *result = malloc(sizeof(FAXPP_TokenizerEnv));
  if(result == 0) return 0;

  if(init_tokenizer_internal(result, encode) == OUT_OF_MEMORY) {
    free(result);
    return 0;
  }

  return result;
}

void
FAXPP_free_tokenizer(FAXPP_Tokenizer *tokenizer)
{
  FAXPP_free_buffer(&tokenizer->token_buffer);
  free(tokenizer);
}

FAXPP_Error
FAXPP_init_tokenize(FAXPP_Tokenizer *env, void *buffer, unsigned int length, unsigned int done)
{
  env->buffer = buffer;
  env->buffer_end = buffer + length;

  env->position = buffer;
  env->current_char = 0;
  env->char_len = 0;

  env->line = 1;
  env->column = 0;

  env->nesting_level = 0;
  env->do_encode = 1;
  env->seen_doc_element = 0;
  env->buffer_done = done;

  env->decode = 0;

  env->token_buffer.cursor = 0;

  env->token_position1 = 0;
  env->token_position2 = 0;

  env->state = initial_state;
  env->stored_state = 0;

  env->start_element_name_state = default_start_element_name_state;
  env->element_content_state = default_element_content_state;

  env->ncname_start_char = NCNAME_START_CHAR10;
  env->ncname_char = NCNAME_CHAR10;
  env->non_restricted_char = NON_RESTRICTED_CHAR10;

  FAXPP_Error err = sniff_encoding(env);
  if(err) return err;

  token_start_position(env);

  return NO_ERROR;
}

FAXPP_Error
FAXPP_tokenizer_release_buffer(FAXPP_Tokenizer *tokenizer, void **buffer_position)
{
  if(buffer_position) *buffer_position = tokenizer->position;

  // Check if the partial token in the tokenizer needs copying to the token_buffer
  if(tokenizer->token.value.ptr >= tokenizer->buffer &&
     tokenizer->token.value.ptr < tokenizer->buffer_end) {
    void *token_start = tokenizer->token.value.ptr;

    // Find the length of the partial token
    unsigned int token_length = tokenizer->token.value.len;
    if(!token_length)
      token_length = tokenizer->position - tokenizer->token.value.ptr;

    // Re-position the token positions to point into the token_buffer
    FAXPP_reset_buffer(&tokenizer->token_buffer);
    tokenizer->token_position1 += tokenizer->token_buffer.cursor - token_start;
    tokenizer->token_position2 += tokenizer->token_buffer.cursor - token_start;
    tokenizer->token.value.ptr = tokenizer->token_buffer.cursor;
    tokenizer->token.value.len = token_length;

    return FAXPP_buffer_append(&tokenizer->token_buffer, token_start, token_length);
  }

  return NO_ERROR;
}

FAXPP_Error
FAXPP_continue_tokenize(FAXPP_Tokenizer *env, void *buffer, unsigned int length, unsigned int done)
{
  if(env->token.value.ptr == env->buffer_end)
    env->token.value.ptr = buffer;

  env->buffer = buffer;
  env->buffer_end = buffer + length;
  env->buffer_done = done;

  env->position = buffer;

  return NO_ERROR;
}

FAXPP_Error
FAXPP_next_token(FAXPP_Tokenizer *env)
{
  env->result_token.type = NO_TOKEN;

  FAXPP_Error err = 0;
  while(err == NO_ERROR && env->result_token.type == NO_TOKEN) {
    err = env->state(env);
  }

  return err;
}

const FAXPP_Token *
FAXPP_get_current_token(const FAXPP_Tokenizer *tokenizer)
{
  return &tokenizer->result_token;
}

unsigned int
FAXPP_get_tokenizer_nesting_level(const FAXPP_Tokenizer *tokenizer)
{
  return tokenizer->nesting_level;
}

unsigned int
FAXPP_get_tokenizer_error_line(const FAXPP_Tokenizer *tokenizer)
{
  return tokenizer->line;
}

unsigned int
FAXPP_get_tokenizer_error_column(const FAXPP_Tokenizer *tokenizer)
{
  return tokenizer->column;
}
