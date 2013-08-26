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

/*
 *  iphone_helpers.mm
 *  MoSync
 *
 *  Created by Niklas Nummelin on 2/23/10.
 *
 */

#include "iphone_helpers.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <wchar.h>

#include <sys/sysctl.h>
#import <mach/mach.h>
#import <mach/mach_host.h>

void logWithNSLog(const char *str, int length) {
	NSString* text = [[NSString alloc] initWithBytes:str length:length encoding:NSUTF8StringEncoding];
	NSLog(@"%@", text);
	[text release];
}

const char *getReadablePath(const char* path) {
	NSString *stringFromChar = [[NSString alloc] initWithBytes:path length:strlen(path) encoding:NSUTF8StringEncoding];
	NSString *nspath = [[NSBundle mainBundle] pathForResource:stringFromChar ofType:@""];
    [stringFromChar release];
	return [nspath UTF8String];
}

const char *getWriteablePath(const char* path) {
	NSString *stringFromChar =  [[NSString alloc] initWithBytes:path length:strlen(path) encoding:NSUTF8StringEncoding];
	//[NSString stringWithCString:path length:strlen(path)];
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectoryPath = [paths objectAtIndex:0];
	NSString *myFilePath = [documentsDirectoryPath stringByAppendingPathComponent:stringFromChar];
    [stringFromChar release];
	return [myFilePath UTF8String];
}

void sleepMillis(int ms) {
	[NSThread sleepForTimeInterval:(((float)ms)/1000.0f)];
}

const char *unicodeToAscii(const wchar_t* str) {
	NSString *stringFromChar = [NSString stringWithCharacters:(const unichar*)str length:wcslen(str)];
	return [stringFromChar UTF8String];

}

int getFreeAmountOfMemory() {
	mach_port_t host_port;
	mach_msg_type_number_t host_size;
	vm_size_t pagesize;
	host_port = mach_host_self();
	host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
	host_page_size(host_port, &pagesize);
	vm_statistics_data_t vm_stat;
	if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) {
		NSLog(@"Failed to fetch vm statistics");
		return 0;
	}
	/* Stats in bytes */
	natural_t mem_free = vm_stat.free_count * pagesize;
	return mem_free;
}

int getTotalAmountOfMemory() {

	int mem;
    int mib[2];
    mib[0] = CTL_HW;

	/*
	mib[1] = HW_PHYSMEM;
    size_t length = sizeof(mem);
    sysctl(mib, 2, &mem, &length, NULL, 0);
    NSLog(@"Physical memory: %.2fMB", mem/1024.0f/1024.0f);
	*/

    mib[1] = HW_USERMEM;
    size_t length = sizeof(mem);
    sysctl(mib, 2, &mem, &length, NULL, 0);
   // NSLog(@"User memory: %.2fMB", mem/1024.0f/1024.0f);

	return mem;
}

bool platformRequest(const char *urlstr) {
//	NSString *stringFromChar = [NSString stringWithCString:urlstr length:strlen(urlstr)];
	NSString *stringFromChar = [[NSString alloc] initWithBytes:urlstr length:strlen(urlstr) encoding:NSUTF8StringEncoding];
	NSURL *url = [NSURL URLWithString:stringFromChar];
    [stringFromChar release];
	if (![[UIApplication sharedApplication] openURL:url])
		return false;
	return true;
}

size_t wcharLength(const wchar* str) {
	size_t length = 0;
	while(*str++) length++;
	return length;
}

size_t wchartLength(const wchar_t* str)
{
	size_t length = 0;
	while(true)
	{
		if (str[length] == '\0')
		{
			break;
		}
		length++;
	}
	return length;
}

void getScreenResolution(int& w, int &h) {

    w = 320;
    h = 480;

    float ver = [[[UIDevice currentDevice] systemVersion] floatValue];
    // You can't detect screen resolutions in pre 3.2 devices, but they are all 320x480
    if (ver >= 3.2f)
    {
        UIScreen* mainscr = [UIScreen mainScreen];
        //w = mainscr.currentMode.size.width;
        //h = mainscr.currentMode.size.height;
        w = mainscr.scale*mainscr.applicationFrame.size.width;
        h = mainscr.scale*mainscr.applicationFrame.size.height;
    }
}

float getScreenScale() {
    return [UIScreen mainScreen].scale;
}
