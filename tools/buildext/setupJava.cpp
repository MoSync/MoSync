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

#include <idl-common/idl-common.h>

//temp hack
string tmpClassDir = "c:/SonyEricsson/JavaME_SDK_CLDC/PC_Emulation/WTK2/apps/MAVM/tmpclasses";
string includeDir = "c:/mosync-trunk/runtimes/java/source";
string outputClassDir = "c:/SonyEricsson/JavaME_SDK_CLDC/PC_Emulation/WTK2/apps/MAVM/classes";

//let's just do a single runtime to begin with.
void setupJavaRuntimes(const vector<Interface>& infs) {
	//The Runtimes here refer to the set of supported runtimes for these extensions.

	//compile each extension JPP file, put them with The Runtimes' class files.
	string javaLibDir = getJ2meDir() + "/lib";

	for(size_t i=0; i<infs.size(); i++) {
		const Interface& inf(infs[i]);
		string javaFile = "Output/" + inf.name + ".java";
		runCommand("xgcc -x c -IOutput -I\"" + includeDir + "\" "
			"-E \"" + inf.path + "/" + inf.name + ".jpp\" | "
			"sed 's/^# /\\/\\//' > " + javaFile);
	}

	runCommand(getJdkDir() + "/bin/javac -source 1.4 -target 1.4"
		" -d " + tmpClassDir +
		" -classpath " + tmpClassDir + ";"
		" -bootclasspath " +
		javaLibDir + "/jsr082.jar;" +
		javaLibDir + "/cldcapi11.jar;" +
		javaLibDir + "/midpapi20.jar;" +
		javaLibDir + "/jsr179.jar " +
		"Output/*.java");

	//preverify/obfuscate The Runtime JARs.
	//we'll just do MAVM for now
	runCommand(getJ2meDir() + "/bin/preverify "
		"-d \"" + outputClassDir + "\" "
		"-classpath " +
		javaLibDir + "/jsr082.jar;" +
		javaLibDir + "/cldcapi11.jar;" +
		javaLibDir + "/midpapi20.jar;" +
		javaLibDir + "/jsr179.jar " +
		"\"" + tmpClassDir + "\"");

	//disable all runtimes that do not support the extensions.
}

//remove all extensions, restore any disabled runtimes.
void clearJava() {
}
