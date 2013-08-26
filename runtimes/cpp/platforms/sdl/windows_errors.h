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

#define WINDOWS_ERRORS(m)\
	m(160001, WINERR_STORE_FILE_NOT_FOUND, "Store file not found")\
	m(160002, WINERR_STORE_ACCESS_DENIED, "Store file access denied")\
	m(160003, WINERR_STORE_DELETE_FAILED, "Store file delete failed")\
	m(160004, WINERR_TIMEZONE, "Could not retrieve time zone information")\

DECLARE_ERROR_ENUM(WINDOWS);
