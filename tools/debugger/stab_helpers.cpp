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

#include "config.h"
#include "helpers/helpers.h"

#include "stabs/stabs.h"
#include "sld.h"

#include "stab_helpers.h"

const Function* stabsFindFunctionByInsideAddress(int pc) {
	int start = mapFunctionStart(pc);
	if(start < 0)
		return NULL;
	return stabsGetFunctionByAddress(start);
}
