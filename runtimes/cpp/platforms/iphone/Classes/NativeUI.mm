/*
 *  iphone_helpers.mm
 *  MoSync
 *
 *  Created by Romain Chalant on 4/09/10.
 *  Copyright 2010 MoSync. All rights reserved.
 *
 */

#include "NativeUI.h"

#include "MoSyncView.h"


int maIPhoneMessageBox() 
{
	const char *str = "I am a MoSync program !";
	UIView *my_sMoSyncView = getMoSyncView();
	[my_sMoSyncView showMessageBox:[NSString stringWithCString:str length:strlen(str)] shouldKill:false];
	
	return 0;
}

int maIPhoneLabel(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	//MoSyncView *my_sMoSyncView = getMoSyncView();
	//[my_sMoSyncView showLabel];
	//const char *str = "I am a label !";
	[wparams->pParent showLabel:[NSString stringWithCString:(wparams->buf) length:strlen(wparams->buf)] 
						   posX:wparams->posX posY:wparams->posY length:wparams->sizeX height:wparams->sizeY widgetId:wparams->widgetID];
	
	return 0;
}

int maIPhoneButton(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	//UIView *my_sMoSyncView = getMoSyncView();
	[wparams->pParent showButton:[NSString stringWithCString:(wparams->buf) length:strlen(wparams->buf)]
							posX:wparams->posX posY:wparams->posY length:wparams->sizeX height:wparams->sizeY widgetId:wparams->widgetID];
	return 0;
}

int maIPhoneEdit(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	//UIView *my_sMoSyncView = getMoSyncView();
	[wparams->pParent showTextField:[NSString stringWithCString:(wparams->buf) length:strlen(wparams->buf)]
							   posX:wparams->posX posY:wparams->posY length:wparams->sizeX height:wparams->sizeY widgetId:wparams->widgetID];
	return 0;
}

int maIPhoneView(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *my_sMoSyncView = getMoSyncView();
	MoSyncView *vi = [my_sMoSyncView showScreen:wparams->widgetID];
	whandle->pWidget = vi;
	//[vi showLabel];
	//[vi showButton];
	return 0;
}

int maIPhoneNavigationBar(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	//UIView *my_sMoSyncView = getMoSyncView();
	[wparams->pParent showNavigationBar:[NSString stringWithCString:(wparams->buf) length:strlen(wparams->buf)] ];
	return 0;
}


int maIPhoneToolBar(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	//UIView *my_sMoSyncView = getMoSyncView();
	[wparams->pParent showToolBar:[NSString stringWithCString:(wparams->buf) length:strlen(wparams->buf)] ];
	return 0;
}

int maIPhoneToolBarItem(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	//UIView *my_sMoSyncView = getMoSyncView();
	[wparams->pParent showToolBarItem:[NSString stringWithCString:(wparams->buf) length:strlen(wparams->buf)] ];
	return 0;
}
