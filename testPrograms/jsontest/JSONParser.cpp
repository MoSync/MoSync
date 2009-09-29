/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "JSONParser.h"

#define JSON_OK 1

static char strBuf[1024];
#define PARSER ((*(MAUtil::JSONListener*)ctx))
#define P2STR(v, n) MAUtil::String(strncpy(strBuf, (v), (n)))

int parse_null(void * ctx) 					{ return PARSER.null(); }
int parse_boolean(void * ctx, int boolVal) 	{ return PARSER.boolean((bool)boolVal); }
int parse_number(void * ctx, const char * numberVal, unsigned int numberLen) {
	return PARSER.number(P2STR(numberVal, numberLen));
}

int parse_string(void * ctx, const unsigned char * stringVal, unsigned int stringLen) {
	return PARSER.string(P2STR(stringVal, stringLen));
}

int parse_start_map(void * ctx) {
	return PARSER.startMap();
}

int parse_map_key(void * ctx, const unsigned char * key, unsigned int stringLen) {
	return PARSER.mapKey(P2STR(key, stringLen));
}

int parse_end_map(void * ctx) 			{ return PARSER.endMap(); }
int parse_start_array(void * ctx)		{ return parser.startArray(); }
int parse_end_array(void * ctx) 		{ return PARSER.endArray(); }

namespace MAUtil {

	int JSONListener::null() {
		return JSON_OK;
	}
	int JSONListener::boolean(bool value) {
		return JSON_OK;
	}
	int JSONListener::number(const String& number) {
		return JSON_OK;
	}
	int JSONListener::string(const String& string) {
		return JSON_OK;
	}
	int JSONListener::startMap() {
		return JSON_OK;
	}
	int JSONListener::mapKey(const String& key) {
		return JSON_OK;
	}
	int JSONListener::endMap() {
		return JSON_OK;
	}
	int JSONListener::startArray() {
		return JSON_OK;
	}
	int JSONListener::endArray() {
		return JSON_OK;
	}
	
	JSONParser::JSONParser() {
		yajl_parser_config cfg = {1,0};
		yajl_handle parser = yajl_alloc(&callbacks, &cfg, NULL);
	}

	void JSONParser::parseFromMemory(const char* jsonText) {
		memset(strBuf, 0, 1024);
		
		yajl_callbacks callbacks = {
			parse_null,
			parse_boolean,
			NULL,
			NULL,
			parse_number,
			parse_string,
			parse_start_map,
			parse_map_key,
			parse_end_map,
			parse_start_array,
			parse_end_array
		};

		
		
		printf("calling parse..\n");
		//yajl_status status = 
		yajl_parse(parser, (const unsigned char*)json, strlen(json));
		
			
	}
	
	
	/* Asynchronous - function will return, and callbacks will be invoked later using Environment event handling. */
	void JSONParser::parseFromUrl(const char* url);

}