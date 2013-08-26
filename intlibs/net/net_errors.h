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

#define NET_ERRORS(m)\
	m(280001, ERR_HTTP_METHOD_INVALID, "HTTP method invalid")\
	m(280002, ERR_HTTP_READONLY_WRITE, "Attempted to write to a read-only HTTP connection")\
	m(280003, ERR_URL_SPACE, "Whitespace is not allowed in URLs")\
	m(280004, ERR_CONN_READFROM, "readFrom not allowed on this connection")\
	m(280005, ERR_CONN_WRITETO, "writeTo not allowed on this connection")\

DECLARE_ERROR_ENUM(NET)
