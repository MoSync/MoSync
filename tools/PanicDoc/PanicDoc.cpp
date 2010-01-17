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

#include <iostream>
#include <iomanip>

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#include <helpers/helpers.h>
#include <bluetooth/bt_errors.h>
#include "../runtimes/cpp/base/base_errors.h"
#include "../runtimes/cpp/platforms/sdl/sdl_errors.h"
#include "../runtimes/cpp/platforms/sdl/windows_errors.h"
#include "../runtimes/cpp/platforms/symbian/inc/symbian_errors.h"
#include "../../runtimes/cpp/platforms/winmobile/wce_errors.h"
#include <net/net_errors.h>

#define ERROR_SETS(m) m(HELPERS) m(BASE) m(SDL) m(BT) m(WINDOWS) m(SYMBIAN) m(WCE) m(NET)

#include <helpers/errors_inline.h>

using namespace std;

void header();
void footer();
void output_error(int code, const char* name, const char* desc);

#define COLUMN_1 10
#define COLUMN_2 35


void output_error_props(int code, const char* name, const char* desc) {
	cout << code << "=" << desc << ".\n";
}

void output_error(int code, const char* name, const char* desc) {
	cout.width(COLUMN_1);
	cout << left;
	cout << code;
	cout.width(COLUMN_2);
	cout << left;
	cout << name;
	cout.width(0);
	cout << "// " << desc << ".\n";
}

void gen_docxml() {
	header();

#define OUTPUT_ERROR(val, id, desc) output_error(val, #id, desc);
#define OUTPUT_SET(set) { cout << " \n";\
	set##_ERRORS(OUTPUT_ERROR); cout << "\n"; }

	ERROR_SETS(OUTPUT_SET);

	footer();
}

void gen_props() {

#define OUTPUT_ERROR_PROPS(val, id, desc) output_error_props(val, #id, desc);
#define OUTPUT_PROPS_SET(set) { set##_ERRORS(OUTPUT_ERROR_PROPS);  }

	ERROR_SETS(OUTPUT_PROPS_SET);
}

int main(int argc, char** argv) {
	if(argc < 2)
		gen_docxml();
	else {
		if(!strcmp(argv[1], "-props"))
			gen_props();
		else if(!strcmp(argv[1], "-docxml"))
			gen_docxml();
		else {
			cout << "ERROR, invalid command line switch!";
			return 1;
		}
	}
	
	return 0;
}


void header() {
	cout <<
		"<document title=\"Panics\" author=\"Fredrik\" area=\"\" keyword=\"\">\n"
		"<section id=\"intro\" title=\"Panic Code Reference\">\n"
		"<![CDATA[\n"
		"[code filename=\"dummy.h\"]\n"
		"Code      Symbol                             Description\n"
		<< endl;
}

void footer() {
	cout <<
		"[/code]\n"
		"]]>\n"
		"</section>\n"
		"</document>\n"
		<< endl;
}
