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

#ifndef EXTCOMP_H_
#define EXTCOMP_H_

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <map>

void streamFunctionCSignature(ostream&, Interface&, Function&);
void streamFunctionWrapper(ostream&, Interface&, Function&, bool);
string resolveTypedef(Interface&, string&);
void streamExtensionManifest(map<string,string>&);
void streamExtHashValue(ostream&, Interface&);
void streamXMLComment(ostream&, string& comment);
void writeHeaders(string& outputFile, Interface& ext, bool includeFunctions);
string getModHandle(Interface& ext);
string getFnIxDefine(Function& f);
string getFnIxHandle(Interface& f);
string getModHashDefine(Interface& ext);
bool isReturnType(Interface&, string&);
string extractPointerType(string type, int& ptrDepth);
Struct* getStruct(Interface& ext, string& structType);
int getPadding(Interface& ext, string& type);
size_t cTypeAlignedSize(Interface& ext, string& type);
string safeArg(string& arg);

#endif /* EXTCOMP_H_ */
