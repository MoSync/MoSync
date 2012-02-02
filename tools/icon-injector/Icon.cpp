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

#include "Icon.h"
#include <expat.h>
#include <stdio.h>
#include <cstring>
#include "Shared.h"

using namespace std;

namespace MoSync {

static int Depth = 0;
static Icon* retIcon = 0;
static string* platformState = 0;

static void start(void *data, const char *el, const char **attr) {
  int i;

  if(!strcmp(el, "icon")) {
	  //printf("We have an icon!");
	  retIcon = new Icon();
  }

  if(!strcmp(el, "instance")) {
	  std::string size;
	  std::string src;
	  const char* platform = NULL;
	  for (i = 0; attr[i]; i += 2) {
		 if(!strcmp(attr[i], "size"))
			 size = attr[i+1];
		 else if(!strcmp(attr[i], "src"))
			 src = attr[i + 1];
		 else if(!strcmp(attr[i], "platform"))
			 platform = attr[i + 1];
		//printf(" %s='%s'", attr[i], attr[i + 1]);
	  }
	  if(!retIcon)
		  errorExit("Broken XML! Instance element found outside of Icon element!");
	  if (!platform || !strcmp(platform, platformState->c_str())) {
		  retIcon->addInstance(size, src);
	  }
  }

  //printf("\n");
  Depth++;
}  /* End of start handler */

static void end(void *data, const char *el) {
  Depth--;
}  /* End of end handler */

	Icon* Icon::parse(const string& filename, string& platform) {
		char buf[1024];
		size_t len;   /* len is the number of bytes in the current bufferful of data */
		int done = 0;
		int depth = 0;  /* nothing magic about this; the sample program tracks depth to know how far to indent. */
					  /* depth is thus going to be the user data for this parser. */
		retIcon = 0;
		platformState = &platform;
		FILE* f = fopen(filename.c_str(), "r");

		if(!f) {
			errorExit((string("Could not open input file: ") + filename).c_str());
		}

		XML_Parser parser = XML_ParserCreate(NULL);
		XML_SetUserData(parser, &depth);
		XML_SetElementHandler(parser, start, end);
		//printf("about to parse...\n");

		do {
			/*get a piece of input into the buffer
			done = whether this bufferful is the last bufferful */
			len = fread(buf, 1, 1024, f);
			if (!XML_Parse(parser, buf, (int)len, done)) {
			  //handle parse error
				errorExit("XML parsing failed");
			}
			//if(len <= 0) done = true;
		} while (!feof(f));
		//printf("parsed\n");
		XML_ParserFree(parser);

		string absolutePath = getAbsolutePath(filename);
		vector<IconInstance*>& mInstances = retIcon->getInstances();
		for(size_t i = 0; i < mInstances.size(); i++) {
#ifdef WIN32
			mInstances[i]->filename = absolutePath + "\\" + mInstances[i]->filename;
#else
			mInstances[i]->filename = absolutePath + "/" + mInstances[i]->filename;
#endif
		}

		return retIcon;
	}

	void Icon::addInstance(const std::string& size, const std::string& filename) {
		mInstances.push_back(new IconInstance(size, filename));
	}

	std::vector<IconInstance*>& Icon::getInstances() {
		return mInstances;
	}

	const IconInstance* Icon::findBestInstance(const std::string& size,
		const std::string& ext) const
	{
		const IconInstance* res = 0;
		for(vector<IconInstance*>::const_iterator i = mInstances.begin(); i != mInstances.end(); ++i) {
			if(ext.length() != 0 && getExtension((*i)->filename) != ext)
				continue;
			if((*i)->size == size) {
				res = *i;
				break;
			} else if((*i)->size == "default") {
				res = *i;
			}
		}
		return res;
	}
}
