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

#define BT_ERRORS(m)\
	m(120001, BTERR_MAX_SDP_LEVEL, "Maximum SDP level reached")\
	m(120002, BTERR_DISCOVERY_IN_PROGRESS, "Discovery already in progress")\
	m(120003, BTERR_NEGATIVE_BUFFER_SIZE, "Negative buffer size")\
	m(120004, BTERR_READ_BUFFER_OVERFLOW, "Read buffer overflow")\
	m(120005, BTERR_NONAME, "No name supplied")\

DECLARE_ERROR_ENUM(BT)
