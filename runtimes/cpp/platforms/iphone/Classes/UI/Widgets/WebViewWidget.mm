/* Copyright (C) 2011 MoSync AB
 
 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

#import "WebViewWidget.h"

#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation WebViewWidget

- (id)init {
	UIWebView* webView = [[[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)] retain]; // TODO: do have to do this (retain)??
	view = webView;
	webView.delegate = self;
	newurl = @"";
	return [super init];	
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"url"]) {
		UIWebView* webView = (UIWebView*)view;
		NSURL *url = [NSURL URLWithString:value];
		NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
		[webView loadRequest:requestObj];
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@"url"]) {
		UIWebView* webView = (UIWebView*)view;
		return webView.request.URL.absoluteString;
	} else if([key isEqualToString:@"newurl"]) {
		NSString* ret = @"";
		ret = newurl;
		return ret;
	} else {
		return [super getPropertyWithKey:key];
	}
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
	newurl = [[NSString stringWithString:request.URL.absoluteString] retain]; // TODO: do have to do this (retain)??
	
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_WEB_VIEW_URL_CHANGED;
	eventData->widgetHandle = handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
	return YES; // MoSync user have to manually start a new request..
}

@end