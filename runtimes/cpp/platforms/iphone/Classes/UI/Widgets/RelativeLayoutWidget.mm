//
//  RelativeLayoutWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "RelativeLayoutWidget.h"


@implementation RelativeLayoutWidget

- (id)init {	
	view = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];	
	return [super init];	
}

@end
