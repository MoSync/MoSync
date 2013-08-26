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


#ifndef ANDROIDEXT_H_
#define ANDROIDEXT_H_

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <idl-common/idl-common.h>
#include <idl-common/types.h>

using namespace std;

void generateAndroidFiles(string&, Interface&, string&, string&);
void writeJNIBridge(string& outputDir, Interface& ext, string& androidPackageName);
void streamJNIFunctionWrapper(ostream& out, Interface& ext, Function& f, string& androidPackageName);
string getJavaByteCodeSignature(Interface& ext, Function& f, string& androidPackageNames);
string getBytecodeStructClass(string& package, string& type);
void streamAndroidExtMF(ostream&, Interface&, string&, string&);
void writeExtensionClass(string& extensionFile, Interface& ext, string& androidPackageName, bool interface);
void writeAndroidStubs(string&, Interface&, string&, bool);
string toJavaClassName(string& name);
string toAndroidType(Interface&, string&, string&, bool, bool, bool);
string getAndroidDefaultValue(string&);

#endif /* ANDROIDEXT_H_ */
