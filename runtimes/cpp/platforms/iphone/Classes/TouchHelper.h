

@interface TouchHelper : NSObject {
	UITouch* dummyTouch;
	NSMutableArray* touchArray;	
}

- (id) init;
- (int) addTouch: (UITouch*) touch;
- (int) getTouchId: (UITouch*) touch;
- (void) removeTouch: (UITouch*) touch;
- (void) clearTouches;

@end
