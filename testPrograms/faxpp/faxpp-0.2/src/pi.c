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

FAXPP_Error
pi_name_start_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'x':
  case 'X':
    env->state = pi_name_x_state;
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = pi_name_state;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0) {
      return INVALID_CHAR_IN_PI_NAME;
    }
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
pi_name_x_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'm':
  case 'M':
    env->state = pi_name_m_state;
    next_char(env);
    break;
  default:
    env->state = pi_name_state;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
pi_name_m_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'l':
  case 'L':
    env->state = pi_name_l_state;
    next_char(env);
    break;
  default:
    env->state = pi_name_state;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
pi_name_l_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
  case '?':
    env->state = pi_name_state;
    return INVALID_PI_NAME_OF_XML;
  default:
    env->state = pi_name_state;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
pi_name_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    WHITESPACE:
      env->state = pi_ws_state;
      token_end_position(env);
      report_token(PI_NAME_TOKEN, env);
      next_char(env);
      return NO_ERROR;
    case '?':
      env->state = pi_content_state;
      token_end_position(env);
      report_token(PI_NAME_TOKEN, env);
      token_start_position(env);
      return NO_ERROR;
    default:
      if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0) {
        next_char(env);
        return INVALID_CHAR_IN_PI_NAME;
      }
    }

    next_char(env);
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
pi_ws_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  default:
    env->state = pi_content_state;
    token_start_position(env);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0)
      return RESTRICTED_CHAR;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
pi_content_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case '?':
      env->state = pi_content_seen_question_state;
      env->token_position1 = env->token_buffer.cursor ? env->token_buffer.cursor : env->position;
      next_char(env);
      return NO_ERROR;
    LINE_ENDINGS
    default:
      next_char(env);
      if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0)
        return RESTRICTED_CHAR;
      break;
    }
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
pi_content_seen_question_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '>':
    base_state(env);
    env->token_buffer.cursor = 0;
    env->token.value.len = env->token_position1 - env->token.value.ptr;
    report_token_maybe_empty(PI_VALUE_TOKEN, env);
    next_char(env);
    token_start_position(env);
    break;
  case '?':
    env->token_position1 = env->token_buffer.cursor ? env->token_buffer.cursor : env->position;
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = pi_content_state;
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->non_restricted_char) == 0)
      return RESTRICTED_CHAR;
    break;
  }
  return NO_ERROR;
}

