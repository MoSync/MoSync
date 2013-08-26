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

#import <Foundation/Foundation.h>
#import "IWidget.h"
#import "CordovaViewController.h"
#import "CDVWebViewDelegate.h"

@interface WebViewWidget : IWidget <UIWebViewDelegate> {
	NSString* newurl;
	NSString* baseUrl;
    NSString* softHookPattern;
	NSString* hardHookPattern;
	NSString* cantNavigate;
	NSString* canNavigateForward;
	NSString* canNavigateBack;
	NSString* canNavigateEither;
    NSString* javaScriptIdentifier; //"javascript:"
    NSMutableDictionary* urlsToNotHook; //Used to let the set url property to bypass the hooks
    CordovaViewController* cordovaViewController;
    CDVWebViewDelegate* cordovaWebViewDelegate;
}

- (id)init;
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;
- (NSString*)getPropertyWithKey: (NSString*)key;
-(NSString*)getDefaultBaseURL;
@end
