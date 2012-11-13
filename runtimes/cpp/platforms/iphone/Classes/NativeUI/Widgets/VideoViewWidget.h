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

#import <Foundation/Foundation.h>
#import <MediaPlayer/MediaPlayer.h>
#import "IWidget.h"

/**
 * Displays a video player to the user.
 */
@interface VideoViewWidget : IWidget
{
    /**
     * Controller for the media player.
     */
    MPMoviePlayerController* _moviePlayerController;
}

/**
 * Sets a video widget property.
 * @param key The property of the video widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value;

/**
 * Returns a property value of the video widget.
 * @param key The property of the video widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey:(NSString*)key;

/**
 * Handles an video widget action(play, pause or stop).
 * @param One of the MAW_VIDEO_WIDGET_STATE constants.
 */
-(void)handleAction:(NSString*)value;

@end
