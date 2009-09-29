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

#include <MAUtil/Moblet.h>
#include <mastring.h>
#include <conprint.h>
#include <yajl/yajl_parse.h>
using namespace MAUtil;

#define JSON_OK 1

int parse_null(void * ctx) {
	return JSON_OK;
}

int parse_boolean(void * ctx, int boolVal) {
	return JSON_OK;
}

int parse_integer(void * ctx, long integerVal) {
	printf("There's an integer! %d\n", (int)integerVal);
	return JSON_OK;
}

int parse_double(void * ctx, double doubleVal) {
	printf("There's a double! %d, %f\n", (int)doubleVal, doubleVal);
	return JSON_OK;
}

/** A callback which passes the string representation of the number
 *  back to the client.  Will be used for all numbers when present */
int parse_number(void * ctx, const char * numberVal, unsigned int numberLen) {
	char buf[256];
	memset(buf, 0, 256);
	strncpy(buf, (const char*)numberVal, numberLen);
	printf("got a number: %s\n", buf);
	return JSON_OK;
}

/** strings are returned as pointers into the JSON text when,
 * possible, as a result, they are _not_ null padded */
int parse_string(void * ctx, const unsigned char * stringVal, unsigned int stringLen) {
	char buf[256];
	memset(buf, 0, 256);
	strncpy(buf, (const char*)stringVal, stringLen);
	printf("got a string: %s\n", buf);
	return JSON_OK;
}

int parse_start_map(void * ctx) {
	printf("start map\n");
	return JSON_OK;
}

int parse_map_key(void * ctx, const unsigned char * key, unsigned int stringLen) {
	char buf[256];
	memset(buf, 0, 256);
	strncpy(buf, (const char*)key, stringLen);
	printf("got a map key: %s\n", buf);
	return JSON_OK;
}

int parse_end_map(void * ctx) {
	return JSON_OK;
}

int parse_start_array(void * ctx) {
	return JSON_OK;
}

int parse_end_array(void * ctx) {
	return JSON_OK;
}

static const char* json = 
"{"
"      \"Image\": {"
"          \"Width\":  800,"
"          \"Height\": 600,"
"          \"Title\":  \"View from 15th Floor\","
"          \"Thumbnail\": {"
"              \"Url\":    \"http://www.example.com/image/481989943\","
"              \"Height\": 125,"
"              \"Width\":  \"100\""
"          },"
"          \"IDs\": [116, 943, 234, 38793.0]"
"		}}"
		

;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		// todo: initializtion
		
		yajl_callbacks callbacks = {
			parse_null,
			parse_boolean,
			parse_integer,
			parse_double,
			parse_number,
			//NULL,
			parse_string,
			parse_start_map,
			parse_map_key,
			parse_end_map,
			parse_start_array,
			parse_end_array
		};

		yajl_parser_config cfg = {1,0};
		yajl_handle parser = yajl_alloc(&callbacks, &cfg, NULL);
		printf("calling parse..\n");
		//yajl_status status = 
		yajl_parse(parser, (const unsigned char*)json, strlen(json));

		printf("Hellu\n");
	}

	void keyPressEvent(int keyCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
