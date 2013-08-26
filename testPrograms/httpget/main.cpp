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
