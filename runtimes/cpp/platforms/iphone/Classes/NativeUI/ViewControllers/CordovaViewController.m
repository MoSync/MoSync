/*
 Copyright (C) 2011 MoSync AB
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

/**
 * @file CordovaViewController.mm
 * @author Eric Svensson
 *
 * @brief View controller for Cordova-iOS.
 * Uses MoSync's WebView instead of Cordova's.
 * Will be moved to MoSync's main view controller.
 */

#import "CordovaViewController.h"
#import "CDVURLProtocol.h"
#import "CDVTimer.h"
#import "CDVUserAgentUtil.h"

@interface CordovaViewController () {
    NSInteger _userAgentLockToken;
}

@property (nonatomic, readwrite, strong) NSArray* startupPluginNames;

@end

@implementation CordovaViewController

@synthesize startupPluginNames;

/**
 * Overrides init to use MoSync's WebView instead of Cordova's.
 */
- (id)init:(UIWebView*)webView
{
    self.webView = webView;
    self = [super init];
    return self;
}

/**
 * Overrides createGapView to register this view controller instead of Cordova's.
 */
- (void)createGapView
{
    [CDVURLProtocol registerViewController:self];
}


/**
 * Overrides viewDidLoad to avoid e.g. making a webView loadRequest to
 * Cordova's startPage.
 */
- (void)viewDidLoad
{
    [self createGapView];

    if ([self.startupPluginNames count] > 0) {
        [CDVTimer start:@"TotalPluginStartup"];
        
        for (NSString* pluginName in self.startupPluginNames) {
            [CDVTimer start:pluginName];
            [self getCommandInstance:pluginName];
            [CDVTimer stop:pluginName];
        }
        
        [CDVTimer stop:@"TotalPluginStartup"];
    }

    [CDVUserAgentUtil acquireLock:^(NSInteger lockToken) {
        _userAgentLockToken = lockToken;
        [CDVUserAgentUtil setUserAgent:self.userAgent lockToken:lockToken];
    }];
}

@end
