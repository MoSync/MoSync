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

#include "tokenizer_states.h"
#include "char_classes.h"

extern uint8_t FAXPP_utf_8_bytes[256];

FAXPP_Error
start_element_end_state(FAXPP_TokenizerEnv *env)
{
  env->state = (env)->element_content_state;
  env->nesting_level += 1;
  report_empty_token(START_ELEMENT_END_TOKEN, env);
  // no next_char
  return 0;
}

FAXPP_Error
self_closing_element_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '>':
    base_state(env);
    token_end_position(env);
    report_empty_token(SELF_CLOSING_ELEMENT_TOKEN, env);
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    next_char(env);
    return INVALID_CHAR_IN_START_ELEMENT;
  }
  return NO_ERROR;  
}

FAXPP_Error
default_element_content_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    if(env->position >= env->buffer_end) {
      if(env->token.value.ptr) {
        token_end_position(env);
        if(env->token.value.len != 0) {
          report_token(CHARACTERS_TOKEN, env);
          return NO_ERROR;
        }
      }
      token_start_position(env);
      return PREMATURE_END_OF_BUFFER;
    }

    read_char_no_check(env);

    switch(env->current_char) {
    case '<':
      env->state = default_element_content_markup_state;
      token_end_position(env);
      report_token(CHARACTERS_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '&':
      store_state(env);
      env->state = reference_state;
      token_end_position(env);
      report_token(CHARACTERS_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    case ']':
      env->state = default_element_content_rsquare_state1;
      next_char(env);
      return NO_ERROR;
    LINE_ENDINGS
      break;
    default:
      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0) {
        next_char(env);
        return RESTRICTED_CHAR;
      }
      break;
    }

    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
utf8_element_content_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    if(env->position >= env->buffer_end) {
      if(env->token.value.ptr) {
        token_end_position(env);
        if(env->token.value.len != 0) {
          report_token(CHARACTERS_TOKEN, env);
          return NO_ERROR;
        }
      }
      token_start_position(env);
      return PREMATURE_END_OF_BUFFER;
    }

    // Assume it's a one byte character for now
    env->current_char = *(uint8_t*)env->position;
    env->char_len = 1;

    switch(env->current_char) {
    case '<':
      env->state = utf8_element_content_markup_state;
      token_end_position(env);
      report_token(CHARACTERS_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '&':
      store_state(env);
      env->state = reference_state;
      token_end_position(env);
      report_token(CHARACTERS_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    case ']':
      env->state = utf8_element_content_rsquare_state1;
      next_char(env);
      return NO_ERROR;
    LINE_ENDINGS
      break;

    // 0x0A, 0x0D, 0x26, 0x3C, 0x5D - Done above

    case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
    case 0x08:                       case 0x0B: case 0x0C:            case 0x0E: case 0x0F:
    case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: case 0x1F:
    case 0x7F:
      next_char(env);
      return RESTRICTED_CHAR;
    case 0x09:
    case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25:            case 0x27:
    case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F:
    case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
    case 0x38: case 0x39: case 0x3A: case 0x3B:            case 0x3D: case 0x3E: case 0x3F:
    case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
    case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
    case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C:            case 0x5E: case 0x5F:
    case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
    case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
    case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E:
      // The char is a valid one byte char
      break;

    default:
      // Decode properly
      env->char_len = FAXPP_utf8_decode(env->position, env->buffer_end, &env->current_char);
      switch((env)->char_len) {
      case TRANSCODE_PREMATURE_END_OF_BUFFER:
        return PREMATURE_END_OF_BUFFER;
      case TRANSCODE_BAD_ENCODING:
        return BAD_ENCODING;
      }

      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0) {
        next_char(env);
        return RESTRICTED_CHAR;
      }
      break;
    }

    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
utf16_element_content_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    if(env->position + 1 >= env->buffer_end) {
      if(env->token.value.ptr) {
        token_end_position(env);
        if(env->token.value.len != 0) {
          report_token(CHARACTERS_TOKEN, env);
          return NO_ERROR;
        }
      }
      token_start_position(env);
      return PREMATURE_END_OF_BUFFER;
    }

    // Assume it's not a surrogate pair for now
    env->current_char = *(uint16_t*)env->position;
    env->char_len = 1 * sizeof(uint16_t);

    switch(env->current_char) {
    case '<':
      env->state = utf16_element_content_markup_state;
      token_end_position(env);
      report_token(CHARACTERS_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '&':
      store_state(env);
      env->state = reference_state;
      token_end_position(env);
      report_token(CHARACTERS_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    case ']':
      env->state = utf16_element_content_rsquare_state1;
      next_char(env);
      return NO_ERROR;
    LINE_ENDINGS
      break;
    default:
      // Check if it was actually a surrogate pair
      if(env->current_char >= 0xD800 && env->current_char <= 0xDF00) {
        // Decode properly
        env->char_len = FAXPP_utf16_native_decode(env->position, env->buffer_end, &env->current_char);
        switch((env)->char_len) {
        case TRANSCODE_PREMATURE_END_OF_BUFFER:
          return PREMATURE_END_OF_BUFFER;
        case TRANSCODE_BAD_ENCODING:
          return BAD_ENCODING;
        }
      }

      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0) {
        next_char(env);
        return RESTRICTED_CHAR;
      }
      break;
    }

    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
end_element_ws_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  case '>':
    env->nesting_level -= 1;
    base_state(env);
    next_char(env);
    token_start_position(env);
    break;
  default:
    next_char(env);
    return INVALID_CHAR_IN_END_ELEMENT;
  }
  return NO_ERROR;  
}

