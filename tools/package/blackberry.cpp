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

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "packagers.h"
#include "util.h"

using namespace std;

/*static string parseBlackberryVersion(const RuntimeInfo& ri) {
	unsigned major = ri.blackberryVersion, minor = ri.blackberryMinor;
	bool bad = false;
	bad |= major < 4;
	bad |= major > 6;
	if(bad) {
		printf("Error: bad Blackberry version (%i.%i)\n", major, minor);
		exit(1);
	}
	switch(major) {
	case 4:
		if(minor < 3)
			return "40";
		else
			return "47";
	case 5:
		return "500";
	case 6:
		return "600";
	default:
		exit(42);
	}
}*/

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testName(s);
	testBlackberryJde(s);

	if (s.blackberryPackAsCOD) {
		std::stringstream cmd;
		string bbbin = string(s.blackberryJde) + "/bin";
		string bblib = string(s.blackberryJde) + "/lib";
		const char* codnameParam =
				(ri.blackberryVersion <= 4 && ri.blackberryMinor < 3) ?
						"codename" : "codname";

		cmd << "java -Xmx256m -classpath \""<<bbbin<<"/rapc.jar\""
			" net.rim.tools.compiler.Compiler"
			" -noshortname -verbose -warning"
			" -listing="<<s.name<<" \"import="<<bblib<<"/net_rim_api.jar\""
			" \""<<codnameParam<<"="<<s.name<<"\" -quiet -midlet"
			" \"jad="<<s.name<<".jad\" \""<<s.name<<".jar\"";
		sh(cmd.str().c_str());

		/*std::string codName(s.name);
		codName += ".cod";
		std::string newName = s.dst + ("/" + codName);
		remove(newName.c_str());
		renameFile(newName, codName);*/

		// TODO: if the COD is a zip file, re-zip it for better compression.
	}
}
