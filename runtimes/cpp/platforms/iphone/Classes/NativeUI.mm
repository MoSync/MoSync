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


int maIPhoneMessageBox(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *my_sMoSyncView = (MoSyncView *)getMoSyncView();
	[my_sMoSyncView showMessageBox:[NSString stringWithCString:wparams->buf /*length:strlen(wparams->buf)*/ encoding:NSASCIIStringEncoding] shouldKill:false];
	return 0;
}

int maIPhoneLabel(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showLabel:[NSString stringWithCString:(wparams->buf) encoding:NSASCIIStringEncoding] 
						   posX:wparams->posX posY:wparams->posY length:wparams->sizeX height:wparams->sizeY widgetId:wparams->widgetID];
	return 0;
}

int maIPhoneButton(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showButton:[NSString stringWithCString:(wparams->buf) encoding:NSASCIIStringEncoding]
							posX:wparams->posX posY:wparams->posY length:wparams->sizeX height:wparams->sizeY widgetId:wparams->widgetID];
	return 0;
}

int maIPhoneEdit(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showTextField:[NSString stringWithCString:(wparams->buf) encoding:NSASCIIStringEncoding]
							   posX:wparams->posX posY:wparams->posY length:wparams->sizeX height:wparams->sizeY widgetId:wparams->widgetID];
	return 0;
}

int maIPhoneView(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *my_sMoSyncView = (MoSyncView *) getMoSyncView();
	MoSyncView *vi = [my_sMoSyncView showScreen:wparams->widgetID];
	whandle->pWidget = vi;
	return 0;
}

int maIPhoneListView(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *my_sMoSyncView = (MoSyncView *) getMoSyncView();
	MoSyncView *vi = [my_sMoSyncView showScreenList:wparams->widgetID];
	whandle->pWidget = vi;
	return 0;
}

int maIPhoneListItem(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showListItem:[NSString stringWithCString:(wparams->buf) encoding:NSASCIIStringEncoding] widgetId: wparams->widgetID ];
	return 0;
}

int maIPhoneNavigationBar(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showNavigationBar:[NSString stringWithCString:(wparams->buf) encoding:NSASCIIStringEncoding] ];
	return 0;
}


int maIPhoneToolBar(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showToolBar];
	return 0;
}

int maIPhoneToolBarItem(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showToolBarItem:wparams->widgetID withIcon: wparams->rsc];
	return 0;
}

int maIPhoneImage(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	[mosyncview showImage:wparams->widgetID withImage: wparams->rsc posX: wparams->posX posY: wparams->posY];
	return 0;
}

int maIPhoneGetText(MAWidgetParameters *wparams, MAWidgetHandle *whandle) 
{
	MoSyncView *mosyncview = (MoSyncView *)wparams->pParent;
	NSString *str = [mosyncview getText:wparams->widgetID];
	const char *cString = [str UTF8String];
	//char *cString2;
	//strcpy ( cString2, cString );
	strcpy(whandle->buf, cString);
	NSLog([NSString stringWithCString:(whandle->buf) encoding:NSASCIIStringEncoding]);
	return 0;
}
