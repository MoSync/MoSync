
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface CameraConfirgurator : NSObject {
	NSMutableDictionary *flashModes;
	NSMutableDictionary *focusModes;
}

-(int)setCameraProperty: (AVCaptureDevice*)device withProperty:(NSString*)property withValue: (NSString*) value;

-(NSString *)getCameraProperty: (AVCaptureDevice*)device withProperty:(NSString*)property;

@end
