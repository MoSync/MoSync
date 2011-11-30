/* Copyright (C) 2011 MoSync AB

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

#include "pugixml-1.0/src/pugixml.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include "helpers/attribute.h"

using namespace std;

// The purpose of this program is to generate projects
// for Windows Phone using the same project as
// the one used when developing. This way the main project
// can be used as a template bundled with the mosync framework.
// It takes an input filename and an output filename, and finally
// a output project type. The type of project determines which
// files the project requires. The two types available now
// are rebuilt and interpreted.

// TODO:
// AppManifest.xml should be modified from here to.
// Add input parameters for project name, company name, version etc.

#if 0	//unused
static string integerToString(int i) {
	char temp[16];
	sprintf(temp, "%i", i);
	return temp;
}

static char *readFileIntoMem(const char* filename, int *len) {
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

static bool writeMemIntoFile(const char* filename, const char *mem, int len) {
	FILE *file = fopen(filename, "wb");
	if(!file) return false;
	fwrite(mem, len, 1, file);
	fclose(file);
	return true;
}
#endif

static void error(const char *why) GCCATTRIB(noreturn);
static void error(const char *why) {
	printf("error: %s\n", why);
	exit(1);
}

#if 0	//unused
static void replaceTemplateDefine(string &templateFile, const string &whatToReplace, const string &replacement) {
	size_t index;
	while((index=templateFile.find(whatToReplace))!=string::npos) {
		int endOfReplacement = index+whatToReplace.length();
		templateFile = templateFile.substr(0, index) + replacement + templateFile.substr(endOfReplacement, templateFile.size()-endOfReplacement);
	}
}
#endif

static pugi::xml_node getNode(const pugi::xml_node& node, const std::string& xpath)
{
	pugi::xpath_query query(xpath.c_str());
	return node.select_single_node(query).node();
}

struct LibraryReference
{
	LibraryReference(const std::string& name, const std::string& value) :
		name(name),
		value(value)
	{
	}

	std::string name;
	std::string value;
};

int main(int argc, char **argv) {

	string inputFile = "";
	string outputType = "rebuilt";
	string outputFile = "";

	std::vector<LibraryReference> libraryReferences;

	bool extensionDevMode = false;
	string releaseInterpretedConfig = " '$(Configuration)|$(Platform)' == 'Release|AnyCPU' ";
	string debugInterpretedConfig = " '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' ";
	string releaseRebuildConfig = "'$(Configuration)|$(Platform)' == 'rebuild_release|AnyCPU'";
	string debugRebuildConfig = "'$(Configuration)|$(Platform)' == 'rebuild_debug|AnyCPU'";
	string programFileRelativePath = "program";
	string resourceFileRelativePath = "resources";
	string dataSectionFileRelativePath = "RebuildData\\data_section.bin";
	string rebuildCsFileRelativePath = "RebuildData\\rebuild.build.cs";
	string projectName = "mosync";
	string runtimePath = "Libraries\\mosyncRuntime\\mosyncRuntime.dll";

	bool hasResourceFile = true;

	for(int i = 1; i < argc; i++) {
		if(strcmp("-build-extension-devenv", argv[i])==0) {
			extensionDevMode = true;
		} else
		if(strcmp("-project-name", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -project-name.");
			projectName = argv[i];
		} else
		if(strcmp("-runtime-path", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -runtime-path.");
			runtimePath = argv[i];
		} else
		if(strcmp("-exclude-resource-file", argv[i])==0) {
			hasResourceFile = false;
		} else
		if(strcmp("-ref", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -ref <name> <path>.");
			std::string name = argv[i];
			i++;
			if(i>=argc) error("Invalid argument to -ref <name> <path>.");
			std::string value = argv[i];

			libraryReferences.push_back(LibraryReference(name, value));
		} else
		if(strcmp("-input-file", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -input.");
			inputFile = argv[i];
		} else
		if(strcmp("-output-type", argv[i])==0) {
				i++;
				if(i>=argc) error("Invalid argument to -output-type.");
				outputType = argv[i];
		} else
		if(strcmp("-output-file", argv[i])==0) {
				i++;
				if(i>=argc) error("Invalid argument to -output-file.");
				outputFile = argv[i];
		}
	}

	if(inputFile=="")
		error("Must specify an input file using -input-file.");
	if(outputType!="rebuilt" && outputType!="interpreted")
		error("Must specify an output type using -output-type. It can either be rebuild or interpreted.");
	if(outputFile=="")
		error("Must specify an output file using -output-file.");

	pugi::xml_document document;
	pugi::xml_parse_result res = document.load_file(inputFile.c_str());
	if(res.status != pugi::status_ok) {
		error("Xml parsing failed!");
		return 0;
	}


	pugi::xpath_query projectQuery("Project");
	pugi::xml_node project = document.select_single_node(projectQuery).node();
	std::string version = project.attribute("ToolsVersion").value();

	if(!extensionDevMode)
	{
		if(outputType == "rebuilt")
		{
			pugi::xml_node relCnfNode = getNode(project, "PropertyGroup[@Condition=\"" + releaseInterpretedConfig + "\"]");
			pugi::xml_node dbgCnfNode = getNode(project, "PropertyGroup[@Condition=\"" + debugInterpretedConfig + "\"]");
			pugi::xml_node programContentNode = getNode(project, "ItemGroup/Content[@Include=\"" + programFileRelativePath + "\"]");
			relCnfNode.parent().remove_child(relCnfNode);
			dbgCnfNode.parent().remove_child(dbgCnfNode);
			programContentNode.parent().remove_child(programContentNode);

			if(!hasResourceFile)
			{
				pugi::xml_node resContentNode = getNode(project, "ItemGroup/Content[@Include=\"" + resourceFileRelativePath + "\"]");
				resContentNode.parent().remove_child(resContentNode);
			}
		}
		else if(outputType == "interpreted")
		{
			pugi::xml_node relRebuildCnfNode = getNode(project, "PropertyGroup[@Condition=\"" + releaseRebuildConfig + "\"]");
			pugi::xml_node dbgRebuildCnfNode = getNode(project, "PropertyGroup[@Condition=\"" + debugRebuildConfig + "\"]");
			pugi::xml_node dataSectionContentNode = getNode(project, "ItemGroup/Content[@Include=\"" + dataSectionFileRelativePath + "\"]");
			pugi::xml_node rebuildCsIncludeNode = getNode(project, "ItemGroup/Compile[@Include=\"" + rebuildCsFileRelativePath + "\"]");
			relRebuildCnfNode.parent().remove_child(relRebuildCnfNode);
			dbgRebuildCnfNode.parent().remove_child(dbgRebuildCnfNode);
			dataSectionContentNode.parent().remove_child(dataSectionContentNode);
			rebuildCsIncludeNode.parent().remove_child(rebuildCsIncludeNode);

			if(!hasResourceFile)
			{
				pugi::xml_node resContentNode = getNode(project, "ItemGroup/Content[@Include=\"" + resourceFileRelativePath + "\"]");
				resContentNode.parent().remove_child(resContentNode);
			}
		}
	}

/*
    <Reference Include="WriteableBitmapExWinPhone">
      <HintPath>Libraries\WriteableBitmapEx_v0.9.7.0\WindowsPhone\WriteableBitmapExWinPhone.dll</HintPath>
    </Reference>
*/

	// insert a reference to the mosync runtime dll
	pugi::xml_node firstReferenceNode = getNode(project, "ItemGroup/Reference[@Include]");
	pugi::xml_node mosyncRuntimeRefNode = firstReferenceNode.parent().insert_child_before("Reference", firstReferenceNode);
	pugi::xml_attribute mosyncRuntimeRefAttrNode = mosyncRuntimeRefNode.append_attribute("Include");
	mosyncRuntimeRefAttrNode.set_value("mosyncRuntime");
	pugi::xml_node mosyncRuntimeHintPath = mosyncRuntimeRefNode.append_child("HintPath").append_child(pugi::node_pcdata);
	mosyncRuntimeHintPath.set_value(runtimePath.c_str());

	for(size_t i = 0; i < libraryReferences.size(); i++)
	{
		pugi::xml_node firstReferenceNode = getNode(project, "ItemGroup/Reference[@Include]");
		pugi::xml_node refNode = firstReferenceNode.parent().insert_child_before("Reference", firstReferenceNode);
		pugi::xml_attribute refAttrNode = refNode.append_attribute("Include");
		refAttrNode.set_value(libraryReferences[i].name.c_str());
		pugi::xml_node hintPath = refNode.append_child("HintPath").append_child(pugi::node_pcdata);
		hintPath.set_value(libraryReferences[i].value.c_str());
	}

	// remove a reference to the mosync runtime project
	pugi::xml_node mosyncRuntimeProjectReferenceNode = getNode(project, "ItemGroup/ProjectReference[@Include=\"..\\mosyncRuntime\\mosyncRuntime.csproj\"]");
	mosyncRuntimeProjectReferenceNode.parent().remove_child(mosyncRuntimeProjectReferenceNode);

	pugi::xml_node rootNameSpaceNode = getNode(project, "PropertyGroup/RootNamespace");
	pugi::xml_node assemblyNameNode = getNode(project, "PropertyGroup/AssemblyName");
	rootNameSpaceNode.first_child().set_value(projectName.c_str());
	assemblyNameNode.first_child().set_value(projectName.c_str());

	pugi::xml_node decl = document.prepend_child(pugi::node_declaration);
	decl.append_attribute("version").set_value("1.0");
	decl.append_attribute("encoding").set_value("utf-8");
	if(!document.save_file(outputFile.c_str(), " ", 1U, pugi::encoding_utf8))
	{
		error("Xml writing failed!");
		return 0;
	}

	return 0;
}
