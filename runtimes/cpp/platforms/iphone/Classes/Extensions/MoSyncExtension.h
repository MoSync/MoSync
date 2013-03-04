#ifndef _MOSYNC_EXTENSION_PROTOCOL_H_
#define _MOSYNC_EXTENSION_PROTOCOL_H_

#include "MoSync.h"
#include "cpp_maapi.h"

@protocol MoSyncExtensionProtocol
	-(id) initWithMoSync:(MoSync*)mosync;
	-(void) close;
	-(int) getHash;
	-(NSString*) getName;
	-(long long) invokeExtensionWithIndex:(int)id andArg1:(int)a andArg2:(int)b andArg3:(int)c;
@end

@interface FunctionInvocation : NSObject
-(id) init;
-(void) invoke:(int)numargs (int)ptr;
@end

void initExtensions(MoSync* mosync);
MAExtensionModule getExtensionModule(const char* name, int hash);
MAExtensionFunction getExtensionFunction(MAExtensionModule module, int ix);
int invokeExtensionFunction(MAExtensionFunction fn, int numargs, int ptrs);

#endif // _MOSYNC_EXTENSION_PROTOCOL_H_
