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

#ifndef XMLWRITER_H_
#define XMLWRITER_H_

#include <stack>
#include <sstream>
#include <string>

using namespace std;

class XMLWriter {
private:
	ostringstream* fOutput;
	stack<string> fElementStack;
	int fIndent;
	int fAttrCount;
	bool fIsLeaf;
	void indent();
	void line(string _line);
	void closeParentTag();
public:
	XMLWriter(ostringstream* output);
	void startTag(string name);
	void endTag();
	void setAttr(string key, string value);
	void text(string text);
	void start();
	void end();
	void dump();
};

#endif /* XMLWRITER_H_ */
