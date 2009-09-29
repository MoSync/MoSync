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

/**
 * Error codes
 *
 * By John Snelson
 *
 */

#include <faxpp/error.h>

const char *
FAXPP_err_to_string(FAXPP_Error err)
{
  switch(err) {
  case DOUBLE_DASH_IN_COMMENT:
    return "DOUBLE_DASH_IN_COMMENT";
  case PREMATURE_END_OF_BUFFER:
    return "PREMATURE_END_OF_BUFFER";
  case INVALID_START_OF_COMMENT:
    return "INVALID_START_OF_COMMENT";
  case INVALID_CHAR_IN_START_ELEMENT:
    return "FORWARD_SLASH_IN_START_ELEMENT";
  case INVALID_CHAR_IN_ATTRIBUTE:
    return "INVALID_CHAR_IN_ATTRIBUTE";
  case INVALID_CHAR_IN_END_ELEMENT:
    return "INVALID_CHAR_IN_END_ELEMENT";
  case NON_WHITESPACE_OUTSIDE_DOC_ELEMENT:
    return "NON_WHITESPACE_OUTSIDE_DOC_ELEMENT";
  case BAD_ENCODING:
    return "BAD_ENCODING";
  case UNSUPPORTED_ENCODING:
    return "UNSUPPORTED_ENCODING";
  case ADDITIONAL_DOCUMENT_ELEMENT:
    return "ADDITIONAL_DOCUMENT_ELEMENT";
  case INVALID_CHAR_IN_PI_NAME:
    return "INVALID_CHAR_IN_PI_NAME";
  case INVALID_PI_NAME_OF_XML:
    return "INVALID_PI_NAME_OF_XML";
  case INVALID_CHAR_IN_ELEMENT_NAME:
    return "INVALID_CHAR_IN_ELEMENT_NAME";
  case INVALID_CHAR_IN_ATTRIBUTE_NAME:
    return "INVALID_CHAR_IN_ATTRIBUTE_NAME";
  case RESTRICTED_CHAR:
    return "RESTRICTED_CHAR";
  case INVALID_CHAR_IN_ENTITY_REFERENCE:
    return "INVALID_CHAR_IN_ENTITY_REFERENCE";
  case INVALID_CHAR_IN_CHAR_REFERENCE:
    return "INVALID_CHAR_IN_CHAR_REFERENCE";
  case INVALID_CHAR_IN_XML_DECL:
    return "INVALID_CHAR_IN_XML_DECL";
  case EXPECTING_EQUALS:
    return "EXPECTING_EQUALS";
  case EXPECTING_WHITESPACE:
    return "EXPECTING_WHITESPACE";
  case UNKNOWN_XML_VERSION:
    return "UNKNOWN_XML_VERSION";
  case OUT_OF_MEMORY:
    return "OUT_OF_MEMORY";
  case ELEMENT_NAME_MISMATCH:
    return "ELEMENT_NAME_MISMATCH";
  case NO_URI_FOR_PREFIX:
    return "NO_URI_FOR_PREFIX";
  case DUPLICATE_ATTRIBUTES:
    return "DUPLICATE_ATTRIBUTES";
  case INVALID_ENCODING_VALUE:
    return "INVALID_ENCODING_VALUE";
  case CDATA_END_IN_ELEMENT_CONTENT:
    return "CDATA_END_IN_ELEMENT_CONTENT";
  case UNDEFINED_ENTITY:
    return "UNDEFINED_ENTITY";
  case DOCTYPE_NOT_IMPLEMENTED:
    return "DOCTYPE_NOT_IMPLEMENTED";
  case NO_ERROR:
    break;
  }

  return "NO_ERROR";
}

