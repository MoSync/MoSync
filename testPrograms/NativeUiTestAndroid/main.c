#include <ma.h>
#include <IX_NATIVE_UI.h>
#include <conprint.h>
#include <madmath.h>
#include <maheap.h>
#include <matime.h>
#include <maassert.h>

#define ACTIVITY_1 10
#define ACTIVITY_2 12


int MAMain() {

	maAndroidStartListActivity(10);


	char *str="hello";


	// Handle events
	MAEvent event;
	while(1) {
		while(maGetEvent(&event)) {
			if(event.type==EVENT_TYPE_NATIVE_UI_MSG) {
				if(event.lo_wparam==ACTIVITY_1) {
					/*maAndroidAddTextView(100, "I am a text view");
					maAndroidAddEditText(101, "I am an Edit Text");
					maAndroidAddButton(102, "I am a button");*/
					maAndroidAddListItem(701, "Item 1");
					maAndroidAddListItem(701, "Item 2");
/*
					maAndroidAddMenuItem(300, "New");
					maAndroidAddMenuItem(301, "Options");
					maAndroidAddMenuItem(302, "Favorites");
					maAndroidAddMenuItem(303, "Search");*/

				}
				if(event.lo_wparam==ACTIVITY_2) {
					maAndroidAddTextView(200, "Enter something below:");
					maAndroidAddEditText(201, "Hello");
					maAndroidAddButton(202, "Hello");
/*
					maAndroidAddMenuItem(400, "Item 1");
					maAndroidAddMenuItem(401, "Item 2");*/
				}


				if(event.lo_wparam==102) {
					maAndroidSetText(101, str);
					maAndroidAddTextView(500, "Added dynamically");
					maAndroidAddButton(501, "Added dynamically");
					maAndroidAddEditText(502, "Added dynamically");
					maAndroidStartActivity(ACTIVITY_2);
				}
			}
		}
		//maWait(0);
		//maWait(0);
	}
	//FREEZE;
	return 0;
}
