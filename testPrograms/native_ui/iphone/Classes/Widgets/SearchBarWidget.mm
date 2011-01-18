//
//  SearchScreenWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SearchBarWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation SearchBarWidget

- (id)init {
	searchBar = [[[UISearchBar alloc] initWithFrame:CGRectMake(0, 10, 100, 30)] retain];
	searchBar.text = @"Search";
	view = searchBar;			
	id ret = [super init];
	[searchBar setAutoresizingMask:UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight];
	view.autoresizesSubviews = YES;
	searchBar.showsCancelButton = YES;
	
	return ret;
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"image"]) {
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return WIDGET_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end