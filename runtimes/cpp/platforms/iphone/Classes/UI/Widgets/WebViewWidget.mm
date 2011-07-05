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
    hookPattern = @"";
    javaScriptIdentifier=@"javascript:";
    urlsToNotHook=[[NSMutableDictionary alloc] init];
    [urlsToNotHook retain];
	return [super init];	
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"url"]) {
		UIWebView* webView = (UIWebView*)view;
        NSRange identifierLocation=[value rangeOfString:javaScriptIdentifier];
        if(identifierLocation.location!=NSNotFound)
        {
            NSString *script=[[value substringFromIndex:identifierLocation.location+identifierLocation.length] retain];
            [webView stringByEvaluatingJavaScriptFromString:script];
            [script release];
        }
        else
        {
            NSURL *url = [NSURL URLWithString:value];
            NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
            NSString *absoluteURL=[url absoluteString];
            NSNumber *unHookCount=(NSNumber*)[urlsToNotHook objectForKey:absoluteURL];
            
            if(unHookCount)
            {
                [urlsToNotHook setValue:[NSNumber numberWithInteger:(unHookCount.integerValue+1)] forKey:absoluteURL]; //No idea why...
            }
            else
            {
                [urlsToNotHook setObject:[NSNumber numberWithInteger:1] forKey:absoluteURL];
            }
            [webView loadRequest:requestObj];
        }
    } else if([key isEqualToString:@MAW_WEB_VIEW_URL_HOOK_PATTERN]) {
		[hookPattern release];
        hookPattern = [value retain];
    } else if([key isEqualToString:@MAW_WEB_VIEW_HTML]) {
		UIWebView* webView = (UIWebView*)view;
        [webView loadHTMLString:value baseURL:NULL];
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
    } else if([key isEqualToString:@MAW_WEB_VIEW_URL_HOOK_PATTERN]) {
		return hookPattern;
	} else {
		return [super getPropertyWithKey:key];
	}
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    MAEvent event;
    event.type = EVENT_TYPE_WIDGET;
    MAWidgetEventData *eventData = new MAWidgetEventData;
    eventData->eventType = MAW_EVENT_CONTENT_STARTED_LOADING;
    eventData->widgetHandle = handle;
    event.data = eventData;
    Base::gEventQueue.put(event);
    return;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    MAEvent event;
    event.type = EVENT_TYPE_WIDGET;
    MAWidgetEventData *eventData = new MAWidgetEventData;
    eventData->eventType = MAW_EVENT_CONTENT_LOADED;
    eventData->widgetHandle = handle;
    event.data = eventData;
    Base::gEventQueue.put(event);
    return;
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
	if(request.URL.absoluteString==NULL)
    {
        return NO;
    }
    NSString *url=[NSString stringWithString:request.URL.absoluteString];
    NSNumber *unHookCount=[urlsToNotHook objectForKey:url];
    BOOL skipHook=NO;
    if(unHookCount)
    {
        skipHook=YES;
        
        if (unHookCount.intValue==1)
        {
            [urlsToNotHook removeObjectForKey:url];
        }
        else
        {
            [urlsToNotHook setValue:[NSNumber numberWithInteger:(unHookCount.integerValue-1)] forKey:url];
        }
    }
    NSRange range=[url rangeOfString:hookPattern options:NSRegularExpressionSearch|NSCaseInsensitiveSearch];
    if(skipHook==NO && range.location==0 && range.length==[url length])
    {
        MAEvent event;
        event.type = EVENT_TYPE_WIDGET;
        MAWidgetEventData *eventData = new MAWidgetEventData;
        eventData->eventType = MAW_EVENT_CUSTOM_MESSAGE;
        eventData->widgetHandle = handle;
        
        MAHandle urlHandle=(MAHandle) Base::gSyscall->resources.create_RT_PLACEHOLDER();
        int size=(int)[url lengthOfBytesUsingEncoding:NSASCIIStringEncoding];
        Base::MemStream* ms = new Base::MemStream(size);
        Base::gSyscall->resources.add_RT_BINARY(urlHandle, ms);
        ms->seek(Base::Seek::Start, 0);
        ms->write([url cStringUsingEncoding:NSASCIIStringEncoding], size);

        eventData->messageDataHandle = urlHandle;
        event.data = eventData;
        
        
        Base::gEventQueue.put(event);
                
        return NO;
    }
    
    //Deprecated
    /*newurl = [[NSString stringWithString:request.URL.absoluteString] retain]; // TODO: do have to do this (retain)??
	
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_WEB_VIEW_URL_CHANGED;
	eventData->widgetHandle = handle;
	event.data = eventData;
	Base::gEventQueue.put(event);*/
	return YES; 
}


@end














