/*
 *  iphone_helpers.mm
 *  MoSync
 *
 *  Created by Romain Chalant on 4/09/10.
 *  Copyright 2010 MoSync. All rights reserved.
 *
 */

#ifndef _NATIVEUI_H_
#define _NATIVEUI_H_

#include "MoSyncMain.h"
#include <helpers/CPP_IX_NATIVE_UI.h>

int maIPhoneMessageBox(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneLabel(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneButton(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneEdit(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneView(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneListView(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneListItem(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneNavigationBar(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneToolBar(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneToolBarItem(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneImage(MAWidgetParameters *wparams, MAWidgetHandle *whandle);
int maIPhoneGetText(MAWidgetParameters *wparams, MAWidgetHandle *whandle);

#endif