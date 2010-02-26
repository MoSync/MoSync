/*
 *  iphone_helpers.mm
 *  MoSync
 *
 *  Created by Niklas Nummelin on 2/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "iphone_helpers.h"

const char *getReadablePath(const char* path) {
	NSString *stringFromChar = [NSString stringWithCString:path length:strlen(path)];  	
	NSString *nspath = [[NSBundle mainBundle] pathForResource:stringFromChar ofType:@""];
//	NSString *source = [NSString stringWithContentsOfFile:nspath encoding:NSUTF8StringEncoding error:NULL];
	return [nspath UTF8String];
}

const char *getWriteablePath(const char* path) {
	NSString *stringFromChar = [NSString stringWithCString:path length:strlen(path)];  	
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *documentsDirectoryPath = [paths objectAtIndex:0];	
	NSString *myFilePath = [documentsDirectoryPath stringByAppendingPathComponent:stringFromChar];
	return [myFilePath UTF8String];	
}

void sleepMillis(int ms) {
	[NSThread sleepForTimeInterval:(((float)ms)/1000.0f)];
}


// timer
/*
int timerFunc(void *arg) {
	Timer::TimerInfo* timerInfo = (Timer::TimerInfo*) arg;
	sleepMillis(timerInfo->ms);
	timerInfo->cb(timerInfo->arg);
}

Timer::Timer(Timer::TimerCallback cb, int ms, void *arg) {
	mTimerInfo.cb = cb;
	mTimerInfo.ms = ms;
	mTimerInfo.arg = arg;
													 
}

Timer::~Timer() {
}

void Timer::run() {
	mThread.start(timerFunc, &mTimerInfo);
}
*/