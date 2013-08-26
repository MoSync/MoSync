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

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tinyxml.h"


bool addDirectory(TiXmlNode *node, std::string platform, std::string directory) {
	std::string directories = std::string(node->Value());
	if(directories.find(directory)!=std::string::npos) return false;

	size_t i = directories.find(platform, 0);
	size_t i2 = directories.find("|", i+platform.length()+1);
	if(i2==std::string::npos) {
		i2 = directories.length();
	}

	std::string newDirectory = ";" + directory;
	directories.insert(i2, newDirectory);
	node->SetValue(directories.c_str());
	return true;
}

bool removeDirectory(TiXmlNode *node, std::string platform, std::string directory) {
	std::string directories = std::string(node->Value());
	size_t i = directories.find(directory);
	if(i==std::string::npos) return false;
	if((i-1>=0)&&directories[i-1]==';') directories.erase(i-1, directory.length()+1);
	else directories.erase(i, directory.length());

	node->SetValue(directories.c_str());
	return true;
}

int findAttrib(TiXmlElement* pElement, const char *attrib)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	while (pAttrib)
	{
		if(strcmp(pAttrib->Value(), attrib)==0) {
			return 1;
		}
		pAttrib=pAttrib->Next();
	}
	return 0;	
}

TiXmlNode* recursiveSearch( TiXmlNode* pParent,  const char *attrib)
{
	if ( !pParent ) return NULL;

	TiXmlNode* pChild;
	int t = pParent->Type();

	switch ( t )
	{
	case TiXmlNode::ELEMENT:
		if(	pParent->Parent() && pParent->Parent()->ToElement() &&
			pParent->Parent()->ToElement()->Attribute("name")&&
			strcmp(pParent->Parent()->ToElement()->Attribute("name"), "VCDirectories")==0) { 

			if(findAttrib(pParent->ToElement(), attrib)) {
				return pParent->FirstChild();
			}
		}

		break;
	}
//	printf( "\n" );
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
	{
		TiXmlNode *res = recursiveSearch( pChild, attrib);
		if(res) {
			return res;
		}
	}
}

enum eModType {
	MT_ADD,
	MT_REMOVE
};

int modifyConfigFile(std::string fileName, eModType modType, std::string type, std::string platform, std::string directory){
		TiXmlDocument doc(fileName.c_str());
		bool fileLoaded = doc.LoadFile();
		if(fileLoaded) {
			printf("this configure file was loaded: %s\n\n", fileName.c_str());	
			TiXmlNode *n = recursiveSearch(&doc, type.c_str());
			if(n) {
				switch(modType) {
					case MT_ADD: addDirectory(n, platform, directory); break;
					case MT_REMOVE: removeDirectory(n, platform, directory); break;
					default:
						break;
				}
				printf("%s: %s\n\n", type.c_str(), n->Value());
				bool fileSaved = doc.SaveFile();
				if(!fileSaved) {
					printf("File could not be saved\n");
					return 0;
				}
			}
		}

		return 1;
}

int main(int argc, char **argv) {
	DWORD size = 256;
	bool foundVcExpressDir = false;
	bool foundVsDir = false;
	char vcExpressDir[256];
	char vsDir[256];
	LONG res;
	eModType modType = MT_ADD;
	std::string platform = "Win32";
	std::string directory = "";
	std::string type = "";
	std::string userProfile = getenv("USERPROFILE");
	HKEY id;

	if(argc<=1) {
		printf(	"*----------------------------------------------------------------*\n"
				"|                                                                |\n"
				"| Visual Studio Default Directory Modifier                       |\n"
				"| (c) Niklas Nummelin / Mobile Sorcery                           |\n"
				"|                                                                |\n"
				"|  -type <string>  which type (tag) to modify  (required)        |\n"
				"|  -directory <string> which directory to add (required)         |\n"
				"|  -platform <string> which platform to add to (default: Win32)  |\n"
				"|  -remove  enable removal of directory (default: not enabled)   |\n"
				"|                                                                |\n"
				"*----------------------------------------------------------------*\n");
		return 0;
	}

	for(int i = 0; i < argc; i++) {
		if(std::string(argv[i])=="-remove") {
			modType = MT_REMOVE;
		}
		else if(std::string(argv[i])=="-platform") {
			i++;
			if(i>=argc) return 1;
			platform = std::string(argv[i]);
		}
		else if(std::string(argv[i])=="-type") {
			i++;
			if(i>=argc) return 1;
			type = std::string(argv[i]);
		}
		else if(std::string(argv[i])=="-directory") {
			i++;
			if(i>=argc) return 1;
			directory = std::string(argv[i]);
		}
	}

	if(directory.length()<1) {
		printf("No directory specified.\n");
		return 1;
	}

	if(type.length()<1) {
		printf("No type specified.\n");
		return 1;
	}

	res = RegOpenKey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\VCExpress\\8.0", &id);
	if(res == ERROR_SUCCESS) {
		res = RegQueryValueEx(id, "InstallDir", NULL, NULL, (byte*)vcExpressDir, &size);
		if(res == ERROR_SUCCESS) {
			printf("Found vc express dir: %s\n", vcExpressDir);
			RegCloseKey(id);
			foundVcExpressDir = true;
		}
	}

	res = RegOpenKey(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\VisualStudio\\8.0", &id);
	if(res == ERROR_SUCCESS) {
		res = RegQueryValueEx(id, "InstallDir", NULL, NULL, (byte*)vsDir, &size);
		if(res == ERROR_SUCCESS) {
			printf("Found visual studio dir: %s\n", vsDir);
			RegCloseKey(id);
			foundVsDir = true;
		}
	}
	

	if(foundVcExpressDir) {
		std::string fileName = userProfile+"\\My Documents\\Visual Studio 2005\\Settings\\C++ Express\\CurrentSettings.vssettings";
		modifyConfigFile(fileName, modType, type, platform, directory);
/*
		fileName = std::string(vcExpressDir) + "..\\..\\VC\\Profile\\VCExpress.vssettings";
		modifyConfigFile(fileName, modType, type, platform, directory);
*/
	}

	if(foundVsDir) {
		std::string fileName = userProfile+"\\My Documents\\Visual Studio 2005\\Settings\\CurrentSettings.vssettings";
		modifyConfigFile(fileName, modType, type, platform, directory);
/*
		fileName = std::string(vsDir) + "Profiles\\VC.vssettings";
		modifyConfigFile(fileName, modType, type, platform, directory);
		fileName = std::string(vsDir) + "Profiles\\General.vssettings";
		modifyConfigFile(fileName, modType, type, platform, directory);
*/
	}

	return 0;
}