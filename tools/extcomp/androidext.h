/* Copyright (C) 2013 Mobile Sorcery AB

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
void writeAndroidStubs(string&, Interface&, string&);
string toAndroidType(Interface&, string&, string&, bool, bool, bool);
string getAndroidDefaultValue(string&);

#endif /* ANDROIDEXT_H_ */
