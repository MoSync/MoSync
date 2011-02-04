//
//  MoSyncTableView.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/25/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface MoSyncTableView : UITableViewController {
	NSMutableArray* mDataForMyTable;
}

//@property(nonatomic, retain)NSMutableArray *mDataForMyTable;

- (void) addView:(UIView*) view;

@end
