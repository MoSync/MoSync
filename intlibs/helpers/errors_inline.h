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

#define IS_IN_SET(set) (code > ERROR_##set##_BASE && code < ERROR_##set##_END)

const char* MoSyncError::subSystemString(int code) {
#define RETURN_SET_STRING(set) if(IS_IN_SET(set)) return #set;
	ERROR_SETS(RETURN_SET_STRING);
	return NULL;
}

#define DECLARE_DESCRIPTIONS(set)\
	static const char* set##_descriptions[MoSyncError::ERROR_##set##_END];

ERROR_SETS(DECLARE_DESCRIPTIONS);

static void initErrors() {
	static bool inited = false;
	if(inited)
		return;
	inited = true;
	const char** curDescs;
	int curBase;
#define SET_INIT_D(val, id, desc) curDescs[val - curBase] = desc;
#define INIT_DESCRIPTIONS(set) curDescs = set##_descriptions;\
	curBase = MoSyncError::ERROR_##set##_BASE; set##_ERRORS(SET_INIT_D);
	ERROR_SETS(INIT_DESCRIPTIONS);
}

const char* MoSyncError::errorDescription(int code) {
	initErrors();
#define RETURN_DESCRIPTION(set)\
	if(IS_IN_SET(set)) return set##_descriptions[code - MoSyncError::ERROR_##set##_BASE];

	ERROR_SETS(RETURN_DESCRIPTION);
	return NULL;
}
