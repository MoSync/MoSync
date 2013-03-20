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

// default rectangle values for the video player
#define DEFAULT_RECT_X        0
#define DEFAULT_RECT_Y        0
#define DEFAULT_RECT_WIDTH  100
#define DEFAULT_RECT_HEIGHT 100

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "VideoViewWidget.h"
#include "Platform.h"

@implementation VideoViewWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        _moviePlayerController = [[MPMoviePlayerController alloc] init];

        CGRect viewRect = CGRectMake(DEFAULT_RECT_X, DEFAULT_RECT_Y, DEFAULT_RECT_WIDTH, DEFAULT_RECT_HEIGHT);
        UIView* smallView = [[UIView alloc] initWithFrame:viewRect];

        [_moviePlayerController.view setFrame: smallView.bounds];
        self.view = smallView;
        [self.view addSubview:_moviePlayerController.view];
        [_moviePlayerController setScalingMode:MPMovieScalingModeAspectFit];

        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(moviePlaybackComplete:)
                                                     name:MPMoviePlayerPlaybackDidFinishNotification
                                                   object:_moviePlayerController];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(moviePlaybackStateChanged:)
                                                     name:MPMoviePlayerPlaybackStateDidChangeNotification
                                                   object:_moviePlayerController];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(sourceReady:)
                                                     name:MPMoviePlayerLoadStateDidChangeNotification
                                                   object:_moviePlayerController];
        [smallView release];
        smallView = nil;
    }

    return self;
}

/**
 * Sets a video view property.
 * @param key The property of the video widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
    if ([key isEqualToString:@MAW_VIDEO_VIEW_PATH])
    {
        NSURL *fileURL = [NSURL fileURLWithPath:value];
        if (fileURL)
        {
            [_moviePlayerController setContentURL:fileURL];
        }
        else
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else if ([key isEqualToString:@MAW_VIDEO_VIEW_URL])
    {
        NSURL *webURL = [NSURL URLWithString:value];
        [_moviePlayerController setContentURL:webURL];
    }
    else if ([key isEqualToString:@MAW_VIDEO_VIEW_ACTION])
    {
        [self handleAction:value];
    }
    else if ([key isEqualToString:@MAW_WIDGET_WIDTH] ||
             [key isEqualToString:@MAW_WIDGET_HEIGHT])
    {
        [super setPropertyWithKey:key toValue:value];
        [_moviePlayerController.view setFrame: self.view.bounds];
    }
    else if ([key isEqualToString:@MAW_VIDEO_VIEW_SEEK_TO])
    {
        TEST_FOR_NEGATIVE_VALUE([value floatValue]);
		// Value is in milliseconds we we have to convert it to seconds.
		CGFloat timeInSeconds = [value floatValue] / SECOND;
        [_moviePlayerController setCurrentPlaybackTime: timeInSeconds];
    }
    else if ([key isEqualToString:@MAW_VIDEO_VIEW_CONTROL])
    {
        if ([value isEqualToString:kWidgetTrueValue])
        {
            [_moviePlayerController setControlStyle:MPMovieControlStyleEmbedded];
        }
        else if ([value isEqualToString:kWidgetFalseValue])
        {
            [_moviePlayerController setControlStyle:MPMovieControlStyleNone];
        }
        else
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }

    return MAW_RES_OK;
}

/**
 * Returns a property value of the video widget.
 * @param key The property of the video widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
	if([key isEqualToString:@MAW_VIDEO_VIEW_DURATION])
    {
        // Convert from seconds to milliseconds.
        int duration = [_moviePlayerController duration] * 1000;
        return[[NSString alloc] initWithFormat:@"%d", duration];
	}
    if([key isEqualToString:@MAW_VIDEO_VIEW_CURRENT_POSITION])
    {
        return[[NSString alloc] initWithFormat:@"%f", [_moviePlayerController currentPlaybackTime]];
	}
    else if ([key isEqualToString:@MAW_VIDEO_VIEW_CONTROL])
    {
        MPMovieControlStyle controlStyle = _moviePlayerController.controlStyle;
        if (controlStyle == MPMovieControlStyleEmbedded)
        {
            return [[NSString alloc] initWithString:kWidgetTrueValue];
        }
        else
        {
            return [[NSString alloc] initWithString:kWidgetFalseValue];
        }
    }
    else
    {
		return [super getPropertyWithKey:key];
	}
}

/**
 * Handle and video widget action(play, pause or stop the video).
 * @param One of the VideoWidgetAction.
 */
-(void)handleAction:(NSString*) value
{
    int newStateValue = [value intValue];
    switch (newStateValue)
    {
        case MAW_VIDEO_VIEW_ACTION_PLAY:
            [_moviePlayerController play];
            break;
        case MAW_VIDEO_VIEW_ACTION_PAUSE:
            [_moviePlayerController pause];
            break;
        case MAW_VIDEO_VIEW_ACTION_STOP:
            [_moviePlayerController stop];
            break;
        default:
            break;
    }
}

/**
 * This delegate method is invoked when the movie has finished playing.
 * @param The current movie player object.
 */
- (void)moviePlaybackComplete:(NSNotification *)notification
{
    // Check if an error occurred.
    NSError *error = [[notification userInfo] objectForKey:@"error"];
    if (!error)
    {
        MAEvent event;
        event.type = EVENT_TYPE_WIDGET;

        MAWidgetEventData *eventData = new MAWidgetEventData;
        eventData->eventType = MAW_EVENT_VIDEO_STATE_CHANGED;
        eventData->videoViewState = MAW_VIDEO_VIEW_STATE_FINISHED;
        eventData->widgetHandle = self.handle;

        event.data = (int)eventData;
        Base::gEventQueue.put(event);
    }
    else
    {
        NSLog(@"VideoViewWidget::moviePlaybackComplete error = %@", [error description]);
    }
}

/**
 * This delegate method is invoked when the playback state has changed.
 * @param The current movie player object.
 */
- (void)moviePlaybackStateChanged:(NSNotification *)notification
{
    // Check if an error occurred.
    NSError *error = [[notification userInfo] objectForKey:@"error"];
    if (!error)
    {
        int playbackStateEvent = 0;
        BOOL sendEvent = TRUE;
        MPMoviePlaybackState playbackState = [_moviePlayerController playbackState];

        // Check the type of the event.
        switch (playbackState)
        {
            case MPMoviePlaybackStatePlaying:
                playbackStateEvent = MAW_VIDEO_VIEW_STATE_PLAYING;
                break;
            case MPMoviePlaybackStatePaused:
                playbackStateEvent = MAW_VIDEO_VIEW_STATE_PAUSED;
                break;
            case MPMoviePlaybackStateStopped:
                playbackStateEvent = MAW_VIDEO_VIEW_STATE_STOPPED;
                break;
            case MPMoviePlaybackStateInterrupted:
                playbackStateEvent = MAW_VIDEO_VIEW_STATE_INTERRUPTED;
                break;
            default:
                // Don't handle the other playback states.
                sendEvent = FALSE;
                break;
        }

        if (sendEvent)
        {
            MAEvent event;
            event.type = EVENT_TYPE_WIDGET;

            MAWidgetEventData *eventData = new MAWidgetEventData;
            eventData->eventType = MAW_EVENT_VIDEO_STATE_CHANGED;
            eventData->widgetHandle = self.handle;
            eventData->videoViewState = playbackStateEvent;

            event.data = (int)eventData;
            Base::gEventQueue.put(event);
        }
    }
}

/**
 * This delegate method is invoked when the network buffering state has changed.
 * @param The current movie player object.
 */
- (void)sourceReady:(NSNotification *)notification
{
    // Check if an error occurred.
    NSError *error = [[notification userInfo] objectForKey:@"error"];
    if (!error)
    {
        if (MPMovieLoadStatePlayable == [_moviePlayerController loadState])
        {
            MAEvent event;
            event.type = EVENT_TYPE_WIDGET;

            MAWidgetEventData *eventData = new MAWidgetEventData;
            eventData->eventType = MAW_EVENT_VIDEO_STATE_CHANGED;
            eventData->videoViewState = MAW_VIDEO_VIEW_STATE_SOURCE_READY;
            eventData->widgetHandle = self.handle;

            event.data = (int)eventData;
            Base::gEventQueue.put(event);
        }
    }
}

/**
 * Release the objects and remove the observers.
 */
- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerPlaybackDidFinishNotification
                                                  object:_moviePlayerController];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerPlaybackStateDidChangeNotification
                                                  object:_moviePlayerController];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerLoadStateDidChangeNotification
                                                  object:_moviePlayerController];

    [_moviePlayerController release];
    [super dealloc];
}

@end
