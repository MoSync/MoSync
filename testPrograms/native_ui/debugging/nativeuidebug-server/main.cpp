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

#include <SDL/SDL_net.h>
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <fstream>



// for cwd
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>

#endif

using namespace std;

string workingdir;

string integerToString(int i) {
	char temp[16];
//	itoa(i, temp, 10);
	sprintf(temp, "%d", i);
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

void replaceTemplateDefine(string &templateFile, const string &whatToReplace, const string &replacement) {
	size_t index;
	while((index=templateFile.find(whatToReplace))!=string::npos) {
		int endOfReplacement = index+whatToReplace.length();
		templateFile = templateFile.substr(0, index) + replacement + templateFile.substr(endOfReplacement, templateFile.size()-endOfReplacement);
	}
}

void error(const char *s) {
	printf("%s\n", s);
}

enum {
	RT_IMAGE,
	RT_BINARY
};
struct Resource {
	int type;
	string id;
	string name;
	string source;
};

vector<Resource> gResources;
map<string, string> gResourceHandles;
string mosyncdir;

string gBmFontFile("bmfont.bmfc.template");

bool generateFont(const string& id, const string& fontType, const string& fontSize, const string& fontColor) {
	static int handle = 0;
	int len;

	string resourceName = "FONT" + integerToString(handle++);


	char *fontTemplate = readFileIntoMem(gBmFontFile.c_str(), &len);
	string fontTemplateStr = string(fontTemplate,len);
	delete fontTemplate;

	replaceTemplateDefine(fontTemplateStr, "%fontType%", fontType);
	replaceTemplateDefine(fontTemplateStr, "%fontSize%", fontSize);

	writeMemIntoFile("bmfont.bmfc", fontTemplateStr.c_str(), fontTemplateStr.length());

	string fntOutput = resourceName + ".fnt";
	string pngOutput = resourceName + "_00.png";
	string commandLine = (mosyncdir + "\\bin\\BMFont\\bmfont -c bmfont.bmfc -o " + fntOutput);
	printf("font generator cmd: %s\n", commandLine.c_str());

	if(system(commandLine.c_str()) != 0) {
		error("font couldn't be generated.");
		return false;
	}

	string mofOutput = resourceName + ".mof";
	commandLine = mosyncdir + "\\bin\\mof -fontData " + fntOutput + " -fontImage " + pngOutput + " -outFile " + mofOutput + " -fontColor " + fontColor;
	printf("mof generator cmd: %s\n", commandLine.c_str());
	if(system(commandLine.c_str()) != 0) {
		error("font couldn't be generated.");
		return false;
	}

	int s = gResources.size();
	gResources.resize(s+1);
	gResources[s].name = resourceName;
	gResources[s].source = mofOutput;
	gResources[s].type = RT_BINARY;
	gResources[s].id = id;
	return true;
}

TCPsocket initListeningSocket(int port) {
	TCPsocket sd; /* Socket descriptor, Client socket descriptor */
	IPaddress ip;
 
	/* Resolving the host using NULL make network interface to listen */
	if (SDLNet_ResolveHost(&ip, NULL, port) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	return sd;
}

void closeListeningSocket(TCPsocket sd) {
	SDLNet_TCP_Close(sd);	
}

void splitCommands(vector<string>& commands, const char* src) {
	char *lastCommand;
	char *src = command;
	while(*src) {
		while(*src && *src==' ') src++;
		if(!*src) break;
		lastCommand = src;
		while(*src && *src!=' ') src++;
		if(*src) {
			*src = 0;
			src++;
		}
		commands.push_back(lastCommand);
	}
}

bool recieveCommands(vector<string>& commands) {
	printf(">");
	char command[256];
	gets(command);
	if(strncmp(command, "exit", 4)==0 || strncmp(command, "quit", 4)==0) return false;
	commands.clear();	
	splitCommands(commands, command);
	return true;
}

bool recieveDone(TCPsocket client) {
	char recvBuf[256];
	SDLNet_TCP_Recv(client, recvBuf, 256);
	if(strncmp(recvBuf, "done", 4) != 0) {
		//printf("Something went wrong with the response.\n");
		return false;
	}
	return true;
}

void buildResources() {
	fstream stream;
	stream.open("res.lst", ios::out);
	
	gResourceHandles.clear();
	for(size_t i = 0; i < gResources.size(); i++) {
		stream << ".res " << gResources[i].name << "\n";

		switch(gResources[i].type) {
			case RT_IMAGE: 
				stream << ".image \"" << gResources[i].source << "\"\n";
				break;
			case RT_BINARY:
				stream << ".bin" << "\n";
				stream << ".include \"" << gResources[i].source << "\"\n";
				break;
		}
		stream << "\n";
		gResourceHandles[gResources[i].id] = integerToString(i);
	}
	stream.close();

	string cmd = mosyncdir + "/bin/pipe-tool -R resources " + workingdir + "/res.lst";
	printf("%s", cmd.c_str());
	if(system(cmd.c_str())!=0) {
		error("pipe-tool failed");
	}

}

void sendResources(TCPsocket client) {
}

bool loadScript(vector<string>& commands, const string& filename) {
	FILE *file = fopen(filename.c_str(), "r");
	if(!file) return false;

	commands.clear();
	char command[256];		
	while(fgets(temp, 256, file)) {
		splitCommands(commands, command); 
	}
	
	if(feof(file))
		return true;
	else
		return false;
}

bool mainLoop(TCPsocket sd) {
	TCPsocket client;
	IPaddress *remoteIP;
	vector<string> commands;

	printf("waiting for client..\n");

	while(!(client = SDLNet_TCP_Accept(sd))) {};

	{
			if ((remoteIP = SDLNet_TCP_GetPeerAddress(client))) {
				printf("connected to client..\n");
				while(recieveCommands(commands)) {
					if(commands.size()>=1) {
						if(commands[0] == "loadScript") {
							if(commands.size()!=2) printf("insufficient amount of parameters to loadScript ");
							if(!loadScript(commands, commands[1])) {
								printf("Couldn't load script.");
							}
							
						}
						else if(commands[0] == "setFont") {
							if(commands.size()!=3) printf("insufficent amount of paremeters to setFont\n");
							else {
								// setFont id fontId
								commands.resize(4);
								commands[0] = "setParam";
								commands[3] = gResourceHandles[commands[2]];
								commands[2] = "font";
							}
						} else
						if(commands[0] == "addFont") {
							if(commands.size()!=5) printf("insufficent amount of paremeters to setFont\n");
							// addFont id type size color
							generateFont(commands[1], commands[2], commands[3], commands[4]);
							commands.resize(1);
							commands[0] = "loadResources";
						}
					}

					for(size_t i = 0; i < commands.size(); i++) {
						SDLNet_TCP_Send(client, (void*)commands[i].c_str(),  commands[i].length());
						if(i!=commands.size()-1)
							SDLNet_TCP_Send(client, (void*)" ",  1);
					}
					SDLNet_TCP_Send(client, (void*)";",  1);

					if(!recieveDone(client)) {
						printf("Something went wrong with the response.\n");
					} else if(commands.size() >= 1) {
						if(commands[0] == "loadResources") {
							buildResources();
							sendResources(client);
							if(!recieveDone(client)) {
								printf("Something went wrong with the resource transfer\n");
							}
						}	
					}
				}
			}
	}
	return true;
}

int main(int argc, char **argv) {
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	TCPsocket sd = initListeningSocket(8080);

	const char *c= getenv("MOSYNCDIR");
	if(!c) { error("no MOSYNCDIR"); return 1; }
	mosyncdir = c;

	workingdir.resize(256);
#ifdef _WIN32
	::GetCurrentDirectoryA(256, &workingdir[0]);
#else
	 getcwd(&workingdir[0], 256);
#endif
	workingdir.resize(strlen(&workingdir[0]));

	while(mainLoop(sd)) {}

	closeListeningSocket(sd);
	
	SDLNet_Quit();

	return 0;
}