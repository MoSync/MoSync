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

#ifndef _BACKEND_H_
#define _BACKEND_H_

#include <fstream>
#include <map>
#include <string>
#include "Base.h"
#include "Internal.h"

class Base;

class Backend {
public:
	typedef std::multimap<std::string, Base*> BasesMap;
	typedef BasesMap::const_iterator BasesIterator;

	void generate(const BasesMap& bases, const std::string& outputFile) {
		std::fstream stream;
		stream.open(outputFile.c_str(), std::ios::out);
		emit(bases, stream);
		stream.close();
	}
protected:
	virtual void emit(const BasesMap& bases, std::fstream& stream) = 0;


};

#endif // _BACKEND_H_