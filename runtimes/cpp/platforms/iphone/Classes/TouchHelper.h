#ifndef _TOUCH_HELPER_H_
#define _TOUCH_HELPER_H_

@interface TouchHelper : NSObject {
	NSMutableArray* touchArray;	
}

- (id) init;
- (int) addTouch: (UITouch*) touch;
- (int) getTouchId: (UITouch*) touch;
- (void) removeTouch: (UITouch*) touch;
- (void) clearTouches;

@end

#endif
