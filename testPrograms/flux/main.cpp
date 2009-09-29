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

#include <ma.h>
#include <conprint.h>
#include <maassert.h>
#include "MAHeaders.h"

//extern "C" int MAMain() {
	//Create an image

	//options on the image:
	//set it as draw target
	//attempt to destroy it, then recreate	//should crash if set
	//attempt to draw it	//should crash if set
	//restore it

	//create a conn to a silent server and start reading to data object
	//options:

	//destroy data (flux)	//should crash if reading
	//getdatasize	//should crash if reading
	//cancel read
//}

void menu();

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	maCreateDrawableImage(IMAGE, 32, 32);
	Handle conn = -1;
	maCreateData(DATA, 32);
	bool imageIsTarget = false;

	while(true) {
		maWait(0);
		EVENT event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				maExit(0);
			if(event.type == EVENT_TYPE_CONN) {
				//MYASSERT(event.conn.handle == conn);
				if(event.conn.opType == CONNOP_CONNECT) {
					printf("Connect %i result %i\n", event.conn.handle, event.conn.result);
					if(event.conn.result > 0)
						maConnReadToData(conn, DATA, 0, 32);
				} else if(event.conn.opType == CONNOP_READ) {
					printf("Read %i result %i\n", event.conn.handle, event.conn.result);
					if(event.conn.result > 0)
						maConnReadToData(conn, DATA, 0, 32);
				} else {
					BIG_PHAT_ERROR;
				}
			}
			if(event.type == EVENT_TYPE_KEY_PRESSED) switch(event.key) {
			case MAK_0:
				maExit(0);

			//options on the image:
			case MAK_1:	//set it as draw target
				if(!imageIsTarget)
					printf("SetDrawTarget(IMAGE)\n");
				maSetDrawTarget(IMAGE);
				imageIsTarget = true;
				break;
			case MAK_2:	//attempt to destroy it, then recreate	//should crash if set
				if(!imageIsTarget)
					printf("DestroyObject(IMAGE)\n");
				maDestroyObject(IMAGE);
				maCreateDrawableImage(IMAGE, 32, 32);
				break;
			case MAK_3:	//attempt to draw it	//should crash if set
				if(!imageIsTarget)
					printf("DrawImage\n");
				maDrawImage(IMAGE, 0, 0);
				break;
			case MAK_4:	//restore it
				maSetDrawTarget(HANDLE_SCREEN);
				imageIsTarget = false;
				printf("SetDrawTarget(SCREEN)\n");
				break;

				//conn
			case MAK_6:	//connect
				if(!imageIsTarget)
					printf("Connect\n");
				conn = maConnect("socket://localhost:1234");
				break;
			case MAK_7:	//destroy data (flux)	//should crash if reading
				if(!imageIsTarget)
					printf("DestroyObject(DATA)\n");
				maDestroyObject(DATA);
				maCreateData(DATA, 32);
				break;
			case MAK_8:	//getdatasize	//should crash if reading
				if(!imageIsTarget)
					printf("GetDataSize\n");
				maGetDataSize(DATA);
				break;
			case MAK_9:	//cancel read
				if(!imageIsTarget)
					printf("ConnCancel\n");
				maConnClose(conn);
				break;
			}	//if switch
		}	//while(maGetEvent)
	}	//while(true)
}
