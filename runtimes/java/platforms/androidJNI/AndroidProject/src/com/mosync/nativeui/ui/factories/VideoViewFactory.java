package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.MediaController;
import android.widget.VideoView;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.VideoViewWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates a video view and sets up the default
 * behavior for sending events when clicked.
 *
 * @author emma
 */
public class VideoViewFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		VideoView videoView = new VideoView( activity);

		videoView.setOnPreparedListener( new OnPreparedListener() {

			@Override
			public void onPrepared(MediaPlayer mp) {
				EventQueue.getDefault().postVideoStateChanged( handle, IX_WIDGET.MAW_VIDEO_VIEW_STATE_SOURCE_READY);
			}
		});

		videoView.setOnCompletionListener( new OnCompletionListener() {

			@Override
			public void onCompletion(MediaPlayer mp) {
				EventQueue.getDefault().postVideoStateChanged(handle, IX_WIDGET.MAW_VIDEO_VIEW_STATE_FINISHED);
			}
		});

		videoView.setOnErrorListener( new OnErrorListener() {

			@Override
			public boolean onError(MediaPlayer mp, int what, int extra) {
				EventQueue.getDefault().postVideoStateChanged(handle, IX_WIDGET.MAW_VIDEO_VIEW_STATE_INTERRUPTED);
				return true;
			}
		});

		// Attach a media controller
		MediaController mc = new MediaController(activity);
		videoView.setMediaController(mc);

		return new VideoViewWidget( handle, videoView );
	}
}
