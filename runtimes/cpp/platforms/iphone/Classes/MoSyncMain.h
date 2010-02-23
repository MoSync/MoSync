/*
 *  MoSyncMain.h
 *  MoSync
 *
 *  Created by Niklas Nummelin on 2/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MOSYNCMAIN_H_
#define _MOSYNCMAIN_H_

class UIView;

void MoSyncMain(int width, int height, UIView* mosyncView);

void UpdateMoSyncView(CGContextRef ref);
void DoneUpdatingMoSyncView();

#endif

