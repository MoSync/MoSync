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

// This file needs to have a number of macros defined before it is included

FAXPP_Error
PREFIX(start_element_name_state)(FAXPP_TokenizerEnv *env)
{
  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      env->state = PREFIX(start_element_ws_state);
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '/':
      env->state = self_closing_element_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->state = start_element_end_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    case ':':
      env->state = PREFIX(start_element_name_seen_colon_state);
      token_end_position(env);
      report_token(START_ELEMENT_PREFIX_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0) {
      return INVALID_CHAR_IN_ELEMENT_NAME;
    }
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
PREFIX(start_element_name_seen_colon_state)(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default: 
    env->state = PREFIX(start_element_name_seen_colon_state2);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
PREFIX(start_element_name_seen_colon_state2)(FAXPP_TokenizerEnv *env)
{
  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      env->state = PREFIX(start_element_ws_state);
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '/':
      env->state = self_closing_element_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->state = start_element_end_state;
      token_end_position(env);
      report_token(START_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

FAXPP_Error
PREFIX(start_element_mandatory_ws_state)(FAXPP_TokenizerEnv *env)
{
  END_CHECK;

  READ_CHAR;

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    // Fall through
  case '/':
  case '>':
    env->state = PREFIX(start_element_ws_state);
    break;
  default:
    env->state = PREFIX(start_element_ws_state);
    return EXPECTING_WHITESPACE;
  }
  return NO_ERROR;  
}

FAXPP_Error
PREFIX(start_element_ws_state)(FAXPP_TokenizerEnv *env)
{
  END_CHECK;

  READ_CHAR;

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  case '/':
    env->state = self_closing_element_state;
    next_char(env);
    break;
  case '>':
    env->state = env->element_content_state;
    env->nesting_level += 1;
    report_empty_token(START_ELEMENT_END_TOKEN, env);
    next_char(env);
    token_start_position(env);
    break;
  case 'x':
    env->state = PREFIX(ns_name_state1);
    token_start_position(env);
    next_char(env);
    break;
  default:
    DEFAULT_CASE;

    env->state = PREFIX(attr_name_state);
    token_start_position(env);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ATTRIBUTE_NAME;
    break;
  }
  return NO_ERROR;  
}

FAXPP_Error
PREFIX(element_content_markup_state)(FAXPP_TokenizerEnv *env)
{
  END_CHECK;

  READ_CHAR;

  switch(env->current_char) {
  case '?':
    env->state = pi_name_start_state;
    next_char(env);
    token_start_position(env);
    break;
  case '!':
    env->state = cdata_or_comment_state;
    next_char(env);
    token_start_position(env);
    break;
  case '/':
    env->state = PREFIX(end_element_name_state);
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    DEFAULT_CASE;

    env->state = (env)->start_element_name_state;
    token_start_position(env);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
PREFIX(element_content_rsquare_state1)(FAXPP_TokenizerEnv *env)
{
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

  READ_CHAR;

  switch(env->current_char) {
  case ']':
    env->state = PREFIX(element_content_rsquare_state2);
    next_char(env);
    break;
  default:
    env->state = PREFIX(element_content_state);
    break;
  }

  return NO_ERROR;
}

FAXPP_Error
PREFIX(element_content_rsquare_state2)(FAXPP_TokenizerEnv *env)
{
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

  READ_CHAR;

  switch(env->current_char) {
  case '>':
    env->state = PREFIX(element_content_state);
    next_char(env);
    return CDATA_END_IN_ELEMENT_CONTENT;;
  case ']':
    next_char(env);
    break;
  default:
    env->state = PREFIX(element_content_state);
    break;;
  }

  return NO_ERROR;
}

FAXPP_Error
PREFIX(end_element_name_state)(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = PREFIX(end_element_name_state2);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
  }
  return NO_ERROR;  
}

FAXPP_Error
PREFIX(end_element_name_state2)(FAXPP_TokenizerEnv *env)
{
  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      env->state = end_element_ws_state;
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->nesting_level -= 1;
      base_state(env);
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    case ':':
      env->state = PREFIX(end_element_name_seen_colon_state);
      token_end_position(env);
      report_token(END_ELEMENT_PREFIX_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      DEFAULT_CASE;
      break;
    }

    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0) {
      next_char(env);
      return INVALID_CHAR_IN_ELEMENT_NAME;
    }

    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
PREFIX(end_element_name_seen_colon_state)(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  LINE_ENDINGS
  default:
    env->state = PREFIX(end_element_name_seen_colon_state2);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
    break;
  }

  return NO_ERROR;  
}

FAXPP_Error
PREFIX(end_element_name_seen_colon_state2)(FAXPP_TokenizerEnv *env)
{
  while(1) {
    END_CHECK;

    READ_CHAR;

    switch(env->current_char) {
    WHITESPACE:
      env->state = end_element_ws_state;
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '>':
      env->nesting_level -= 1;
      base_state(env);
      token_end_position(env);
      report_token(END_ELEMENT_NAME_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    default:
      DEFAULT_CASE;
      break;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
  }

  // Never happens
  return NO_ERROR;  
}

