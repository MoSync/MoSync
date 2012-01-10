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
