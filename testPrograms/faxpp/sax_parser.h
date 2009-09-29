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

#include <stdio.h>
#include <faxpp/parser.h>

typedef void (*FAXPP_SAX_StartDocumentCallback)();
typedef void (*FAXPP_SAX_EndDocumentCallback)();
typedef void (*FAXPP_SAX_StartElementCallback)(const FAXPP_Event* event);
typedef void (*FAXPP_SAX_EndElementCallback)(const FAXPP_Event* event);
typedef void (*FAXPP_SAX_CDataCallback)(const FAXPP_Event* event);

struct FAXPP_SAX_Parser_t;
typedef struct FAXPP_SAX_Parser_t FAXPP_SAX_Parser;

/* callbacks may be null */
/* check errorString uppon failure (return val == NULL) */
FAXPP_SAX_Parser* FAXPP_SAX_create_parser(
	FAXPP_SAX_StartDocumentCallback startDocumentCallback,
	FAXPP_SAX_EndDocumentCallback endDocumentCallback,
	FAXPP_SAX_StartElementCallback startElementCallback,
	FAXPP_SAX_EndElementCallback endElementCallback,
	FAXPP_SAX_CDataCallback cDataCallback
	);

FAXPP_Error FAXPP_SAX_parse(FAXPP_SAX_Parser *parser, void* data, unsigned int length, void **bufferEnd, BOOL done);
void FAXPP_SAX_close(FAXPP_SAX_Parser *parser);
