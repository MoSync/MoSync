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


static void output_error_props(int code, const char* name, const char* desc) {
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

static void gen_docxml() {
	header();

#define OUTPUT_ERROR(val, id, desc) output_error(val, #id, desc);
#define OUTPUT_SET(set) { cout << " \n";\
	set##_ERRORS(OUTPUT_ERROR); cout << "\n"; }

	ERROR_SETS(OUTPUT_SET);

	footer();
}

static void gen_props() {

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
