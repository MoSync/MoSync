#include <ma.h>
#include <IX_NATIVE_UI.h>
#include <conprint.h>
#include <madmath.h>
#include <maheap.h>
#include <matime.h>
#include <maassert.h>

int MAMain() {
	
	
	MAWidgetParameters wparams0;
	wparams0.widgetID = 10;
	
	MAWidgetHandle whandle;
	maIPhoneView(&wparams0, &whandle);
	/*
	MAWidgetParameters wparamsBar;
	char *str1="Hello";
	wparamsBar.pParent = whandle.pWidget;
	strncpy(wparamsBar.buf, str1, strlen(str1)+1);
	maIPhoneNavigationBar(&wparamsBar, NULL);
	*/
	MAWidgetParameters wparams;
	wparams.pParent = whandle.pWidget;
	char *strBar="Open a new window";
	strncpy(wparams.buf, strBar, strlen(strBar)+1);
	wparams.posX = 10;
	wparams.posY = 200;
	wparams.sizeX = 300;
	wparams.sizeY = 30;
	wparams.widgetID = 666;
	
	MAWidgetParameters wparams2;
	wparams2.pParent = whandle.pWidget;
	char *str2="I am a native widget too !";
	strncpy(wparams2.buf, str2, strlen(str2)+1);
	wparams2.posX = 10;
	wparams2.posY = 90;
	wparams2.sizeX = 300;
	wparams2.sizeY = 30;
	
	MAWidgetParameters wparams3;
	wparams3.pParent = whandle.pWidget;
	char *str3="Even I am a native widget !";
	strncpy(wparams3.buf, str2, strlen(str3)+1);
	wparams3.posX = 10;
	wparams3.posY = 150;
	wparams3.sizeX = 300;
	wparams3.sizeY = 30;
	
	maIPhoneLabel(&wparams2, NULL);
	maIPhoneButton(&wparams, NULL);
	maIPhoneEdit(&wparams3, NULL);

	// Handle events
	MAEvent event;
	do {
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_NATIVE_UI_MSG) {
				
				switch(event.lo_wparam) {
					case 666:
						if(event.hi_wparam==0) {
							
							MAWidgetParameters wparams4;
							wparams4.widgetID = 11;
							
							MAWidgetHandle whandle2;
							maIPhoneView(&wparams4, &whandle2);
							
							MAWidgetParameters wparamsBar;
							char *str1="Hello";
							wparamsBar.pParent = whandle2.pWidget;
							strncpy(wparamsBar.buf, str1, strlen(str1)+1);
							maIPhoneNavigationBar(&wparamsBar, NULL);
							
						}
						break;
						
						
					case 10:
						
						break;
						


				}


			}
		}
		maWait(0);
	} while(1);
	
	return 0;
} 
