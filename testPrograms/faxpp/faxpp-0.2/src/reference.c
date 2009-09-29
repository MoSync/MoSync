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
reference_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '#':
    env->state = char_reference_state;
    next_char(env);
    token_start_position(env);
    break;
  case 'a':
    env->state = a_entity_reference_state;
    token_start_position(env);
    next_char(env);
    break;
  case 'g':
    env->state = gt_entity_reference_state1;
    token_start_position(env);
    next_char(env);
    break;
  case 'l':
    env->state = lt_entity_reference_state1;
    token_start_position(env);
    next_char(env);
    break;
  case 'q':
    env->state = quot_entity_reference_state1;
    token_start_position(env);
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = entity_reference_state;
    token_start_position(env);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ENTITY_REFERENCE;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
a_entity_reference_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'm':
    env->state = amp_entity_reference_state1;
    break;
  case 'p':
    env->state = apos_entity_reference_state1;
    break;
  default:
    env->state = entity_reference_state;
    return NO_ERROR;
  }

  next_char(env);
  return NO_ERROR;
}

FAXPP_Error
amp_entity_reference_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'p':
    env->state = amp_entity_reference_state2;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
amp_entity_reference_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ';':
    retrieve_state(env);
    token_end_position(env);
    report_token(AMP_ENTITY_REFERENCE_TOKEN, env);
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
apos_entity_reference_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'o':
    env->state = apos_entity_reference_state2;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
apos_entity_reference_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 's':
    env->state = apos_entity_reference_state3;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
apos_entity_reference_state3(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ';':
    retrieve_state(env);
    token_end_position(env);
    report_token(APOS_ENTITY_REFERENCE_TOKEN, env);
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
gt_entity_reference_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 't':
    env->state = gt_entity_reference_state2;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
gt_entity_reference_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ';':
    retrieve_state(env);
    token_end_position(env);
    report_token(GT_ENTITY_REFERENCE_TOKEN, env);
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
lt_entity_reference_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 't':
    env->state = lt_entity_reference_state2;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
lt_entity_reference_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ';':
    retrieve_state(env);
    token_end_position(env);
    report_token(LT_ENTITY_REFERENCE_TOKEN, env);
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
quot_entity_reference_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'u':
    env->state = quot_entity_reference_state2;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
quot_entity_reference_state2(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'o':
    env->state = quot_entity_reference_state3;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
quot_entity_reference_state3(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 't':
    env->state = quot_entity_reference_state4;
    next_char(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
quot_entity_reference_state4(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case ';':
    retrieve_state(env);
    token_end_position(env);
    report_token(QUOT_ENTITY_REFERENCE_TOKEN, env);
    next_char(env);
    token_start_position(env);
    return NO_ERROR;
  }

  env->state = entity_reference_state;
  return NO_ERROR;
}

FAXPP_Error
entity_reference_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    LINE_ENDINGS
      break;
    case ';':
      retrieve_state(env);
      token_end_position(env);
      report_token(ENTITY_REFERENCE_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    }

    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_char) == 0)
      return INVALID_CHAR_IN_ENTITY_REFERENCE;
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
char_reference_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case 'x':
    env->state = hex_char_reference_state1;
    next_char(env);
    token_start_position(env);
    break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    env->state = dec_char_reference_state;
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = dec_char_reference_state;
    next_char(env);
    return INVALID_CHAR_IN_CHAR_REFERENCE;
  }
  return NO_ERROR;
}

FAXPP_Error
dec_char_reference_state(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case ';':
      retrieve_state(env);
      token_end_position(env);
      report_token(DEC_CHAR_REFERENCE_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      next_char(env);
      break;
    LINE_ENDINGS
    default:
      next_char(env);
      return INVALID_CHAR_IN_CHAR_REFERENCE;
    }
  }

  // Never happens
  return NO_ERROR;
}

FAXPP_Error
hex_char_reference_state1(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
    env->state = hex_char_reference_state2;
    next_char(env);
    break;
  LINE_ENDINGS
  default:
    env->state = hex_char_reference_state2;
    next_char(env);
    return INVALID_CHAR_IN_CHAR_REFERENCE;
  }
  return NO_ERROR;
}

FAXPP_Error
hex_char_reference_state2(FAXPP_TokenizerEnv *env)
{
  while(1) {
    read_char(env);

    switch(env->current_char) {
    case ';':
      retrieve_state(env);
      token_end_position(env);
      report_token(HEX_CHAR_REFERENCE_TOKEN, env);
      next_char(env);
      token_start_position(env);
      return NO_ERROR;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      next_char(env);
      break;
    LINE_ENDINGS
    default:
      next_char(env);
      return INVALID_CHAR_IN_CHAR_REFERENCE;
    }
  }

  // Never happens
  return NO_ERROR;
}

