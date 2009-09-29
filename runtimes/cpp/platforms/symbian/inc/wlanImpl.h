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

#ifndef WLANIMPL_H
#error "Bad include!"
#endif	//WLANIMPL_H

//This file is designed to be included in the private section of class Syscall.

void ClearWlanVariables();
void ConstructWlanL();
void DestructWlan();

int maWlanStartDiscovery();

int maWlanGetNewAp(MAWlanAp* dst);	//does address translation
int WlanGetNewAp(MAWlanAp* dst);	//actual function

static void WlanSyncCallbackL(TAny* aPtr, TInt aResult);

CWlanScanInfo* gWlanInfo;
CWlanMgmtClient* gWlanClient;
CCBSynchronizer* gWlanSynchronizer;
bool gWlanAvailable;
