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
#include <MAUtil/HttpGet.h>
#include "MAHeaders.h"

extern "C" int MAMain() {
	int res;

	InitConsole();

	printf("CreateData\n");
	maCreateData(RES, 386);
	printf("DestroyObject\n");
	maDestroyObject(RES);

	printf("HTTP GET\n");
	//res = myHttpGetToObject(1, "http://dev.mopix.se:8080/testing/mobile.php");
	{
		MAUtil::HttpGetToObject http("http://link.astando.se/astwms/Request.aspx?REQUEST=GetMap&SERVICE=WMS&VERSION=1.1.1&LAYERS=TO_Vattenytor,TO_Oppen_mark,TO_Byggnadsytor,TO_Vagkonturer,Vagkanter,TO_Geografiska_texter,TO_Allm_byggnadsnamn,Gatunamn&STYLES=&FORMAT=png&BGCOLOR=0xFF0000&TRANSPARENT=TRUE&SRS=EPSG:3152&BBOX=78202.0228224145,99374.2252203757,79546.909389597,100413.827113304&WIDTH=240&HEIGHT=188&reaspect=false", RES);
		res = http.state();
		while(res == 0) {
			EVENT event;
			maWait(0);
			while(maGetEvent(&event)) {
				if(event.type == EVENT_TYPE_CLOSE ||
					(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
				{
					maExit(0);
				} else if(event.type == EVENT_TYPE_CONN) {
					printf("cE %i %i %i\n", event.conn.handle, event.conn.opType, event.conn.result);
					http.notify(event.conn);
					res = http.state();
				}
			}
		}
	}
	printf("Got %i\n", res);
	FREEZE;
}
