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

#ifndef MUSTACHE_H
#define MUSTACHE_H

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <stdlib.h>

using namespace std;

class ParserCallback {

public:
	virtual ~ParserCallback() { };
	virtual void startTemplate() = 0;
	virtual void endTemplate() = 0;
	virtual void startSection(string section, bool negate) = 0;
	virtual void endSection(string section) = 0;
	virtual void parameter(string parameter) = 0;
	virtual void text(const string& text, size_t start, size_t end) = 0;
	virtual void directive(string directive) = 0;
};

class MustacheContext {
public:
	virtual ~MustacheContext() { };
	virtual string getParameter(string key) = 0;
	virtual vector<MustacheContext*> getChildren(string key) = 0;
};

class DefaultContext : public MustacheContext {
private:
	map<string, string> fParams;
	map<string, vector<MustacheContext*> > fChildren;
	MustacheContext* fParent;
public:
	DefaultContext(MustacheContext* parent) : fParams(), fChildren(), fParent(parent) { }
	void setParameter(string key, string value) { fParams[key] = value; }
	void addChild(string key, MustacheContext* child) { fChildren[key].push_back(child); }
	string getParameter(string key);
	vector<MustacheContext*> getChildren(string key) { return fChildren[key]; }
};

class Renderable {
public:
	Renderable() { }
	virtual ~Renderable() { };
	virtual void render(MustacheContext* context, ostream& output) { };
};

class RenderableList : public Renderable {
protected:
	vector<Renderable*> fChildren;
public:
	~RenderableList();
	void addChild(Renderable* child) { fChildren.push_back(child); }
	virtual void render(MustacheContext* context, ostream& output);
};

class Section : public RenderableList {
private:
	string fSection;
	bool fNegate;
public:
	Section(string key, bool negate) : fSection(key), fNegate(negate) { };
	virtual void render(MustacheContext* context, ostream& output);
};

class Parameter : public Renderable {
private:
	string fParam;
public:
	~Parameter() { }
	Parameter(string key) : fParam(key) { };
	void render(MustacheContext* context, ostream& output);
};

class Text : public Renderable {
private:
	string fText;
public:
	~Text() { }
	Text(const string& text) : fText(text) { }
	void render(MustacheContext* context, ostream& output);
};


class DefaultParserCallback : public ParserCallback {
private:
	MustacheContext* fContext;
	stack<RenderableList*> fRenderContext;
	ostream& fOutput;
	bool fTrim;
public:
	DefaultParserCallback(MustacheContext* context, ostream& output) : fContext(context), fOutput(output), fTrim(false) { }
	~DefaultParserCallback() { };
	void startTemplate();
	void endTemplate();
	void startSection(string section, bool negate);
	void endSection(string section);
	void parameter(string parameter);
	void text(const string&, size_t start, size_t end);
	void directive(string directive);
};

class MustacheParser {
private:
	size_t fPos;
	string fInput;
	stack<string> fSections;
	int fSuppressDepth;
	bool fStrict;
	string until(string token, bool consumeToken, bool doTrim);
	string untilWhiteSpace();
	bool match(char ch);
	void whiteSpace();
	bool isWhiteSpace(char ch);
	bool eof();
	string location();

public:
	MustacheParser(bool strict) : fPos(0), fSuppressDepth(-1), fStrict(strict) { };

	string parse(const string& input, ParserCallback* cb);
	string parseFile(string filename, ParserCallback* cb);
};

string trim(const string& input);

string truncateDebug(const string& str);

#endif
