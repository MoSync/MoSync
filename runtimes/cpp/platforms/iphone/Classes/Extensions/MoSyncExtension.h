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

#ifndef _MOSYNC_EXTENSION_PROTOCOL_H_
#define _MOSYNC_EXTENSION_PROTOCOL_H_

#include "MoSync.h"

@protocol MoSyncExtensionProtocol
	-(id) initWithMoSync:(MoSync*)mosync;
	-(void) close;
	-(int) getHash;
	-(NSString*) getName;
	-(long long) invokeExtensionWithIndex:(int)id andArg1:(int)a andArg2:(int)b andArg3:(int)c;
@end

void initExtensions(MoSync* mosync);

#endif // _MOSYNC_EXTENSION_PROTOCOL_H_
