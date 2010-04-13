#include <ma.h>
#include <IX_NATIVE_UI.h>
#include <conprint.h>
#include <madmath.h>
#include <maheap.h>
#include <matime.h>
#include <maassert.h>

int MAMain() {
	
	//maIPhoneMessageBox();
	//maIPhoneLabel();
	MAWidgetHandle whandle;
	maIPhoneView(&whandle);
	
	MAWidgetParameters wparams;
	wparams.pParent = whandle.pWidget;
	char *str1="I am a native widget !";
	strncpy(wparams.buf, str1, strlen(str1)+1);
	wparams.posX = 10;
	wparams.posY = 30;
	wparams.sizeX = 300;
	wparams.sizeY = 30;
	
	MAWidgetParameters wparams2;
	wparams2.pParent = whandle.pWidget;
	char *str2="I am a native widget too !";
	strncpy(wparams2.buf, str2, strlen(str2)+1);
	wparams2.posX = 10;
	wparams2.posY = 90;
	wparams2.sizeX = 300;
	wparams2.sizeY = 30;
	
	maIPhoneLabel(&wparams2, NULL);
	maIPhoneButton(&wparams, NULL);
	//maIPhoneButton();
	//maIPhoneView();
	FREEZE;
	return 0;
} 
