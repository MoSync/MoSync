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

#ifdef DEBUG
const char *FAXPP_state_to_string(FAXPP_StateFunction state)
{
  if(state == initial_state)
    return "initial_state";
  else if(state == initial_misc_state)
    return "initial_misc_state";
  else if(state == initial_markup_state)
    return "initial_markup_state";
  else if(state == end_of_buffer_state)
    return "end_of_buffer_state";
  else if(state == comment_start_state1)
    return "comment_start_state1";
  else if(state == comment_start_state2)
    return "comment_start_state2";
  else if(state == comment_content_state)
    return "comment_content_state";
  else if(state == comment_content_seen_dash_state)
    return "comment_content_seen_dash_state";
  else if(state == comment_content_seen_dash_twice_state)
    return "comment_content_seen_dash_twice_state";
  else if(state == pi_name_start_state)
    return "pi_name_start_state";
  else if(state == pi_name_x_state)
    return "pi_name_x_state";
  else if(state == pi_name_m_state)
    return "pi_name_m_state";
  else if(state == pi_name_l_state)
    return "pi_name_l_state";
  else if(state == pi_name_state)
    return "pi_name_state";
  else if(state == pi_ws_state)
    return "pi_ws_state";
  else if(state == pi_content_state)
    return "pi_content_state";
  else if(state == pi_content_seen_question_state)
    return "pi_content_seen_question_state";

  else if(state == default_start_element_name_state)
    return "default_start_element_name_state";
  else if(state == default_start_element_name_seen_colon_state)
    return "default_start_element_name_seen_colon_state";
  else if(state == default_start_element_name_seen_colon_state2)
    return "default_start_element_name_seen_colon_state2";
  else if(state == default_start_element_mandatory_ws_state)
    return "default_start_element_mandatory_ws_state";
  else if(state == default_start_element_ws_state)
    return "default_start_element_ws_state";
  else if(state == default_element_content_markup_state)
    return "default_element_content_markup_state";
  else if(state == default_attr_name_state)
    return "default_attr_name_state";
  else if(state == default_attr_name_seen_colon_state)
    return "default_attr_name_seen_colon_state";
  else if(state == default_attr_name_seen_colon_state2)
    return "default_attr_name_seen_colon_state2";
  else if(state == default_attr_equals_state)
    return "default_attr_equals_state";
  else if(state == default_attr_value_start_state)
    return "default_attr_value_start_state";
  else if(state == default_attr_value_apos_state)
    return "default_attr_value_apos_state";
  else if(state == default_attr_value_quot_state)
    return "default_attr_value_quot_state";
  else if(state == default_element_content_state)
    return "default_element_content_state";
  else if(state == default_element_content_rsquare_state1)
    return "default_element_content_rsquare_state1";
  else if(state == default_element_content_rsquare_state2)
    return "default_element_content_rsquare_state2";
  else if(state == default_end_element_name_state)
    return "default_end_element_name_state";
  else if(state == default_end_element_name_state2)
    return "default_end_element_name_state2";
  else if(state == default_end_element_name_seen_colon_state)
    return "default_end_element_name_seen_colon_state";
  else if(state == default_end_element_name_seen_colon_state2)
    return "default_end_element_name_seen_colon_state2";
  else if(state == default_ns_name_state1)
    return "default_ns_name_state1";
  else if(state == default_ns_name_state2)
    return "default_ns_name_state2";
  else if(state == default_ns_name_state3)
    return "default_ns_name_state3";
  else if(state == default_ns_name_state4)
    return "default_ns_name_state4";
  else if(state == default_ns_name_state5)
    return "default_ns_name_state5";

  else if(state == utf8_start_element_name_state)
    return "utf8_start_element_name_state";
  else if(state == utf8_start_element_name_seen_colon_state)
    return "utf8_start_element_name_seen_colon_state";
  else if(state == utf8_start_element_name_seen_colon_state2)
    return "utf8_start_element_name_seen_colon_state2";
  else if(state == utf8_start_element_mandatory_ws_state)
    return "utf8_start_element_mandatory_ws_state";
  else if(state == utf8_start_element_ws_state)
    return "utf8_start_element_ws_state";
  else if(state == utf8_attr_name_state)
    return "utf8_attr_name_state";
  else if(state == utf8_attr_name_seen_colon_state)
    return "utf8_attr_name_seen_colon_state";
  else if(state == utf8_attr_name_seen_colon_state2)
    return "utf8_attr_name_seen_colon_state2";
  else if(state == utf8_attr_equals_state)
    return "utf8_attr_equals_state";
  else if(state == utf8_attr_value_start_state)
    return "utf8_attr_value_start_state";
  else if(state == utf8_attr_value_apos_state)
    return "utf8_attr_value_apos_state";
  else if(state == utf8_attr_value_quot_state)
    return "utf8_attr_value_quot_state";
  else if(state == utf8_element_content_state)
    return "utf8_element_content_state";
  else if(state == utf8_element_content_rsquare_state1)
    return "utf8_element_content_rsquare_state1";
  else if(state == utf8_element_content_rsquare_state2)
    return "utf8_element_content_rsquare_state2";
  else if(state == utf8_element_content_markup_state)
    return "utf8_element_content_markup_state";
  else if(state == utf8_end_element_name_state)
    return "utf8_end_element_name_state";
  else if(state == utf8_end_element_name_state2)
    return "utf8_end_element_name_state2";
  else if(state == utf8_end_element_name_seen_colon_state)
    return "utf8_end_element_name_seen_colon_state";
  else if(state == utf8_end_element_name_seen_colon_state2)
    return "utf8_end_element_name_seen_colon_state2";
  else if(state == utf8_ns_name_state1)
    return "utf8_ns_name_state1";
  else if(state == utf8_ns_name_state2)
    return "utf8_ns_name_state2";
  else if(state == utf8_ns_name_state3)
    return "utf8_ns_name_state3";
  else if(state == utf8_ns_name_state4)
    return "utf8_ns_name_state4";
  else if(state == utf8_ns_name_state5)
    return "utf8_ns_name_state5";

  else if(state == utf16_start_element_name_state)
    return "utf16_start_element_name_state";
  else if(state == utf16_start_element_name_seen_colon_state)
    return "utf16_start_element_name_seen_colon_state";
  else if(state == utf16_start_element_name_seen_colon_state2)
    return "utf16_start_element_name_seen_colon_state2";
  else if(state == utf16_start_element_mandatory_ws_state)
    return "utf16_start_element_mandatory_ws_state";
  else if(state == utf16_start_element_ws_state)
    return "utf16_start_element_ws_state";
  else if(state == utf16_attr_name_state)
    return "utf16_attr_name_state";
  else if(state == utf16_attr_name_seen_colon_state)
    return "utf16_attr_name_seen_colon_state";
  else if(state == utf16_attr_name_seen_colon_state2)
    return "utf16_attr_name_seen_colon_state2";
  else if(state == utf16_attr_equals_state)
    return "utf16_attr_equals_state";
  else if(state == utf16_attr_value_start_state)
    return "utf16_attr_value_start_state";
  else if(state == utf16_attr_value_apos_state)
    return "utf16_attr_value_apos_state";
  else if(state == utf16_attr_value_quot_state)
    return "utf16_attr_value_quot_state";
  else if(state == utf16_element_content_state)
    return "utf16_element_content_state";
  else if(state == utf16_element_content_rsquare_state1)
    return "utf16_element_content_rsquare_state1";
  else if(state == utf16_element_content_rsquare_state2)
    return "utf16_element_content_rsquare_state2";
  else if(state == utf16_element_content_markup_state)
    return "utf16_element_content_markup_state";
  else if(state == utf16_end_element_name_state)
    return "utf16_end_element_name_state";
  else if(state == utf16_end_element_name_state2)
    return "utf16_end_element_name_state2";
  else if(state == utf16_end_element_name_seen_colon_state)
    return "utf16_end_element_name_seen_colon_state";
  else if(state == utf16_end_element_name_seen_colon_state2)
    return "utf16_end_element_name_seen_colon_state2";
  else if(state == utf16_ns_name_state1)
    return "utf16_ns_name_state1";
  else if(state == utf16_ns_name_state2)
    return "utf16_ns_name_state2";
  else if(state == utf16_ns_name_state3)
    return "utf16_ns_name_state3";
  else if(state == utf16_ns_name_state4)
    return "utf16_ns_name_state4";
  else if(state == utf16_ns_name_state5)
    return "utf16_ns_name_state5";

  else if(state == start_element_end_state)
    return "start_element_end_state";
  else if(state == self_closing_element_state)
    return "self_closing_element_state";
  else if(state == end_element_ws_state)
    return "end_element_ws_state";
  else if(state == reference_state)
    return "reference_state";
  else if(state == a_entity_reference_state)
    return "a_entity_reference_state";
  else if(state == amp_entity_reference_state1)
    return "amp_entity_reference_state1";
  else if(state == amp_entity_reference_state2)
    return "amp_entity_reference_state2";
  else if(state == apos_entity_reference_state1)
    return "apos_entity_reference_state1";
  else if(state == apos_entity_reference_state2)
    return "apos_entity_reference_state2";
  else if(state == apos_entity_reference_state3)
    return "apos_entity_reference_state3";
  else if(state == gt_entity_reference_state1)
    return "gt_entity_reference_state1";
  else if(state == gt_entity_reference_state2)
    return "gt_entity_reference_state2";
  else if(state == lt_entity_reference_state1)
    return "lt_entity_reference_state1";
  else if(state == lt_entity_reference_state2)
    return "lt_entity_reference_state2";
  else if(state == quot_entity_reference_state1)
    return "quot_entity_reference_state1";
  else if(state == quot_entity_reference_state2)
    return "quot_entity_reference_state2";
  else if(state == quot_entity_reference_state3)
    return "quot_entity_reference_state3";
  else if(state == quot_entity_reference_state4)
    return "quot_entity_reference_state4";
  else if(state == entity_reference_state)
    return "entity_reference_state";
  else if(state == char_reference_state)
    return "char_reference_state";
  else if(state == dec_char_reference_state)
    return "dec_char_reference_state";
  else if(state == hex_char_reference_state1)
    return "hex_char_reference_state1";
  else if(state == hex_char_reference_state2)
    return "hex_char_reference_state2";
  else if(state == cdata_or_comment_state)
    return "cdata_or_comment_state";
  else if(state == cdata_start_state1)
    return "cdata_start_state1";
  else if(state == cdata_start_state2)
    return "cdata_start_state2";
  else if(state == cdata_start_state3)
    return "cdata_start_state3";
  else if(state == cdata_start_state4)
    return "cdata_start_state4";
  else if(state == cdata_start_state5)
    return "cdata_start_state5";
  else if(state == cdata_start_state6)
    return "cdata_start_state6";
  else if(state == cdata_content_state)
    return "cdata_content_state";
  else if(state == cdata_end_state1)
    return "cdata_end_state1";
  else if(state == cdata_end_state2)
    return "cdata_end_state2";
  else if(state == ws_plus_state)
    return "ws_plus_state";
  else if(state == ws_state)
    return "ws_state";
  else if(state == equals_state)
    return "equals_state";
  else if(state == xml_decl_or_markup_state)
    return "xml_decl_or_markup_state";
  else if(state == xml_decl_or_pi_state1)
    return "xml_decl_or_pi_state1";
  else if(state == xml_decl_or_pi_state2)
    return "xml_decl_or_pi_state2";
  else if(state == xml_decl_or_pi_state3)
    return "xml_decl_or_pi_state3";
  else if(state == xml_decl_or_pi_state4)
    return "xml_decl_or_pi_state4";
  else if(state == xml_decl_version_state1)
    return "xml_decl_version_state1";
  else if(state == xml_decl_version_state2)
    return "xml_decl_version_state2";
  else if(state == xml_decl_version_state3)
    return "xml_decl_version_state3";
  else if(state == xml_decl_version_state4)
    return "xml_decl_version_state4";
  else if(state == xml_decl_version_state5)
    return "xml_decl_version_state5";
  else if(state == xml_decl_version_state6)
    return "xml_decl_version_state6";
  else if(state == xml_decl_version_state7)
    return "xml_decl_version_state7";
  else if(state == xml_decl_version_value_state1)
    return "xml_decl_version_value_state1";
  else if(state == xml_decl_version_value_state2)
    return "xml_decl_version_value_state2";
  else if(state == xml_decl_version_value_state3)
    return "xml_decl_version_value_state3";
  else if(state == xml_decl_version_value_state4)
    return "xml_decl_version_value_state4";
  else if(state == xml_decl_version_value_quot_state)
    return "xml_decl_version_value_quot_state";
  else if(state == xml_decl_version_value_apos_state)
    return "xml_decl_version_value_apos_state";
  else if(state == xml_decl_encoding_ws_state)
    return "xml_decl_encoding_ws_state";
  else if(state == xml_decl_encoding_state1)
    return "xml_decl_encoding_state1";
  else if(state == xml_decl_encoding_state2)
    return "xml_decl_encoding_state2";
  else if(state == xml_decl_encoding_state3)
    return "xml_decl_encoding_state3";
  else if(state == xml_decl_encoding_state4)
    return "xml_decl_encoding_state4";
  else if(state == xml_decl_encoding_state5)
    return "xml_decl_encoding_state5";
  else if(state == xml_decl_encoding_state6)
    return "xml_decl_encoding_state6";
  else if(state == xml_decl_encoding_state7)
    return "xml_decl_encoding_state7";
  else if(state == xml_decl_encoding_state8)
    return "xml_decl_encoding_state8";
  else if(state == xml_decl_encoding_value_state)
    return "xml_decl_encoding_value_state";
  else if(state == xml_decl_encoding_value_quot_state1)
    return "xml_decl_encoding_value_quot_state1";
  else if(state == xml_decl_encoding_value_quot_state2)
    return "xml_decl_encoding_value_quot_state2";
  else if(state == xml_decl_encoding_value_apos_state1)
    return "xml_decl_encoding_value_apos_state1";
  else if(state == xml_decl_encoding_value_apos_state2)
    return "xml_decl_encoding_value_apos_state2";
  else if(state == xml_decl_standalone_ws_state)
    return "xml_decl_standalone_ws_state";
  else if(state == xml_decl_seen_question_state)
    return "xml_decl_seen_question_state";
  else if(state == xml_decl_standalone_state1)
    return "xml_decl_standalone_state1";
  else if(state == xml_decl_standalone_state2)
    return "xml_decl_standalone_state2";
  else if(state == xml_decl_standalone_state3)
    return "xml_decl_standalone_state3";
  else if(state == xml_decl_standalone_state4)
    return "xml_decl_standalone_state4";
  else if(state == xml_decl_standalone_state5)
    return "xml_decl_standalone_state5";
  else if(state == xml_decl_standalone_state6)
    return "xml_decl_standalone_state6";
  else if(state == xml_decl_standalone_state7)
    return "xml_decl_standalone_state7";
  else if(state == xml_decl_standalone_state8)
    return "xml_decl_standalone_state8";
  else if(state == xml_decl_standalone_state9)
    return "xml_decl_standalone_state9";
  else if(state == xml_decl_standalone_state10)
    return "xml_decl_standalone_state10";
  else if(state == xml_decl_standalone_value_state1)
    return "xml_decl_standalone_value_state1";
  else if(state == xml_decl_standalone_value_state2)
    return "xml_decl_standalone_value_state2";
  else if(state == xml_decl_standalone_yes_state1)
    return "xml_decl_standalone_yes_state1";
  else if(state == xml_decl_standalone_yes_state2)
    return "xml_decl_standalone_yes_state2";
  else if(state == xml_decl_standalone_no_state)
    return "xml_decl_standalone_no_state";
  else if(state == xml_decl_standalone_value_quot_state)
    return "xml_decl_standalone_value_quot_state";
  else if(state == xml_decl_standalone_value_apos_state)
    return "xml_decl_standalone_value_apos_state";
  else if(state == xml_decl_question_state)
    return "xml_decl_question_state";

  return "unknown";
}
#endif

/*********************
 *
 *  Tokenizer State Functions
 *
 *********************/

FAXPP_Error
ws_plus_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    env->state = ws_state;
    next_char(env);
    break;
  default:
    retrieve_state(env);
    token_start_position(env);
    // don't call next_char()
    return EXPECTING_WHITESPACE;
  }
  return NO_ERROR;
}

FAXPP_Error
ws_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    next_char(env);
    break;
  default:
    retrieve_state(env);
    token_start_position(env);
    // don't call next_char()
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
equals_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  WHITESPACE:
    break;
  case '=':
    env->state = ws_state;
    break;
  default:
    next_char(env);
    return EXPECTING_EQUALS;
  }
  next_char(env);
  return NO_ERROR;  
}

FAXPP_Error
initial_state(FAXPP_TokenizerEnv *env)
{
  if(env->position >= env->buffer_end) {
    if(env->token.value.ptr) {
      token_end_position(env);
      if(env->token.value.len != 0) {
        report_token(IGNORABLE_WHITESPACE_TOKEN, env);
        return NO_ERROR;
      }
    }
    token_start_position(env);
    return PREMATURE_END_OF_BUFFER;
  }

  read_char_no_check(env);

  switch(env->current_char) {
  case '<':
    env->state = xml_decl_or_markup_state;
    token_end_position(env);
    report_token(IGNORABLE_WHITESPACE_TOKEN, env);
    next_char(env);
    break;
  WHITESPACE:
    env->state = initial_misc_state;
    next_char(env);
    break;
  default:
    env->state = initial_misc_state;
    next_char(env);
    return NON_WHITESPACE_OUTSIDE_DOC_ELEMENT;
  }
  return NO_ERROR;
}

FAXPP_Error
initial_misc_state(FAXPP_TokenizerEnv *env)
{
  if(env->position >= env->buffer_end) {
    if(env->token.value.ptr) {
      token_end_position(env);
      if(env->token.value.len != 0) {
        report_token(IGNORABLE_WHITESPACE_TOKEN, env);
        return NO_ERROR;
      }
    }
    token_start_position(env);
    return PREMATURE_END_OF_BUFFER;
  }

  read_char_no_check(env);

  switch(env->current_char) {
  case '<':
    env->state = initial_markup_state;
    token_end_position(env);
    report_token(IGNORABLE_WHITESPACE_TOKEN, env);
    next_char(env);
    break;
  WHITESPACE:
    next_char(env);
    break;
  default:
    next_char(env);
    return NON_WHITESPACE_OUTSIDE_DOC_ELEMENT;
  }
  return NO_ERROR;
}

FAXPP_Error
initial_markup_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '?':
    env->state = pi_name_start_state;
    next_char(env);
    token_start_position(env);
    break;
  case '!':
    env->state = comment_start_state1;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    env->state = (env)->start_element_name_state;
    env->seen_doc_element = 1;
    token_start_position(env);
    next_char(env);
    if((FAXPP_char_flags(env->current_char) & env->ncname_start_char) == 0)
      return INVALID_CHAR_IN_ELEMENT_NAME;
    break;
  }
  return NO_ERROR;
}

FAXPP_Error
final_state(FAXPP_TokenizerEnv *env)
{
  if(env->position >= env->buffer_end) {
    if(env->token.value.ptr) {
      token_end_position(env);
      if(env->token.value.len != 0) {
        report_token(IGNORABLE_WHITESPACE_TOKEN, env);
        if(env->buffer_done)
          env->state = end_of_buffer_state;
        return NO_ERROR;
      }
    }
    if(env->buffer_done) {
      report_empty_token(END_OF_BUFFER_TOKEN, env);
      return NO_ERROR;
    }
    token_start_position(env);
    return PREMATURE_END_OF_BUFFER;
  }

  read_char_no_check(env);

  switch(env->current_char) {
  case '<':
    env->state = final_markup_state;
    token_end_position(env);
    report_token(IGNORABLE_WHITESPACE_TOKEN, env);
    next_char(env);
    break;
  WHITESPACE:
    next_char(env);
    break;
  default:
    next_char(env);
    return NON_WHITESPACE_OUTSIDE_DOC_ELEMENT;
  }
  return NO_ERROR;
}

FAXPP_Error
final_markup_state(FAXPP_TokenizerEnv *env)
{
  read_char(env);

  switch(env->current_char) {
  case '?':
    env->state = pi_name_start_state;
    next_char(env);
    token_start_position(env);
    break;
  case '!':
    env->state = comment_start_state1;
    next_char(env);
    token_start_position(env);
    break;
  LINE_ENDINGS
  default:
    env->state = (env)->start_element_name_state;
    token_start_position(env);
    next_char(env);
    return ADDITIONAL_DOCUMENT_ELEMENT;
  }
  return NO_ERROR;
}

FAXPP_Error
end_of_buffer_state(FAXPP_TokenizerEnv *env)
{
    report_empty_token(END_OF_BUFFER_TOKEN, env);
    return NO_ERROR;
}

// Include the default states

#define PREFIX(name) default_ ## name
#define END_CHECK \
  if((env)->position >= (env)->buffer_end) \
    return PREMATURE_END_OF_BUFFER
#define READ_CHAR read_char_no_check(env)
#define DEFAULT_CASE (void)0

#include "element_states.h"
#include "attr_states.h"

#undef DEFAULT_CASE
#undef READ_CHAR
#undef END_CHECK
#undef PREFIX

// Include the utf8 states

#define PREFIX(name) utf8_ ## name
#define END_CHECK \
  if((env)->position >= (env)->buffer_end) \
    return PREMATURE_END_OF_BUFFER
#define READ_CHAR \
  /* Assume it's a one byte character for now */ \
  env->current_char = *(uint8_t*)env->position; \
  env->char_len = 1
#define DEFAULT_CASE \
{ \
  /* Check if it really was a one byte char */ \
  if(env->current_char >= 0x80) { \
    /* Decode properly */ \
    env->char_len = FAXPP_utf8_decode(env->position, env->buffer_end, &env->current_char); \
    switch((env)->char_len) { \
    case TRANSCODE_PREMATURE_END_OF_BUFFER: \
      return PREMATURE_END_OF_BUFFER; \
    case TRANSCODE_BAD_ENCODING: \
      return BAD_ENCODING; \
    } \
  } \
}

#include "element_states.h"
#include "attr_states.h"

#undef DEFAULT_CASE
#undef READ_CHAR
#undef END_CHECK
#undef PREFIX

// Include the utf16 states

#define PREFIX(name) utf16_ ## name
#define END_CHECK \
  if((env)->position + 1 >= (env)->buffer_end) \
    return PREMATURE_END_OF_BUFFER
#define READ_CHAR \
  /* Assume it's not a surrogate pair for now */ \
  env->current_char = *(uint16_t*)env->position; \
  env->char_len = 1 * sizeof(uint16_t)
#define DEFAULT_CASE \
{ \
  /* Check if it was actually a surrogate pair */ \
  if(env->current_char >= 0xD800 && env->current_char <= 0xDF00) { \
    /* Decode properly */ \
    env->char_len = FAXPP_utf16_native_decode(env->position, env->buffer_end, &env->current_char); \
    switch((env)->char_len) { \
    case TRANSCODE_PREMATURE_END_OF_BUFFER: \
      return PREMATURE_END_OF_BUFFER; \
    case TRANSCODE_BAD_ENCODING: \
      return BAD_ENCODING; \
    } \
  } \
}

#include "element_states.h"
#include "attr_states.h"

#undef DEFAULT_CASE
#undef READ_CHAR
#undef END_CHECK
#undef PREFIX

