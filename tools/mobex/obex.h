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

#ifndef OBEX_H
#define OBEX_H

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#include <helpers/array.h>
#include <bluetooth/connection.h>

int sendObject(const MABtAddr& address, const Array<char>& file, const Array<u16>& name,
							 int port, int maxPacketSize);

#define CONNERR_OBEX -1001


#define LOG_CODE LOG("code %i\n", __LINE__)

#undef FAIL
//#define FAIL(verb) { LOG("Failure %s, ", verb); LOG_CODE; return false; }

#undef TEST_LEZ
#define TEST_LEZ(a) { int res = (a); if(res <= 0) {\
	LOG("Failure sending, "); LOG_CODE; return res; } }


#endif	//OBEX_H
