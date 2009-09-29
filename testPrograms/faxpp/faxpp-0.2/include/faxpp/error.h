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

#ifndef __FAXPP__ERROR_H
#define __FAXPP__ERROR_H

/// An enumeration of the error conditions returned by faxpp
typedef enum {
  NO_ERROR  = 0,
  DOUBLE_DASH_IN_COMMENT,
  PREMATURE_END_OF_BUFFER,
  INVALID_START_OF_COMMENT,
  INVALID_CHAR_IN_START_ELEMENT,
  INVALID_CHAR_IN_ATTRIBUTE,
  INVALID_CHAR_IN_END_ELEMENT,
  NON_WHITESPACE_OUTSIDE_DOC_ELEMENT,
  BAD_ENCODING,
  UNSUPPORTED_ENCODING,
  ADDITIONAL_DOCUMENT_ELEMENT,
  INVALID_CHAR_IN_PI_NAME,
  INVALID_PI_NAME_OF_XML,
  INVALID_CHAR_IN_ELEMENT_NAME,
  INVALID_CHAR_IN_ATTRIBUTE_NAME,
  RESTRICTED_CHAR,
  INVALID_CHAR_IN_ENTITY_REFERENCE,
  INVALID_CHAR_IN_CHAR_REFERENCE,
  INVALID_CHAR_IN_XML_DECL,
  EXPECTING_EQUALS,
  EXPECTING_WHITESPACE,
  UNKNOWN_XML_VERSION,
  INVALID_ENCODING_VALUE,
  CDATA_END_IN_ELEMENT_CONTENT,
  UNDEFINED_ENTITY,
  DOCTYPE_NOT_IMPLEMENTED, ///< TBD Remove this error - jpcs
  OUT_OF_MEMORY,

  ELEMENT_NAME_MISMATCH,
  NO_URI_FOR_PREFIX,
  DUPLICATE_ATTRIBUTES
} FAXPP_Error;

/**
 * Returns a string describing the given error code
 * \param err The error code
 * \return a string
 */
const char *FAXPP_err_to_string(FAXPP_Error err);

#endif
