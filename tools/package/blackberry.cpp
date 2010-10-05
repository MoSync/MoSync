#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "packagers.h"

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS& s) {
	testDst(s);
	testName(s);
	std::stringstream cmd;
	cmd << "java -Xmx256m -classpath \""<<mosyncdir()<<"/bin/bb40/rapc.jar\""
		" net.rim.tools.compiler.Compiler"
		" \"import="<<mosyncdir()<<"/bin/bb40/net_rim_api.jar\""
		" \"codename="<<s.name<<"\" -midlet"
		" \"jad="<<s.name<<".jad\" \""<<s.name<<".jar\"";
	sh(cmd.str().c_str());

	std::string codName(s.name);
	codName += ".cod";
	int res = rename(codName.c_str(), (s.dst + ("/" + codName)).c_str());
	if(res != 0) {
		printf("rename error %i, %i\n", res, errno);
		exit(1);
	}
}
