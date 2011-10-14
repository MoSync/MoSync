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
	UIWebView* webView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
	view = webView;
	webView.delegate = self;
	newurl = @"";
    softHookPattern = @"";
	hardHookPattern = @"";
    javaScriptIdentifier = @"javascript:";
	cantNavigate = @"";
	canNavigateForward = @"forward";
	canNavigateBack = @"back";
	canNavigateEither = [[canNavigateBack stringByAppendingString:canNavigateForward] retain];
	baseUrl = [[self getDefaultBaseURL] retain];
    urlsToNotHook=[[NSMutableDictionary alloc] init];
	id ret = [super init];
    [self setAutoSizeParamX:FILL_PARENT andY:FILL_PARENT];
    return ret;
}

- (void)dealloc {
    [urlsToNotHook release];
    [super dealloc];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	//Set a new URL
	if([key isEqualToString:@MAW_WEB_VIEW_URL]) {

		UIWebView* webView = (UIWebView*)view;
		//Check whether the user tries to send some javascript
        NSRange identifierLocation = [value rangeOfString:javaScriptIdentifier];
		NSRange schemaLocation = [value rangeOfString:@"://"];
        if(identifierLocation.location == 0)
        {
			NSInteger scriptLocation = identifierLocation.location+identifierLocation.length;
            NSString *script=[[value substringFromIndex:scriptLocation] retain];
            [webView stringByEvaluatingJavaScriptFromString:script];
            [script release];
        }
        else
        {
			NSURL *url;
			//Process a normal URL
          if (schemaLocation.location == NSNotFound) {
			  NSString *urlString = [NSString stringWithFormat:@"%@%@", baseUrl, value];
			  NSString* webURLString = [urlString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
			  url = [[NSURL URLWithString: webURLString] filePathURL];
		  } else {
              // Convert the string URL into ascii encoding.
              NSData* data = [value dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:YES];
              NSString* formattedURL = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
              url = [NSURL URLWithString:formattedURL];
              [formattedURL release];
		  }

            NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
            NSString *absoluteURL = [url absoluteString];

			//Hook by-pass system. Each url is saved in a dictionary with it's
			//string as the key, and the number of time it was called as the value,
			//in case it gets called more than once before it goes thourh the hook system
            NSNumber *unHookCount = (NSNumber*)[urlsToNotHook objectForKey:absoluteURL];
			NSLog(@"%@", absoluteURL);

            if(unHookCount)
            {
				//increase the hook count for this url
				NSNumber *newCount = [NSNumber numberWithInteger:(unHookCount.integerValue+1)];
                [urlsToNotHook setValue:newCount forKey:absoluteURL];
            }
            else
            {
				//create a new dictionary entry for this URL.
                [urlsToNotHook setObject:[NSNumber numberWithInteger:1] forKey:absoluteURL];
            }
            [webView loadRequest:requestObj];
        }

    } else if([key isEqualToString:@MAW_WEB_VIEW_SOFT_HOOK]) {
		[softHookPattern release];
        softHookPattern = [value retain];

	} else if([key isEqualToString:@MAW_WEB_VIEW_HARD_HOOK]) {
		[hardHookPattern release];
        hardHookPattern = [value retain];

    } else if([key isEqualToString:@MAW_WEB_VIEW_HTML]) {
		UIWebView* webView = (UIWebView*)view;
        [webView loadHTMLString:value baseURL:[NSURL URLWithString:baseUrl]];
    } else if([key isEqualToString:@MAW_WEB_VIEW_ENABLE_ZOOM]) {
		UIWebView* webView = (UIWebView*)view;
        if([value isEqualToString:@"true"])
        {
            webView.scalesPageToFit = YES;
        }
        else if([value isEqualToString:@"false"])
        {
            webView.scalesPageToFit = NO;
        }

    } else if([key isEqualToString:@MAW_WEB_VIEW_NAVIGATE]) {
		UIWebView* webView = (UIWebView*)view;
        if([value isEqualToString:@"back"])
        {
            [webView goBack];
        }
        else if([value isEqualToString:@"forward"])
        {
            [webView goForward];
        }

    } else if ([key isEqualToString:@MAW_WEB_VIEW_BASE_URL]) {
		[baseUrl release];
		baseUrl = [value retain];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@MAW_WEB_VIEW_URL]) {
		UIWebView* webView = (UIWebView*)view;
		return [webView.request.URL.absoluteString retain];

	} else if([key isEqualToString:@MAW_WEB_VIEW_NEW_URL]) {
		return [newurl retain];

    } else if([key isEqualToString:@MAW_WEB_VIEW_SOFT_HOOK]) {
		return [softHookPattern retain];

	} else if([key isEqualToString:@MAW_WEB_VIEW_HARD_HOOK]) {
		return [hardHookPattern retain];

    } else if ([key isEqualToString:@MAW_WEB_VIEW_BASE_URL]) {
		return [baseUrl retain];
	} else if ([key isEqualToString:@MAW_WEB_VIEW_NAVIGATE]) {
		UIWebView* webView = (UIWebView*)view;
		if(webView.canGoBack && webView.canGoForward){
			return [canNavigateEither retain];
		}
		else if(webView.canGoBack){
			return [canNavigateBack retain];
		}
		else if(webView.canGoForward){
			return [canNavigateForward retain];
		}
		else {
			return [cantNavigate retain];
		}

	} else {
		return [super getPropertyWithKey:key];
	}
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    MAEvent event;
    event.type = EVENT_TYPE_WIDGET;
    MAWidgetEventData *eventData = new MAWidgetEventData;
    eventData->eventType = MAW_EVENT_WEB_VIEW_CONTENT_LOADING;
    eventData->widgetHandle = handle;
	eventData->status = MAW_CONSTANT_STARTED;
    event.data = (MAAddress)eventData;
    Base::gEventQueue.put(event);
    return;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    MAEvent event;
    event.type = EVENT_TYPE_WIDGET;
    MAWidgetEventData *eventData = new MAWidgetEventData;
    eventData->eventType = MAW_EVENT_WEB_VIEW_CONTENT_LOADING;
    eventData->widgetHandle = handle;
	eventData->status = MAW_CONSTANT_DONE;
    event.data = (MAAddress)eventData;
    Base::gEventQueue.put(event);
    return;
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    MAEvent event;
    event.type = EVENT_TYPE_WIDGET;
    MAWidgetEventData *eventData = new MAWidgetEventData;
    eventData->eventType = MAW_EVENT_WEB_VIEW_CONTENT_LOADING;
    eventData->widgetHandle = handle;
	eventData->status = MAW_CONSTANT_ERROR;
    event.data = (MAAddress)eventData;
    Base::gEventQueue.put(event);
    return;
}

- (BOOL)webView:(UIWebView *)webView
		shouldStartLoadWithRequest:(NSURLRequest *)request
		navigationType:(UIWebViewNavigationType)navigationType {

	if(request.URL.absoluteString == NULL)
    {
        return NO;
    }

    NSString *url=[NSString stringWithString:request.URL.absoluteString];
	//Hook by-pass system for the case of the MoSync coder using the
	//set url property.
    NSNumber *unHookCount = [urlsToNotHook objectForKey:url];
    BOOL skipHook = NO;
    if(unHookCount)
    {
        skipHook = YES;
        if (unHookCount.intValue == 1)
        {
            [urlsToNotHook removeObjectForKey:url];
        }
        else
        {
			NSNumber *newCount = [NSNumber numberWithInteger:(unHookCount.integerValue-1)];
            [urlsToNotHook setValue:newCount forKey:url];
        }
    }
	NSStringCompareOptions options = NSRegularExpressionSearch|NSCaseInsensitiveSearch;
	NSRange softHookRange = [url rangeOfString:softHookPattern options:options];
    NSRange hardHookRange = [url rangeOfString:hardHookPattern options:options];

    if(skipHook == NO &&
		(
		 (softHookRange.location == 0 && softHookRange.length == [url length]) ||
		 (hardHookRange.location == 0 && hardHookRange.length == [url length])
		)
	   )
    {
		//The url matched one of the hooks
        MAEvent event;
        event.type = EVENT_TYPE_WIDGET;
        MAWidgetEventData *eventData = new MAWidgetEventData;
        eventData->eventType = MAW_EVENT_WEB_VIEW_HOOK_INVOKED;
        eventData->widgetHandle = handle;
		bool loadUrl;

		//The hard hook takes precedence in the case that both were matched
		if(hardHookRange.location != NSNotFound)
		{
			eventData->hookType = MAW_CONSTANT_HARD;
			loadUrl = NO; //The hard hood stops the loading process
		}
		else
		{
			eventData->hookType = MAW_CONSTANT_SOFT;
			loadUrl = YES; //The soft hook allows the proading process
		}

        //We create a placeholder resource that holds the url string
        MAHandle urlHandle = (MAHandle) Base::gSyscall->resources.create_RT_PLACEHOLDER();
        int size = (int)[url lengthOfBytesUsingEncoding:NSASCIIStringEncoding];
        Base::MemStream* ms = new Base::MemStream(size);
        Base::gSyscall->resources.add_RT_BINARY(urlHandle, ms);
        ms->seek(Base::Seek::Start, 0);
        ms->write([url cStringUsingEncoding:NSASCIIStringEncoding], size);

        eventData->urlData = urlHandle;
        event.data = (MAAddress)eventData;


        Base::gEventQueue.put(event);

        return loadUrl;
    }

    //Deprecated
   [newurl release];
    newurl = [[NSString stringWithString:request.URL.absoluteString] retain]; // TODO: do have to do this (retain)??

	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_WEB_VIEW_URL_CHANGED;
	eventData->widgetHandle = handle;
	event.data = (MAAddress)eventData;
	Base::gEventQueue.put(event);
	return YES;
}

-(NSString*)getDefaultBaseURL {
	NSString *localPath =  [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
//	NSURL *myURL = [[NSBundle mainBundle] bundleURL];
//	NSLog(@"Bundle URL:%@", [myURL absoluteString]);
//	[myURL release];
	return [NSString stringWithFormat:@"file://localhost%@/", localPath];
}

@end
