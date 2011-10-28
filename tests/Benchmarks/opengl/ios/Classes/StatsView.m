
#import "StatsView.h"


@interface StatsView ()

@end

#pragma mark -

@implementation StatsView

@synthesize delegate;

- (void)drawView 
{

    [delegate drawView:self];

}

- (void)dealloc 
{
    [super dealloc];
}

@end
