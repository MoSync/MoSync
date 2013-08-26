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

#include <string.h>

#include "config.h"
#include "helpers/log.h"

#include "sld.h"
#include "CoreCommon.h"

#include "cmd_break.h"
#include "StubConnection.h"
#include "helpers.h"
#include "commandInterface.h"

using namespace std;

void file_exec_and_symbols(const string& args);
void file_list_exec_source_files(const string& args);

void file_exec_and_symbols(const string& args) {
	if(!loadSLD(args.c_str())) {
		error("Could not load SLD file");
		return;
	}
	oprintDoneLn();
	commandComplete();
}

void file_list_exec_source_files(const string& args) {
	oprintDone();
	oprintf(",files=[");
	const vector<FileMapping>& files(sldFiles());
	for(size_t i=0; i<files.size(); i++) {
		if(i != 0)
			oprintf(",");
		const char* f = files[i].name.c_str();
		const char* name = strrchr(f, '/');
		if(name)
			oprintf("{file=\"%s\",fullname=\"%s\"}", name + 1, f);
		else
			oprintf("{file=\"%s\"}", f);
	}
	oprintf("]\n");
	commandComplete();
}
