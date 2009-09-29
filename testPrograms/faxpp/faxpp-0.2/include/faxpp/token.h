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

#ifndef __FAXPP__TOKEN_H
#define __FAXPP__TOKEN_H

#include "text.h"

/// An enumeration of the token types returned by the tokenizer
typedef enum {
  NO_TOKEN  = 0,

  XML_DECL_VERSION_TOKEN,
  XML_DECL_ENCODING_TOKEN,
  XML_DECL_STANDALONE_TOKEN,
  XML_DECL_END_TOKEN,

  START_ELEMENT_PREFIX_TOKEN,
  START_ELEMENT_NAME_TOKEN,
  START_ELEMENT_END_TOKEN,
  SELF_CLOSING_ELEMENT_TOKEN,

  XMLNS_PREFIX_TOKEN,
  XMLNS_NAME_TOKEN,

  XML_PREFIX_TOKEN,

  ATTRIBUTE_PREFIX_TOKEN,
  ATTRIBUTE_NAME_TOKEN,
  ATTRIBUTE_VALUE_TOKEN,

  END_ELEMENT_PREFIX_TOKEN,
  END_ELEMENT_NAME_TOKEN,

  CHARACTERS_TOKEN,
  CDATA_TOKEN,
  IGNORABLE_WHITESPACE_TOKEN,

  COMMENT_TOKEN,

  PI_NAME_TOKEN,
  PI_VALUE_TOKEN,

  AMP_ENTITY_REFERENCE_TOKEN,
  APOS_ENTITY_REFERENCE_TOKEN,
  GT_ENTITY_REFERENCE_TOKEN,
  LT_ENTITY_REFERENCE_TOKEN,
  QUOT_ENTITY_REFERENCE_TOKEN,

  ENTITY_REFERENCE_TOKEN,
  DEC_CHAR_REFERENCE_TOKEN,
  HEX_CHAR_REFERENCE_TOKEN,

  END_OF_BUFFER_TOKEN = 99
} FAXPP_TokenType;

/// A structure containing token information, returned by the tokenizer
typedef struct {
  FAXPP_TokenType type; ///< The type of the token
  FAXPP_Text value;     ///< The string value of the token (might be zero length)
  unsigned int line;    ///< The line number of the start of the token
  unsigned int column;  ///< The column number of the start of the token
} FAXPP_Token;

/**
 * Returns a string describing the given token type
 * \param type The token type
 * \return a string
 *
 * \relatesalso FAXPP_Token
 */
const char *FAXPP_token_to_string(const FAXPP_TokenType type);

#endif
