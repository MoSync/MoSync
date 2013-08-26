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

#ifndef COMMON_H
#define COMMON_H

// This is the service UUID, shared between client and server.

//#define SERVER_UUID "102030405060708090A0B0C0D0E0F010"
#define SERVER_UUID_1(m) m(10203040)
#define SERVER_UUID_2(m) m(50607080)
#define SERVER_UUID_3(m) m(90A0B0C0)
#define SERVER_UUID_4(m) m(D0E0F010)

#define S(s) #s
#define P(s) 0x##s

#define SERVER_UUID_STRING SERVER_UUID_1(S) SERVER_UUID_2(S) SERVER_UUID_3(S) SERVER_UUID_4(S)

#define SERVER_MAUUID_DECL {{ SERVER_UUID_1(P), SERVER_UUID_2(P), SERVER_UUID_3(P), SERVER_UUID_4(P) }}

#endif	//COMMON_H
