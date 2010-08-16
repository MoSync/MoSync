#include <sstream>
#include <errno.h>
#include "packagers.h"

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS& s) {
	testDst(s);
	testName(s);
//java.exe -Xmx256m -classpath "c:\Program\Research In Motion\BlackBerry JDE 4.0\bin\rapc.jar"\;"c:\Program\Java\jdk1.6.0_04\lib\tools.jar" net.rim.tools.compiler.Compiler -exepath="c:\Program\Research In Motion\BlackBerry JDE 4.0\bin"
	std::stringstream cmd;
	cmd << "java -Xmx256m -classpath "<<mosyncdir()<<"/bin/bb40/rapc.jar"
		" net.rim.tools.compiler.Compiler"
		" import="<<mosyncdir()<<"/bin/bb40/net_rim_api.jar"
		" codename="<<s.name<<" -midlet"
		" jad="<<s.name<<".jad "<<s.name<<".jar";
	sh(cmd.str().c_str());

	std::string codName(s.name);
	codName += ".cod";
	int res = rename(codName.c_str(), (s.dst + ("/" + codName)).c_str());
	if(res != 0) {
		printf("rename error %i, %i\n", res, errno);
		exit(1);
	}
}
