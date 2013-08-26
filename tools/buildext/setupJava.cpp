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
