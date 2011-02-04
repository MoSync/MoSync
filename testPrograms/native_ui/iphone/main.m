//
//  main.m
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/25/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"nativeuitestAppDelegate");
    [pool release];
    return retVal;
}
