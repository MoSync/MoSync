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

package com.mosync.nativeui.ui.widgets;

import android.net.Uri;
import android.widget.MediaController;
import android.widget.VideoView;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A Video View displays a video file.
 *
 * @author emma
 */
public class VideoViewWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param progressbar A video view wrapped by this widget.
	 */
	public VideoViewWidget(int handle, VideoView videoView, MediaController mc)
	{
		super( handle, videoView );
		mHandle = handle;
		mMediaController = mc;
		mPlaybackWasStopped = false;
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		VideoView videoView = (VideoView) getView( );
		if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_CONTROL ) )
		{

			mShowMediaControlls = BooleanConverter.convert(value);
			if ( mShowMediaControlls )
			{
				videoView.setMediaController(mMediaController);
			}
			else
			{
				videoView.setMediaController(null);
			}
		}
		else if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_PATH ) )
		{
			// Store the last played source, so it can be loaded before starting playback.
			mLastLocalSource = value;
			videoView.setVideoPath(value);
		}
		else if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_URL ) )
		{
			mLastUrlSource = value;
			videoView.setVideoURI(Uri.parse(value));
		}
		else if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_ACTION ) )
		{
			if ( IntConverter.convert(value) == IX_WIDGET.MAW_VIDEO_VIEW_ACTION_PLAY )
			{
				// Reload last played source in case it was stopped.
				if ( mLastLocalSource.length() > 0 && mPlaybackWasStopped )
				{
					videoView.setVideoPath(mLastLocalSource);
				}
				else if ( mLastUrlSource.length() > 0 && mPlaybackWasStopped)
				{
					videoView.setVideoURI(Uri.parse(mLastUrlSource));
				}
				mPlaybackWasStopped = false;

				// And now, The source will be ready for playback even if stop was pressed before.
				videoView.start();

				// Try to start playback, and only if succeeded send the PLAYING event.
				if ( videoView.isPlaying() )
				{
					EventQueue.getDefault().postVideoStateChanged(mHandle, IX_WIDGET.MAW_VIDEO_VIEW_STATE_PLAYING);
				}
				else
				{
					EventQueue.getDefault().postVideoStateChanged(mHandle, IX_WIDGET.MAW_VIDEO_VIEW_STATE_INTERRUPTED);
					return false;
				}
			}
			else if ( IntConverter.convert(value) == IX_WIDGET.MAW_VIDEO_VIEW_ACTION_PAUSE )
			{
				// Pause the playback only if the video is playing.
				if (videoView.isPlaying() && videoView.canPause())
				{
					videoView.pause();
					EventQueue.getDefault().postVideoStateChanged(mHandle, IX_WIDGET.MAW_VIDEO_VIEW_STATE_PAUSED);
				}
				else
				{
					return false;
				}
			}
			else if ( IntConverter.convert(value) == IX_WIDGET.MAW_VIDEO_VIEW_ACTION_STOP )
			{
				mPlaybackWasStopped = true;
				videoView.stopPlayback();

				EventQueue.getDefault().postVideoStateChanged(mHandle, IX_WIDGET.MAW_VIDEO_VIEW_STATE_STOPPED);

			}
		}
		else if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_SEEK_TO ) )
		{
			if (IntConverter.convert(value) > 0 )
			{
				videoView.seekTo(IntConverter.convert(value));
			}
			else
			{
				throw new InvalidPropertyValueException(property, value);
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{

		VideoView videoView = (VideoView) getView( );
		if( property.equals(IX_WIDGET.MAW_VIDEO_VIEW_DURATION ) )
		{
			return Integer.toString(  videoView.getDuration() );
		}
		else if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_CURRENT_POSITION ) )
		{
			return Integer.toString( videoView.getCurrentPosition( ) );
		}
		else if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_BUFFER_PERCENTAGE ) )
		{
			return Integer.toString( videoView.getBufferPercentage( ) );
		}
		else if( property.equals( IX_WIDGET.MAW_VIDEO_VIEW_CONTROL ) )
		{
			return Boolean.toString(mShowMediaControlls);
		}
		else
		{
			return super.getProperty( property );
		}
	}

	/**
	 * The handle of this widget.
	 */
	private int mHandle = 0;

	/**
	 * The controls for media player.
	 * They are shown by default.
	 */
	private MediaController mMediaController;

	/**
	 * Store the state of the MediaControlls.
	 */
	private boolean mShowMediaControlls = true;

	/**
	 * Fixing bug: after stopPlayback(), start() cannot load the source.
	 * So, store the last uri or path and load them after re-starting playback.
	 */
	private String mLastLocalSource = "";
	private String mLastUrlSource = "";

	// Reload the source only after playback was stopped.
	private boolean mPlaybackWasStopped = false;

}
