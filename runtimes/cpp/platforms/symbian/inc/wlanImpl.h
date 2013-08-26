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
