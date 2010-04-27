/*
 *  iphone_helpers.mm
 *  MoSync
 *
 *  Created by Niklas Nummelin on 2/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "iphone_helpers.h"
#include <wchar.h>

#include <sys/sysctl.h>  
#import <mach/mach.h>
#import <mach/mach_host.h>

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
	NSString *stringFromChar = [NSString stringWithCString:urlstr length:strlen(urlstr)];
	NSURL *url = [NSURL URLWithString:stringFromChar];
	if (![[UIApplication sharedApplication] openURL:url])
		return false;
	return true;
}