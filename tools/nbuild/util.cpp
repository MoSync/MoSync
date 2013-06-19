/* Copyright (C) 2010 MoSync AB

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

#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <ostream>
#include <cerrno>
#include <sys/stat.h>
#include "util.h"
#include "filelist/filelist.h"

using namespace std;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static const int IOS_NAME_INDEX = ios_base::xalloc();

void setName(ios& i, const char* name) {
	i.pword(IOS_NAME_INDEX) = (void*)name;
}

void beGood(ios& i) {
	if(!i.good()) {
		printf("File I/O error in '%s'\n", (char*)i.pword(IOS_NAME_INDEX));
		exit(1);
	}
}

// writes a copy of in to out.
static void writeCopy(istream& in, ostream& out) {
	beGood(in);
	beGood(out);

	streamoff size;
	in.seekg(0, ios_base::end);
	size = in.tellg();
	in.seekg(0, ios_base::beg);
	beGood(in);

	streamoff pos = 0;
	while(pos < size) {
		char buffer[64*1024];	// arbitrary size
		streamoff todo = MIN(size - pos, (streamoff)sizeof(buffer));
		in.read(buffer, todo);
		beGood(in);
		out.write(buffer, todo);
		beGood(out);
		pos += todo;
	}
}

void copyFile(const char* dst, const char* src) {
	ifstream in(src, ios_base::binary);
	setName(in, src);
	ofstream out(dst, ios_base::binary);
	setName(out, dst);
	writeCopy(in, out);
}

void appendFile(const char* dst, const char* src) {
	ifstream in(src, ios_base::binary);
	setName(in, src);
	ofstream out(dst, ios_base::binary | ios_base::app);
	setName(out, dst);
	writeCopy(in, out);
}

bool existsFile(const char* filename) {
	ifstream file(filename);
	if (file) {
		file.close();
	}
	return file.good();
}

bool existsDir(const char* dir) {
	struct stat s;
	int res = stat(dir, &s);
	if(res == 0) {
		return (s.st_mode & S_IFDIR) != 0;
	}
	if(errno == ENOENT)
		return false;
	printf("stat(%s) failed: %i(%s)\n",
		dir, errno, strerror(errno));
	exit(1);
}

void renameFile(const string& dst, const string& src) {
	int _res = rename(src.c_str(), dst.c_str());
	if(_res != 0) {
		printf("rename(%s, %s) failed: %i %i(%s)\n",
			src.c_str(), dst.c_str(), _res, errno, strerror(errno));
		exit(1);
	}
}

streamoff getFileSize(const char* filename) {
	ifstream in(filename, ios_base::binary);
	setName(in, filename);
	beGood(in);

	streamoff size;
	in.seekg(0, ios_base::end);
	size = in.tellg();
	beGood(in);
	return size;
}

string readFileToString(const char* src) {
	ifstream in(src);
	setName(in, src);
	beGood(in);

	ostringstream oss;
	oss << in.rdbuf();
	beGood(in);
	return oss.str();
}

void* readBinaryFile(const char* src, size_t& size) {
	ifstream in(src, ios_base::binary);
	setName(in, src);
	beGood(in);

	in.seekg(0, ios_base::end);
	size = (size_t)in.tellg();
	in.seekg(0, ios_base::beg);
	beGood(in);

	void* buf = malloc(size);
	in.read((char*)buf, size);
	beGood(in);
	return buf;
}

void writeFile(const char* dst, const void* src, size_t size) {
	ofstream out(dst, ios_base::binary);
	setName(out, dst);
	beGood(out);
	out.write((char*)src, size);
	beGood(out);
}

void applyTemplate(const char* dst, const char* src, const TemplateMap& tm) {
	ifstream in(src);
	setName(in, src);
	beGood(in);
	ofstream out(dst);
	setName(out, dst);
	beGood(out);

	while(true) {
		int c = in.get();
		if(in.eof())
			break;
		if(c == '%') {	// template argument start marker
			int d = in.get();
			if(d == '%') {	//literal percent sign
				out << '%';
			} else {
				// find the end
				string tempName;
				while(!in.eof()) {
					if(d == '%') {	//template argument end marker
						break;
					} else if(isspace(d)) {
						printf("Broken template file (space in template).\n");
						exit(1);
					} else {
						tempName += (char)d;
					}
					d = in.get();
				}
				// apply the template argument
				TemplateMap::const_iterator itr = tm.find(tempName);
				if(itr == tm.end()) {
					printf("Missing template argument: %s\n", tempName.c_str());
					exit(1);
				}
				out << itr->second;
			}
		} else {
			out.put(c);
		}
	}
}

void toDir(std::string& str) {
	if(str[str.size()-1] != '/')
		str += '/';
}

std::string getDir(const std::string& name) {
	size_t index = name.find_last_of('/');
#ifdef WIN32
	size_t bi = name.find_last_of('\\');
	if((bi > index || index == string::npos) && bi != string::npos)
		index = bi;
#endif
	return name.substr(0, index);
}

string fullpathString(const char* name) {
	char* res = fullpath(name);
	if(!res) {
		printf("Could not resolve path: '%s'\n", name);
		exit(1);
	}
	string s = res;
	free(res);
	return s;
}


std::string delim(std::vector<std::string>& input, const std::string& delim) {
	std::string result = std::string();
	for (size_t i = 0; i < input.size(); i++) {
		result.append(input.at(i));
		if (i < input.size() - 1) {
			result.append(delim);
		}
	}
	return result;
}

void split(vector<string>& out, const string& input, const string& delims) {
	string::size_type lastPos = input.find_first_not_of(delims, 0);
	string::size_type pos = input.find_first_of(delims, lastPos);

	while (string::npos != pos || string::npos != lastPos) {
		string part = input.substr(lastPos, pos - lastPos);
		out.push_back(part);
		lastPos = input.find_first_not_of(delims, pos);
		pos = input.find_first_of(delims, lastPos);
	}
}

void write72line(std::ostream& output, const std::string& input) {
	std::vector<std::string> splitLines = std::vector<std::string>();
	int len = 72;
	for (size_t i = 0; i < input.length(); i+= len) {
		len = i < 2 ? 72 : 71; // Don't forget the initial space
		splitLines.push_back(input.substr(i, len));
	}
	// Newline + first char of next must be space
	output << delim(splitLines, string("\n "));
}

string arg(const char* arg) {
	string result = string(arg);
	bool isQuoted = (result.size() > 2) &&
			(result.at(0) == '\"');
	if (!isQuoted && result.find(' ') != string::npos) {
		result = "\"" + result + "\"";
	}
	return result;
}

string arg(string argArg) {
	return arg(argArg.c_str());
}

void toOSSlashes(string& str) {
#ifdef WIN32
	toBackSlashes(str);
#else
	toSlashes(str);
#endif
}

string file(const char* argArg) {
	string argStr = string(argArg);
	toOSSlashes(argStr);
	return arg(argStr.c_str());
}

string file(std::string arg) {
	return file(arg.c_str());
}

void toSlashes(string& str) {
	for(size_t i=0; i<str.size(); i++) {
		str[i] = (str[i] == '\\' ? '/' : str[i]);
	}
}
void toBackSlashes(std::string& str) {
	for(size_t i=0; i<str.size(); i++) {
		str[i] = (str[i] == '/' ? '\\' : str[i]);
	}
}

bool streq(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}
