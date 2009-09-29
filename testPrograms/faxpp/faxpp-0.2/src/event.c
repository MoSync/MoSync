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

#include <faxpp/event.h>

#ifdef DEBUG
const char *
event_to_string(FAXPP_EventType type)
{
  switch(type) {
  case START_DOCUMENT_EVENT:
    return "START_DOCUMENT_EVENT";
  case END_DOCUMENT_EVENT:
    return "END_DOCUMENT_EVENT";
  case START_ELEMENT_EVENT:
    return "START_ELEMENT_EVENT";
  case SELF_CLOSING_ELEMENT_EVENT:
    return "SELF_CLOSING_ELEMENT_EVENT";
  case END_ELEMENT_EVENT:
    return "END_ELEMENT_EVENT";
  case CHARACTERS_EVENT:
    return "CHARACTERS_EVENT";
  case CDATA_EVENT:
    return "CDATA_EVENT";
  case IGNORABLE_WHITESPACE_EVENT:
    return "IGNORABLE_WHITESPACE_EVENT";
  case COMMENT_EVENT:
    return "COMMENT_EVENT";
  case PI_EVENT:
    return "PI_EVENT";
  case ENTITY_REFERENCE_EVENT:
    return "ENTITY_REFERENCE_EVENT";
  case DEC_CHAR_REFERENCE_EVENT:
    return "DEC_CHAR_REFERENCE_EVENT";
  case HEX_CHAR_REFERENCE_EVENT:
    return "HEX_CHAR_REFERENCE_EVENT";
  case NO_EVENT: break;
  }

  return "NO_EVENT";
}
#endif
