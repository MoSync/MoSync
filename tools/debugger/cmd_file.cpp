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
