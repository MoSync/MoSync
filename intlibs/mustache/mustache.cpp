/* Copyright (C) 2012 MoSync AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURfPosE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#include "mustache.h"
#include <fstream>

string MustacheParser::parseFile(string filename, ParserCallback* cb) {
	// TODO: Maybe we should use stream instead of string internally.
	// Probably not a big deal though.
	 ifstream file(filename.c_str(), ios::in|ios::binary|ios::ate);
	  if (file.is_open())
	  {
	    size_t size = file.tellg();
	    char* contents = new char [size + 1];
	    file.seekg (0, ios::beg);
	    file.read (contents, size);
	    contents[size] = '\0';
	    file.close();

	    string input = string(contents, size);
	    string result = parse(input, cb);
	    delete[] contents;
	    return result;
	  }

	  return string("Could not read file: ") + filename;
}

string MustacheParser::parse(const string& input, ParserCallback* cb) {
	fInput = input;
	string startDelim = string("{{");
	string endDelim = string("}}");

	bool debug = false;

	size_t verbatimStart = 0;
	size_t verbatimEnd = 0;

	cb->startTemplate();

	while (fPos < input.size()) {
		string text = until(startDelim, true, false);
		verbatimEnd = verbatimStart + text.size();

		cb->text(input, verbatimStart, verbatimEnd);

		if (fPos < input.size() - 1) {
			if (match('#') || match('^')) {
				bool negate = input[fPos - 1] == '^';

				whiteSpace();
				string section = until(endDelim, true, true);
				if (debug) {
					string parent = fSections.empty() ? "" : " (parent " + fSections.top() + ")";
					printf("%sStarted %ssection %s%s\n",
							location().c_str(), negate ? "negated " : "",
							section.c_str(),
							parent.c_str());
				}
				cb->startSection(section, negate);

				fSections.push(section);
			} else if (match('/')) {
				string startSection =
						fSections.empty() ? string() : fSections.top();
				if (!fSections.empty()) {
					fSections.pop();
				}
				string endSection = until(endDelim, true, true);
				if (startSection != endSection) {
					return "Unmatched section: " + endSection;
				}
				if (debug) {
					printf("%sEnded section %s\n", location().c_str(), endSection.c_str());
				}
				cb->endSection(endSection);
			} else if (match('=')) {
				string newStartDelim = untilWhiteSpace();
				newStartDelim = trim(newStartDelim);
				whiteSpace();
				string newEndDelim = until(endDelim, true, true);
				if (debug) {
					printf("%sChanged delimiters from %s and %s to %s and %s\n",
							location().c_str(), startDelim.c_str(), endDelim.c_str(),
							newStartDelim.c_str(), newEndDelim.c_str());
				}
				startDelim = newStartDelim;
				endDelim = newEndDelim;
			} else if (match('!')) {
				string comment = until(endDelim, true, false);
				if (debug) {
					printf("%sComment (%d chars): %s\n",
							location().c_str(), (int) comment.size(), truncateDebug(comment).c_str());
				}
			} else if (match('@')) {
				// Directives.
				string directive = until(endDelim, true, true);
				cb->directive(directive);
				if (directive == "debug") {
					debug = true;
				}
				if (debug) {
					printf("%sGot directive %s\n", location().c_str(), directive.c_str());
				}
			} else {
				string param = until(endDelim, true, true);
				cb->parameter(param);
				if (debug) {
					printf("%sParameter %s\n", location().c_str(), param.c_str());
				}
			}
			verbatimStart = fPos;
		}
	}

	if (!fSections.empty()) {
		return string("Unmatched section: ") + fSections.top();
	}

	cb->endTemplate();

	return string(); // <-- No error.
}

string MustacheParser::location() {
	char locationStr[20];
	sprintf(locationStr, "%06d: ", (int) fPos);
	return string(locationStr);
}

bool MustacheParser::match(char ch) {
	bool result = fInput[fPos] == ch;
	if (result) {
		fPos++;
	}
	return result;
}

string MustacheParser::until(string token, bool consumeToken, bool doTrim) {
	size_t startPos = fPos;
	size_t result = fInput.find(token, fPos); // <-- Just before the token!
	if (result == string::npos) {
		result = fInput.size();
		fPos = result;
	} else {
		fPos = result + (consumeToken ? token.size() : 0);
	}
	string str = fInput.substr(startPos, result - startPos);
	if (doTrim) {
		str = trim(str);
	}
	return str;
}

string MustacheParser::untilWhiteSpace() {
	size_t startfPos = fPos;
	bool found = false;
	while (!eof() && !found) {
		int ch = fInput[fPos];
		fPos++;
		if (isWhiteSpace(ch)) {
			fPos--;
			found = true;
		}
	}

	return fInput.substr(startfPos, fPos - startfPos + 1);
}

void MustacheParser::whiteSpace() {
	while (!eof() && isWhiteSpace(fInput[fPos])) {
		fPos++;
	}
}

bool MustacheParser::isWhiteSpace(char ch) {
	return ' ' == ch || '\t' == ch || '\n' == ch || '\r' == ch;
}

bool MustacheParser::eof() {
	return fPos >= fInput.size();
}

void DefaultParserCallback::startTemplate() {
	fRenderContext.push(new RenderableList());
}

void DefaultParserCallback::endTemplate() {
	RenderableList* doc = fRenderContext.top();
	doc->render(fContext, fOutput);
	fRenderContext.pop();
}

void DefaultParserCallback::startSection(string section, bool negate) {
	RenderableList* currentSection = fRenderContext.top();
	Section* sectionRenderable = new Section(section, negate);
	currentSection->addChild(sectionRenderable);
	fRenderContext.push(sectionRenderable);
}

void DefaultParserCallback::endSection(string section) {
	fRenderContext.pop();
}

void DefaultParserCallback::parameter(string key) {
	RenderableList* currentSection = fRenderContext.top();
	Parameter* parameterRenderable = new Parameter(key);
	currentSection->addChild(parameterRenderable);
}

void DefaultParserCallback::text(const string& textStr, size_t start, size_t end) {
	RenderableList* currentSection = fRenderContext.top();
	string str = textStr.substr(start, end - start);
	if (!fTrim || str.find_first_not_of(" \t\r\n") != string::npos) {
		Text* textRenderable = new Text(str);
		currentSection->addChild(textRenderable);
	}
}

void DefaultParserCallback::directive(string directiveName) {
	if (directiveName == "trim") {
		fTrim = true;
	} else if (directiveName == "no-trim") {
		fTrim = false;
	}
}


string DefaultContext::getParameter(string key) {
	string result = fParams[key];
	if (result.empty() && fParent) {
		return fParent->getParameter(key);
	}
	return result;
}

void RenderableList::render(MustacheContext* context, ostream& output) {
	for (size_t j = 0;  j < fChildren.size(); j++) {
		Renderable* renderable = fChildren[j];
		if (renderable) {
			renderable->render(context, output);
		}
	}
}

void Section::render(MustacheContext* context, ostream& output) {
	if (!context) {
		return;
	}
	vector<MustacheContext*> renderContexts;
	vector<MustacheContext*> children = context->getChildren(fSection);
	string param = context->getParameter(fSection);
	bool isDefined = !children.empty() || !param.empty();
	bool doRender = fNegate ^ isDefined;
	bool renderChildren = doRender && !children.empty();

	if (renderChildren) {
		renderContexts = children;
	} else if (doRender) {
		renderContexts.push_back(context);
	}

	for (size_t i = 0; i < renderContexts.size(); i++) {
		MustacheContext* renderContext = renderContexts[i];
		RenderableList::render(renderContext, output);
	}
}

RenderableList::~RenderableList() {
	for (size_t i = 0; i < fChildren.size(); i++) {
		Renderable* child = fChildren[i];
		if (child) {
			delete child;
		}
	}
}

void Parameter::render(MustacheContext* context, ostream& output) {
	if (!context) {
		return;
	}
	string paramValue = context->getParameter(fParam);
	output << paramValue;
}

void Text::render(MustacheContext* context, ostream& output) {
	output << fText;
}

string trim(const string& input) {
	string ws = " \t\r\n";
    size_t startIx = input.find_first_not_of(ws);
    if (startIx == std::string::npos) {
        return string();
    }

    size_t endIx = input.find_last_not_of(ws);

    return input.substr(startIx, endIx - startIx + 1);
}

string truncateDebug(const string& str) {
	string trimmed = trim(str);
	size_t strLen = trimmed.size();
	bool needsEllipsis = (int) strLen > 50;
	string truncated = trimmed.substr(0, needsEllipsis ? 50 : strLen) +
			((needsEllipsis) ? "..." : "");
	return truncated;
}
