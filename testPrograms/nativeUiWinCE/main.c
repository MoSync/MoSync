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
#include <maassert.h>
#include <IX_NATIVE_UI.h>

#define MY_BUTTON 101
#define MY_EDIT 102
#define BN_CLICKED 0 // Event IDs, see winuser.h
#define LEFT_BUTTON 1
#define ITEM1_ID 105
#define ITEM2_ID 106

int MAMain() {

	// Build a button
	MAWidgetParameters buttonParams;
	buttonParams.pParent=NULL;
	buttonParams.posX=30;
	buttonParams.posY=30;
	buttonParams.sizeX=50;
	buttonParams.sizeY=50;
	buttonParams.widgetID=MY_BUTTON;
	MAWidgetHandle buttonHandle;
	maButton(&buttonParams, &buttonHandle);

	// Build an edit
	MAWidgetParameters editParams;
	editParams.pParent=NULL;//mywin_out.hwnd;
	editParams.posX=100;
	editParams.posY=100;
	editParams.sizeX=200;
	editParams.sizeY=200;
	editParams.widgetID=MY_EDIT;
	MAWidgetHandle editHandle;
	maEdit(&editParams, &editHandle);

	// Build the menu bar
	maMenuBar();
	MAWidgetParameters leftButtonParams;
	char *str1="Exit";
	strncpy(leftButtonParams.buf, str1, strlen(str1));
	maSetLeftButton(&leftButtonParams, NULL);

	MAWidgetParameters item1Params;
	char *str3="Exit";
	strncpy(item1Params.buf, str3, strlen(str3));
	item1Params.widgetID=ITEM1_ID;
	maAddRightMenuItem(&item1Params, NULL);

	MAWidgetParameters item2Params;
	char *str4="New window";
	strncpy(item2Params.buf, str4, strlen(str4));
	item2Params.widgetID=ITEM2_ID;
	maAddRightMenuItem(&item2Params, NULL);









	// Handle events
	MAEvent event;
	do {
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_NATIVE_UI_MSG) {

					switch(event.lo_wparam) {
						case MY_BUTTON:
							if(event.hi_wparam==BN_CLICKED) {

									MAWidgetParameters dlgParams;
									char *str2="Salut ca va ?";
									strncpy(dlgParams.buf, str2, strlen(str2));
									MAWidgetHandle dlgHandle;
									maMessageBox(&dlgParams, &dlgHandle);


							}
							break;


						case LEFT_BUTTON:
							if(event.hi_wparam==BN_CLICKED) maExit(0);
							break;

						case ITEM1_ID:
							if(event.hi_wparam==BN_CLICKED) maExit(0);
							break;

						case ITEM2_ID:
							if(event.hi_wparam==BN_CLICKED) {
								MAWidgetHandle winHandle;
								maWindow(NULL, &winHandle);

								MAWidgetParameters labelParams;
								labelParams.pParent=winHandle.pWidget;
								labelParams.widgetID=1007;
								labelParams.posX=100;
								labelParams.posY=100;
								labelParams.sizeX=200;
								labelParams.sizeY=200;
								char *str5="This is a new window :)";
								strncpy(labelParams.buf, str5, strlen(str5));
								maLabel(&labelParams, NULL);
							}
							break;


					}


			}
		}
		maWait(0);
	} while(1);

	return 0;
}
