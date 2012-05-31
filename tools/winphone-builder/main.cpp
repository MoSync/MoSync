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
#include <sys/types.h>
#include <dirent.h>

#include "helpers/attribute.h"

using namespace std;

// The purpose of this program is to generate projects
// for Windows Phone using the same project as
// the one used when developing. This way the main project
// can be used as a template bundled with the mosync framework.
// It takes an input filename and an output filename, and finally
// an output project type. The type of project determines which
// files the project requires. The two types available now
// are rebuilt and interpreted.

// TODO:
// AppManifest.xml should be modified from here to.
// Add input parameters for project name, company name, version etc.

static void error(const char *why) GCCATTRIB(noreturn);
static pugi::xml_node getNode(const pugi::xml_node& node, const std::string& xpath);
static std::string createFileName(const std::string& name);
static bool saveXML(pugi::xml_document& document, const std::string& outputFile);
static bool updateWMAppManifest(const std::string& filename, const std::string& output, const std::string& projectName, const std::string& companyName, const std::string& version, const std::string& guid);

static void error(const char *why) {
	printf("error: %s\n", why);
	exit(1);
}

static pugi::xml_node getNode(const pugi::xml_node& node, const std::string& xpath)
{
	pugi::xpath_query query(xpath.c_str());
	return node.select_single_node(query).node();
}

struct LibraryReference
{
	LibraryReference(const std::string& _name, const std::string& _value) :
		name(_name),
		value(_value)
	{
	}

	std::string name;
	std::string value;
};

struct ApplicationBarIconReference
{
	ApplicationBarIconReference(const std::string _path) :
		iconPath(_path)
	{}

	std::string iconPath;
};

static std::string createFileName(const std::string& name)
{
	return name;
}

static bool saveXML(pugi::xml_document& document, const std::string& outputFile)
{
	pugi::xml_node decl = document.prepend_child(pugi::node_declaration);
	decl.append_attribute("version").set_value("1.0");
	decl.append_attribute("encoding").set_value("utf-8");
	if(!document.save_file(outputFile.c_str(), " ", 1U, pugi::encoding_utf8))
	{
		return false;
	}

	return true;
}

static bool updateWMAppManifest(const std::string& filename, const std::string& output, const std::string& projectName, const std::string& companyName, const std::string& version, const std::string& guid)
{
	pugi::xml_document document;
	pugi::xml_parse_result res = document.load_file(filename.c_str());
	if(res.status != pugi::status_ok) {
		error("WMAppManifest.xml parsing failed!");
		return false;
	}

	pugi::xpath_query appQuery("Deployment/App");
	pugi::xml_node app = document.select_single_node(appQuery).node();

	app.attribute("Title").set_value(projectName.c_str());
	app.attribute("Author").set_value(companyName.c_str());
	app.attribute("Publisher").set_value(companyName.c_str());
	app.attribute("Version").set_value(version.c_str());
	app.attribute("ProductID").set_value(("{" + guid + "}").c_str());

	pugi::xpath_query tileTitleQuery("Tokens/PrimaryToken/TemplateType5/Title");
	pugi::xml_node tileTitle = app.select_single_node(tileTitleQuery).node();
	tileTitle.first_child().set_value(projectName.c_str());

	if(!saveXML(document, output))
	{
		return false;
	}

	return true;
}

int main(int argc, char **argv) {

	string inputFile = "";
	string outputType = "rebuilt";
	string outputFile = "";

	std::vector<LibraryReference> libraryReferences;
	std::vector<ApplicationBarIconReference> applicationBarIconReferences;

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
	string companyName = "MoSync AB";
	string appVersion = "1.0";
	string guid = "2b77ba5b-5284-42e4-8fe3-dfa5d683553c";
	string inputAppManifestFile = ""; // defaults to none.
	string outputAppManifestFile = ""; // defaults to none.
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
		if(strcmp("-company-name", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -company-name.");
			companyName = argv[i];
		} else
		if(strcmp("-version", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -version.");
			appVersion = argv[i];
		} else
		if(strcmp("-guid", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -guid.");
			guid = argv[i];
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
		if(strcmp("-input-app-manifest-file", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -input-app-manifest-file.");
			inputAppManifestFile = argv[i]; // WMAppManifest.xml
		} else
		if(strcmp("-output-app-manifest-file", argv[i])==0) {
			i++;
			if(i>=argc) error("Invalid argument to -output-app-manifest-file.");
			outputAppManifestFile = argv[i]; // WMAppManifest.xml
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
		pugi::xml_node firstLibReferenceNode = getNode(project, "ItemGroup/Reference[@Include]");
		pugi::xml_node refNode = firstLibReferenceNode.parent().insert_child_before("Reference", firstLibReferenceNode);
		pugi::xml_attribute refAttrNode = refNode.append_attribute("Include");
		refAttrNode.set_value(libraryReferences[i].name.c_str());
		pugi::xml_node hintPath = refNode.append_child("HintPath").append_child(pugi::node_pcdata);
		hintPath.set_value(libraryReferences[i].value.c_str());
	}

	//add the application bar icons
	std::string dirPath;

	if("" != outputFile)
	{
		int position = outputFile.find("Output");
		if(-1 != position)
		{
			dirPath = outputFile.substr(0, position);
		}
		else
		{
			position = outputFile.find("FinalOutput");
			if(-1 != position)
			{
				dirPath = outputFile.substr(0, position);
			}
		}
		//create the output dir string for the icons "\ApplicationBarIcons\".
		dirPath.append("ApplicationBarIcons\\");
	}

	//create directory
	int position = outputFile.find("project");
	std::string outputDirPath;
	std::string command;
	command.append("mkdir \"");

	outputDirPath.append(outputFile.substr(0, position));
	command.append(outputFile.substr(0, position));

	outputDirPath.append("project\\AppBar.Icons\"");
	command.append("project\\AppBar.Icons\"");

	system(command.c_str());

	struct dirent* de = NULL;
	DIR* d = NULL;

	// Open the application bar icons directory and create an ApplicationBarIconReference obj for each.
	d = opendir(dirPath.c_str());
	if(NULL != d)
	{
        de = readdir(d);

		while(de)
		{
			// Ignore the . and .. entries
			if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
			{
				applicationBarIconReferences.push_back( ApplicationBarIconReference( de->d_name ) );
				std::string copyCmd;
				copyCmd.append("cp ");
				copyCmd.append(dirPath.c_str());
				copyCmd.append( de->d_name );
				copyCmd.append(" \"");
				copyCmd.append(outputDirPath);
				system(copyCmd.c_str());
			}
            de = readdir(d);
		}
	}
	closedir(d);

	// Write the changes to the csproj file.
	pugi::xml_node mosyncApplicationBarIcons = project.append_child("ItemGroup");

	while(applicationBarIconReferences.empty() == false)
	{
		pugi::xml_node contentChild = mosyncApplicationBarIcons.append_child("Content");
		std::string nodeValue;
		nodeValue.append("AppBar.Icons\\");
		nodeValue.append(applicationBarIconReferences.back().iconPath);
		contentChild.append_attribute("Include").set_value(nodeValue.c_str());
		contentChild.append_child("CopyToOutputDirectory").append_child(pugi::node_pcdata).set_value("PreserveNewest");

		applicationBarIconReferences.pop_back();
	}

	// remove a reference to the mosync runtime project
	pugi::xml_node mosyncRuntimeProjectReferenceNode = getNode(project, "ItemGroup/ProjectReference[@Include=\"..\\mosyncRuntime\\mosyncRuntime.csproj\"]");
	mosyncRuntimeProjectReferenceNode.parent().remove_child(mosyncRuntimeProjectReferenceNode);

	pugi::xml_node rootNameSpaceNode = getNode(project, "PropertyGroup/RootNamespace");
	pugi::xml_node assemblyNameNode = getNode(project, "PropertyGroup/AssemblyName");
	pugi::xml_node xapFileNameNode = getNode(project, "PropertyGroup/XapFilename");
	//pugi::xml_node silverlightAppEntryNode = getNode(project, "PropertyGroup/SilverlightAppEntry");

	rootNameSpaceNode.first_child().set_value(projectName.c_str());
	assemblyNameNode.first_child().set_value(projectName.c_str());
	xapFileNameNode.first_child().set_value((createFileName(projectName) + ".xap").c_str());
	//silverlightAppEntryNode.first_child().set_value((createFileName(projectName) + ".App").c_str());

	if(inputAppManifestFile != "" && outputAppManifestFile != "")
	{
		if(!updateWMAppManifest(inputAppManifestFile, outputAppManifestFile, projectName, companyName, appVersion, guid))
		{
			error("WMAppManifest.xml writing failed!");
			return 1;
		}
	}

	if(!saveXML(document, outputFile))
	{
		error("csproj xml writing failed!");
		return 1;
	}

	return 0;
}
