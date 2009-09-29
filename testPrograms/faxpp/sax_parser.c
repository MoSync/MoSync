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

#include "sax_parser.h"

struct FAXPP_SAX_Parser_t {
	FAXPP_Parser *parser;
	BOOL firstParse;
	
	FAXPP_SAX_StartDocumentCallback startDocumentCallback;
	FAXPP_SAX_EndDocumentCallback endDocumentCallback;
	FAXPP_SAX_StartElementCallback startElementCallback;
	FAXPP_SAX_EndElementCallback endElementCallback;
	FAXPP_SAX_CDataCallback cDataCallback;
};

void FAXPP_SAX_DoEvent(FAXPP_SAX_Parser *parser, const FAXPP_Event *event)
{
  int i;
  FAXPP_AttrValue *atval;

  switch(event->type) {
  case START_DOCUMENT_EVENT:
    if(event->version.ptr != 0) {
		if(parser->startDocumentCallback) 
			parser->startDocumentCallback();
    }
    break;
  case END_DOCUMENT_EVENT:
  		if(parser->endDocumentCallback) 
			parser->endDocumentCallback();
    break;
  case START_ELEMENT_EVENT:
  case SELF_CLOSING_ELEMENT_EVENT:
    	if(parser->startElementCallback) 
			parser->startElementCallback(event);
		if(event->type == SELF_CLOSING_ELEMENT_EVENT) {
			if(parser->endElementCallback)  
				parser->endElementCallback(event);
		}
    break;
  case END_ELEMENT_EVENT:
     	if(parser->endElementCallback)  
			parser->endElementCallback(event);
    break;
  case CHARACTERS_EVENT:
  case CDATA_EVENT:
      	if(parser->cDataCallback)   
			parser->cDataCallback(event);
    break;
  case IGNORABLE_WHITESPACE_EVENT:
    break;
  case COMMENT_EVENT:
    break;
  case PI_EVENT:
    break;
  case ENTITY_REFERENCE_EVENT:
    break;
  case DEC_CHAR_REFERENCE_EVENT:
    break;
  case HEX_CHAR_REFERENCE_EVENT:
    break;
  case NO_EVENT:
    break;
  }
}

/* callbacks may be null */
/* NULL IF OUT OF MEMORY */
FAXPP_SAX_Parser* FAXPP_SAX_create_parser(
	FAXPP_SAX_StartDocumentCallback startDocumentCallback,
	FAXPP_SAX_EndDocumentCallback endDocumentCallback,
	FAXPP_SAX_StartElementCallback startElementCallback,
	FAXPP_SAX_EndElementCallback endElementCallback,
	FAXPP_SAX_CDataCallback cDataCallback
	) {
	FAXPP_SAX_Parser *parser = (FAXPP_SAX_Parser*) malloc(sizeof(FAXPP_SAX_Parser));
	parser->parser = FAXPP_create_parser(NO_CHECKS_PARSE_MODE, FAXPP_utf8_encode);
	if(parser->parser == 0) {
		return NULL;
	}
	parser->firstParse = TRUE;

	parser->startDocumentCallback = startDocumentCallback;
	parser->endDocumentCallback = endDocumentCallback;
	parser->startElementCallback = startElementCallback;
	parser->endElementCallback = endElementCallback;	
	parser->cDataCallback = cDataCallback;
	
	return parser;
}

FAXPP_Error FAXPP_SAX_parse(FAXPP_SAX_Parser *parser, void* data, unsigned int length, void **bufferEnd, BOOL done) {
	 FAXPP_Error err;
	  
	if(parser->firstParse) {
		FAXPP_init_parse(parser->parser, data, length, done);
		parser->firstParse = FALSE;
	} else {
		FAXPP_continue_parse(parser->parser, data, length, done);		
	}
	
    while((err = FAXPP_next_event(parser->parser)) == 0) {
		FAXPP_SAX_DoEvent(parser, FAXPP_get_current_event(parser->parser));

		if(FAXPP_get_current_event(parser->parser)->type == END_DOCUMENT_EVENT)
			break;
    }
	
	if(err == PREMATURE_END_OF_BUFFER) {
		if(bufferEnd)
			FAXPP_release_buffer(parser->parser, bufferEnd);
	} 
	
	return err;
}

void FAXPP_SAX_close(FAXPP_SAX_Parser *parser) {
	FAXPP_free_parser(parser->parser);
	free(parser);
}
