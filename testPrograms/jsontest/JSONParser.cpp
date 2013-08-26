/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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