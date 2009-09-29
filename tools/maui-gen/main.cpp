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

#include <expat.h>
#include <stdio.h>

#include <stack>
#include <string>
#include <map>
#include <vector>
#include <set>

#include <fstream>
using namespace std;

string gBmFontFile = "bmfont.bmfc.template";
string gOutputPath = "";

// resources...
struct Font {
	string resourceName;
	string mofOutput;
	string fntOutput;
	string pngOutput;
};

map<string, Font> gFonts;
map<string, string> gImages;
// ----------------------------------------- 
int gWidgetID = 0;
string integerToString(int i) {
	char temp[16];
	itoa(i, temp, 10);
	return temp;
}

char *readFileIntoMem(const char* filename, int *len) {
	FILE *file = fopen(filename, "rb");
	if(!file) return NULL;
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *memory = new char[length];
	fread(memory, length, 1, file);
	fclose(file);
	*len = length;
	return memory;
}

bool writeMemIntoFile(const char* filename, const char *mem, int len) {
	FILE *file = fopen(filename, "wb");
	if(!file) return false;
	fwrite(mem, len, 1, file);
	fclose(file);
	return true;
}

struct WidgetNode {
	string widgetType;
	map<string, string> attributes;
	vector<WidgetNode*> children;
	WidgetNode *parent;
	int level;
};

void printWidgetTree(WidgetNode *root, int level=0) {
	for(int i = 0; i < level; i++) printf(" ");
	printf("%s\n", root->widgetType.c_str());
	for(int i = 0; i < root->children.size(); i++) {
		printWidgetTree(root->children[i], level+1)	;
	}

}

void error(const char *why) {
	printf("error: %s\n", why);
}

void findUniqueWidgetTypes(fstream& stream, WidgetNode *root, set<string> &widgetTypes) {
	if(root->widgetType!="Gui" && root->widgetType!="root")
		widgetTypes.insert(root->widgetType);
	for(int i = 0; i < root->children.size(); i++)
		findUniqueWidgetTypes(stream, root->children[i], widgetTypes); 
}

void generateIncludeDirectives(fstream& stream, WidgetNode *root) {
	set<string> widgetTypes;
	findUniqueWidgetTypes(stream, root, widgetTypes);
	for(set<string>::iterator i = widgetTypes.begin(); i != widgetTypes.end(); i++) {
		stream << "#include <MAUI/" << *i << ".h>\n";
	}
}

void replaceTemplateDefine(string &templateFile, const string &whatToReplace, const string &replacement) {
	size_t index, index2;
	while((index=templateFile.find(whatToReplace))!=string::npos) {
		int endOfReplacement = index+whatToReplace.length();
		templateFile = templateFile.substr(0, index) + replacement + templateFile.substr(endOfReplacement, templateFile.size()-endOfReplacement);
	}
}

bool generateFont(Font& font, map<string, string>& fontAttributes) {
	static int handle = 0;
	int len;

	string fontType = fontAttributes["type"];
	if(fontType=="") { error("missing font type"); return false; }

	font.resourceName = "FONT" + integerToString(handle++);

	const char *c= getenv("MOSYNCDIR");
	if(!c) { error("no MOSYNCDIR"); return false; }
	string mosyncdir = c;

	char *fontTemplate = readFileIntoMem(gBmFontFile.c_str(), &len);
	string fontTemplateStr = string(fontTemplate,len);
	delete fontTemplate;

	replaceTemplateDefine(fontTemplateStr, "%fontType%", fontType);

	string fontSize = fontAttributes["size"];
	if(fontSize=="") { fontSize = "12"; }
	replaceTemplateDefine(fontTemplateStr, "%fontSize%", fontSize);


	string fontColor = fontAttributes["color"];
	if(fontColor=="") { fontColor = "ffffff"; }

	writeMemIntoFile("bmfont.bmfc", fontTemplateStr.c_str(), fontTemplateStr.length());

	font.fntOutput = gOutputPath + "\\" + font.resourceName + ".fnt";
	font.pngOutput = gOutputPath + "\\" + font.resourceName + "_00.png";
	string commandLine = (mosyncdir + "\\bin\\BMFont\\bmfont -c bmfont.bmfc -o " + font.fntOutput);
	printf("font generator cmd: %s\n", commandLine.c_str());

	if(system(commandLine.c_str()) != 0) {
		error("font couldn't be generated.");
		return false;
	}

	font.mofOutput = gOutputPath + "\\" + font.resourceName + ".mof";
	commandLine = mosyncdir + "\\bin\\mof -fontData " + font.fntOutput + " -fontImage " + font.pngOutput + " -outFile " + font.mofOutput + " -fontColor " + fontColor;
	printf("mof generator cmd: %s\n", commandLine.c_str());
	if(system(commandLine.c_str()) != 0) {
		error("font couldn't be generated.");
		return false;
	}

	return true;
}

#define DEFAULT(x, y) (node->attributes[x]!=""?node->attributes[x]:(y))

class WidgetEmitter {
public:
	void emitWidgetCppPre(fstream& stream, WidgetNode *node) {
		stream << "\t";
		if(node->attributes["id"]!="") {
			stream << node->attributes["id"] << " = ";
		} else {
			node->attributes["id"] = "unnamed" + node->widgetType + integerToString(gWidgetID++);
			stream << node->widgetType << " *" << node->attributes["id"] << " = ";
		}

		

		stream << "new " << node->widgetType << "(" <<
			DEFAULT("x", "0") << ", " <<
			DEFAULT("y", "0") << ", " <<
			DEFAULT("width", "0") << ", " <<
			DEFAULT("height", "0") << ", ";

		if(node->parent->widgetType == "Screen") {
			stream << "NULL";
		} else {
			stream << ((node->parent)?node->parent->attributes["id"]:"NULL");
		}

		stream << ", ";
	}

	void emitWidgetCppPost(fstream& stream, WidgetNode *node) {
		if(node->parent->widgetType == "Screen") {
			stream << "\t" << node->parent->attributes["id"] << "->setMain(" << node->attributes["id"] << ");\n";
		} else {
			if(node->attributes["paddingLeft"] != "") 
				stream << "\t" << node->attributes["id"] << "->setPaddingLeft("<< node->attributes["paddingLeft"] << ");\n";
			if(node->attributes["paddingRight"] != "") 
				stream << "\t" << node->attributes["id"] << "->setPaddingRight("<< node->attributes["paddingRight"] << ");\n";
			if(node->attributes["paddingTop"] != "") 
				stream << "\t" << node->attributes["id"] << "->setPaddingTop("<< node->attributes["paddingTop"] << ");\n";
			if(node->attributes["paddingBottom"] != "") 
				stream << "\t" << node->attributes["id"] << "->setPaddingBottom("<< node->attributes["paddingBottom"] << ");\n";
		}
	}

	virtual void emit(fstream& stream, WidgetNode *node) = 0;
};

class ImageWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {
		if(node->attributes["src"] == "") error("no image source");

		emitWidgetCppPre(stream, node);
		stream <<
			DEFAULT("autoSizeX", "true") << ", " <<
			DEFAULT("autoSizeY", "true") << ", " <<
			gImages[node->attributes["src"]] << ");\n";
		emitWidgetCppPost(stream, node);
	}
};

class ListBoxWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {
		emitWidgetCppPre(stream, node);
		stream <<
			"ListBox::" << ((node->attributes["orientation"]!="")?(node->attributes["orientation"]):"LBO_VERTICAL") << ", " <<
			"ListBox::" << ((node->attributes["animType"]!="")?(node->attributes["orientation"]):"LBA_NONE") << ", " <<
			((node->attributes["wrapping"]!="")?(node->attributes["wrapping"]):"false") << ");\n";
		emitWidgetCppPost(stream, node);

		if(node->attributes["autoSize"] != "") 
			stream << "\t" << node->attributes["id"] << "->setAutoSize("<< node->attributes["autoSize"] << ");\n";


	}
};

class LayoutWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {

		if(node->attributes["gridXSize"]=="") error("Layout needs gridXSize");
		if(node->attributes["gridYSize"]=="") error("Layout needs gridYSize");


		emitWidgetCppPre(stream, node);
		stream <<
			node->attributes["gridXSize"] << ", " <<
			node->attributes["gridYSize"] << ");\n";

		
		if(node->attributes["horizontalAlignment"] != "") 
			stream << "\t" << node->attributes["id"] << "->setHorizontalAligment(Layout::" << node->attributes["horizontalAlignment"] << ");\n";
		if(node->attributes["verticalAlignment"] != "") 
			stream << "\t" << node->attributes["id"] << "->setVerticalAligment(Layout::" << node->attributes["verticalAlignment"] << ");\n";
		if(node->attributes["autoSizeX"] != "") 
			stream << "\t" << node->attributes["id"] << "->setAutoSizeX("<< node->attributes["autoSizeX"] << ");\n";
		if(node->attributes["autoSizeY"] != "") 
			stream << "\t" << node->attributes["id"] << "->setAutoSizeY("<< node->attributes["autoSizeY"] << ");\n";

		emitWidgetCppPost(stream, node);

	}
};

class LabelWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {
		emitWidgetCppPre(stream, node);
		stream << "\"" << node->attributes["caption"] << "\", " << 
		DEFAULT("backColor", "0x0") << ", ";

		if(node->attributes["font"] != "")
			stream << "new Font(" + gFonts[node->attributes["font"]].resourceName << "));\n";
		else
			stream << "NULL);\n";
		
		if(node->attributes["horizontalAlignment"] != "") 
			stream << "\t" << node->attributes["id"] << "->setHorizontalAlignment(Label::" << node->attributes["horizontalAlignment"] << ");\n";
		if(node->attributes["verticalAlignment"] != "") 
			stream << "\t" << node->attributes["id"] << "->setVerticalAlignment(Label::" << node->attributes["verticalAlignment"] << ");\n";
		if(node->attributes["autoSizeX"] != "") 
			stream << "\t" << node->attributes["id"] << "->setAutoSizeX("<< node->attributes["autoSizeX"] << ");\n";
		if(node->attributes["autoSizeY"] != "") 
			stream << "\t" << node->attributes["id"] << "->setAutoSizeY("<< node->attributes["autoSizeY"] << ");\n";


		emitWidgetCppPost(stream, node);
	}
};

class ScreenWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {
		if(node->attributes["id"] == "") error("Screen needs an id");

		string ptr = node->attributes["id"] ;

		stream << "\tif(!" << ptr << ")\n\t\t" <<
			ptr << " = new Screen();\n";
	}
};

class HeaderWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {
		if(node->attributes["id"]!="") {
			stream << "\tMAUI::" << node->widgetType << "* " << node->attributes["id"] << ";\n";
		}
	}
};

class FontResourceWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {

		Font& font = gFonts[node->attributes["id"]];
		if(!generateFont(font, node->attributes)) {
			error("could not generate font");
			return;
		}
		stream << ".res " << font.resourceName << "\n";
		stream << ".bin\n";
		stream << ".include \"" << font.resourceName << ".mof\"\n\n";
	}
};

class ImageResourceWidgetEmitter : public WidgetEmitter {
public:
	void emit(fstream& stream, WidgetNode *node) {
		static int handle = 0;
		if(node->attributes["src"]=="") { error("no source in image widget"); return; }
		if(gImages[node->attributes["src"]]!="") return;
		string resourceName = "IMAGE" + integerToString(handle++);
		stream << ".res " << resourceName << "\n";
		stream << ".image \"" << node->attributes["src"] << "\"\n\n";
		gImages[node->attributes["src"]] = resourceName;
	}
};

class GuiGenerator {
public:
	GuiGenerator() {
	}

	void addEmitter(const string& name, WidgetEmitter* emitter) {
		emitters[name] = emitter;
	}

	void generate(fstream &stream, WidgetNode *node, int level=0) {
		emit(stream, node);
		for(int i = 0; i < node->children.size(); i++) { 
			generate(stream, node->children[i], level+1);
		}	
	}
private:
	void emit(fstream &stream, WidgetNode *node) {		
		if(emitters[node->widgetType]) {
			emitters[node->widgetType]->emit(stream, node);
		}
	}

	map<string, WidgetEmitter*> emitters;
};

stack<WidgetNode*> widgetStack;

void
start(void *data, const char *el, const char **attr) {
	int i;
	if(string(el) == "Gui") return;

	WidgetNode *current = new WidgetNode();
	if(widgetStack.size()) {
		current->parent = widgetStack.top();
		current->parent->children.push_back(current);
	}
	if(widgetStack.size()==1) current->parent = NULL;
	current->widgetType = el;
	current->level = widgetStack.size();
	for (i = 0; attr[i]; i += 2) {
		current->attributes[attr[i]] = attr[i+1];
	}
	if(current->widgetType == "Screen" && current->level > 1) {
		error("MAUI::Screen(s) can only exist in the root");
	}
	widgetStack.push(current);

}  /* End of start handler */

void
end(void *data, const char *el) {
	widgetStack.pop();
}  /* End of end handler */

string generateScreenArguments(WidgetNode *root, bool useNamespace=false) {
	string arguments = "";
	const vector<WidgetNode*> &screens = root->children;
	for(int i = 0; i < screens.size(); i++) {
		if(screens[i]->widgetType != "Screen") continue;
		if(useNamespace)
			arguments += "MAUI::";
		arguments += "Screen *" + screens[i]->attributes["id"];
		if(i!=screens.size()-1) arguments += ",";
	}
	return arguments;
}

string generateScreenInitializerList(WidgetNode *root) {
	string initializerList = "";
	const vector<WidgetNode*> &screens = root->children;
	for(int i = 0; i < screens.size(); i++) {
		if(screens[i]->widgetType != "Screen") continue;
		initializerList += screens[i]->attributes["id"] + "(" + screens[i]->attributes["id"] + ")";
		if(i!=screens.size()-1) initializerList += ",";
	}
	return initializerList;
}

void generateCppFile(WidgetNode *root, const char *outputName, const char *outputFileH, const char *outputFileCpp) {
	GuiGenerator guiGenerator;
	guiGenerator.addEmitter("Screen", new ScreenWidgetEmitter());
	guiGenerator.addEmitter("ListBox", new ListBoxWidgetEmitter());
	guiGenerator.addEmitter("Label", new LabelWidgetEmitter());
	guiGenerator.addEmitter("Layout", new LayoutWidgetEmitter());
	guiGenerator.addEmitter("Image", new ImageWidgetEmitter());
	fstream stream;
	stream.open(outputFileCpp, ios::out);
	

	stream << "#include \"" << outputName << ".h\"\n\n";
	stream << "#include \"MAHeaders.h\"\n";

	stream << "using namespace MAUI;\n\n";
	stream << "Gui::Gui(" << generateScreenArguments(root) << ")";
	

	if( root->children.size() ) {
		stream << " : " << generateScreenInitializerList(root) << " {\n";
	}

	guiGenerator.generate(stream, root);
	stream << "}\n";

	stream.close();
}

void generateHeaderFile(WidgetNode *root, const char *outputFileH) {
	GuiGenerator guiGenerator;
	guiGenerator.addEmitter("Screen", new HeaderWidgetEmitter());
	guiGenerator.addEmitter("ListBox", new HeaderWidgetEmitter());
	guiGenerator.addEmitter("Label", new HeaderWidgetEmitter());
	guiGenerator.addEmitter("Layout", new HeaderWidgetEmitter());
	guiGenerator.addEmitter("Image", new HeaderWidgetEmitter());

	fstream stream;
	stream.open(outputFileH, ios::out);
	stream << "#ifndef _GUI_H_\n";
	stream << "#define _GUI_H_\n\n";
	
	stream << "#include <MAUI/Engine.h>\n";
	generateIncludeDirectives(stream, root);
	stream << "\n";

	stream << "class Gui {\n";
	stream << "\tpublic:\n";
	stream << "\tGui(" << generateScreenArguments(root, true) << ");\n";
	guiGenerator.generate(stream, root, 1);
	stream << "}; // class Gui\n\n";
	stream << "#endif /* _GUI_H_ */\n";
	stream.close();
}

void generateResourceFile(WidgetNode *root, const char *outputFileH) {
	GuiGenerator guiGenerator;
	guiGenerator.addEmitter("Font", new FontResourceWidgetEmitter());
	guiGenerator.addEmitter("Image", new ImageResourceWidgetEmitter());
	fstream stream;
	stream.open(outputFileH, ios::out);
	guiGenerator.generate(stream, root, 1);
	stream.close();
}

int main(int argc, char **argv) {

	string inputFile = "";
	string outputFile = "";

	for(int i = 1; i < argc; i++) {
		if(strcmp("-input", argv[i])==0) {
			i++;
			if(i>=argc) error("invalid argument to -input");
			inputFile = argv[i];
		} else
		if(strcmp("-bm-font-file", argv[i])==0) {
			i++;
			if(i>=argc) error("invalid argument to -bm-font-file");
			gBmFontFile = argv[i];
		} else
		if(strcmp("-output-name", argv[i])==0) {
				i++;
				if(i>=argc) error("invalid argument to -output-name");
				outputFile = argv[i];
		} else
		if(strcmp("-output-path", argv[i])==0) {
				i++;
				if(i>=argc) error("invalid argument to -output-path");
				gOutputPath = argv[i];
		}

	}

	if(inputFile=="") error("must specify an input file using -input");
	if(outputFile=="") outputFile = "gui";

	XML_Parser xmlParser = XML_ParserCreate("UTF-8");
	XML_SetElementHandler(xmlParser, start, end);

	WidgetNode root;
	root.widgetType = "root";
	widgetStack.push(&root);

	int fileLength;
	char *file = readFileIntoMem(inputFile.c_str(), &fileLength);
	XML_Parse(xmlParser, file, fileLength, 1);
	delete file;

	generateResourceFile(&root, (gOutputPath+"\\"+outputFile+".lst").c_str()); // must be first pass (others dependant on this)
	generateHeaderFile(&root, (gOutputPath+"\\"+outputFile+".h").c_str()); // must be before cpp file, cpp file updates the id field
	generateCppFile(&root, outputFile.c_str(), (gOutputPath+"\\"+outputFile+".h").c_str(), (gOutputPath+"\\"+outputFile+".cpp").c_str());

	return 0;
}