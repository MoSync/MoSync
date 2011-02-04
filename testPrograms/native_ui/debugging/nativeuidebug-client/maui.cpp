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

// connect this application to a maui-comm server.

#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUtil/Connection.h>
#include <MAUtil/util.h>
#include <MAUtil/Map.h>
#include <MAUI/Engine.h>
#include <conprint.h>

#include <IX_WIDGET.h>

//#include "MAHeaders.h"

using namespace MAUtil;
using namespace MAUI;

enum State {
	STATE_COMMAND,
	STATE_RESOURCES
};

class MAUIMoblet : public Moblet, public ConnectionListener  {
public:
    MAUIMoblet() : mConnection(this), state(STATE_COMMAND) {
        int res = mConnection.connect("socket://localhost:8080");
        if(res < 0) {
            maPanic(res, "mConnection.connect failed");
        }
		resourcePlaceholder = maCreatePlaceholder();
        // todo: initializtion
    }

    void keyPressEvent(int keyCode) {
        // todo: handle key presses
    }

    void keyReleaseEvent(int keyCode) {
        // todo: handle key releases
    }

    ~MAUIMoblet() {
    }

    void getData() {
 		int offset = buffer.size();
 		buffer.reserve(offset+1024);
         mConnection.recv(&buffer[offset], 1024);
     }

     void writeResponse(const char *c, int len) {
     	mConnection.write(c, len);
     }

     void connectFinished(Connection* conn, int result) {
          if(result < 0) {
              printf("mConnection.connectFinished failed");
              return;
          }
          getData();
      }

     void connWriteFinished(Connection* conn, int result) {
         if(result < 0) {
             printf("mConnection.write failed");
             return;
         }
         getData();
     }

     MAHandle getWidget(const String& id) {
     	Map<String, MAHandle>::Iterator w = widgets.find(id);
     	if(w==widgets.end()) maPanic(0, "Widget id doesn't exist.");
     	return w->second;
     }

     void printWidgetIds() {
 		printf("printing keys:");
 		Map<String, MAHandle>::Iterator i = widgets.begin();
 		for(;i!=widgets.end(); i++)  {
 			printf("'%s'", i->first.c_str());
 		}
     }

 	void recieveResources() {
 		state = STATE_RESOURCES;
 		resourceDataCreated = false;
 		currentResourceSize = 0;
 		getData();
 	}

 	bool loadResources() {
 		if(resourceDataCreated == false) {
 			if(buffer.size() < 4) return false;
 			resourceSize =
 				((int)buffer[0])|
 				(((int)buffer[1])<<8)|
 				(((int)buffer[2])<<16)|
 				(((int)buffer[3])<<24);

 			maCreateData(resourcePlaceholder, resourceSize);
 			maWriteData(resourcePlaceholder, &buffer[4], currentResourceSize, buffer.size());
 			currentResourceSize+=buffer.size()-4;
 		} else {
 			maWriteData(resourcePlaceholder, &buffer[0], currentResourceSize, buffer.size());
 			currentResourceSize+=buffer.size();
 		}

 		if(currentResourceSize!=resourceSize) return false;
 		else {
 			maLoadResources(resourcePlaceholder);
 			maDestroyObject(resourcePlaceholder);
 			return true;
 		}
 	}

     void executeCommands(const Vector<String>& commands) {
     	if(commands.size() < 1) return;

     	if(commands[0] == "widgetCreate") {
     		if(commands.size() != 3) return; // addWidget type id
     			printf("id: '%s'", commands[2].c_str());
     			widgets[commands[2]] = maWidgetCreate(commands[1].c_str());
     			printWidgetIds();
     	} else
      	if(commands[0] == "setProperty") {
     		if(commands.size() != 4) return; // setParam id name value
    			printf("setting param of widget with id");
     		printf("id: '%s'", commands[1].c_str());
     		printWidgetIds();
     		MAHandle widget = getWidget(commands[1]);
     		maWidgetSetProperty(widget, commands[2].c_str(), commands[3].c_str());
      	} else
      	if(commands[0] == "addChild") {
     		if(commands.size() != 3) return; // addChild idParent idChild
     		MAHandle parent = getWidget(commands[1]);
     		MAHandle child = getWidget(commands[2]);
     		maWidgetAddChild(parent, child);
      	}
      	else if(commands[0] == "screenShow") {
     		if(commands.size() != 2) return; // addChild idParent idChild
     		MAHandle screen = getWidget(commands[1]);
     		maWidgetScreenShow(screen);
      	}

     	/*
     	if(commands[0] == "loadResources") {
 			writeResponse("done", 4);
 			recieveResources();
 		}
 		*/
     }

     bool parseCommands() {
     	Vector<String> commands;
 		buffer.resize(buffer.size()+1);
     	buffer[buffer.size()-1] = 0;
 		buffer.resize(buffer.size()-1);

     	int i = 0;
     	String s((char*)&buffer[0]);
     	if((i=s.find(";")) == String::npos) return false;
     	s[i] = 0;
     	printf("%s", s.c_str());
     	stringSplit(s, " ", commands);

        	for(int i = 0; i < commands.size(); i++) {
        		printf("'%s'", commands[i].c_str());
 			commands[i].resize(strlen(commands[i].c_str()));
        	}

        	executeCommands(commands);

        	return true;
     }

     void connRecvFinished(Connection* conn, int result) {
         if(result < 0) {
             printf("mConnection.recv failed");
             return;
         }

 		buffer.resize(buffer.size()+result);

 		int done = 0;

 		if(state == STATE_COMMAND) {
 			if(parseCommands()) done = 1;

 		} else if(state == STATE_RESOURCES) {
 			if(loadResources()) done = 1;
 		}

 		if(done) {
 			writeResponse("done", 4);
 			buffer.clear();
 		} else {
 			getData();
 		}
     }

 private:
 	Vector<byte> buffer;
 	Connection mConnection;

 	Map<String, MAHandle> widgets;

 	State state;
 	MAHandle resourcePlaceholder;
 	bool resourceDataCreated;
 	int currentResourceSize;
 	int resourceSize;

};

extern "C" int MAMain() {
	gConsoleDisplay = 0;
    Moblet::run(new MAUIMoblet());
    return 0;
};
