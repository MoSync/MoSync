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

package com.mosync.internal.android;

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_DATA_STREAM;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_AUDIO_PREPARED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_AUDIO_COMPLETED;

import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_INSTANCE;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_DATA;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_FILENAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_FILE;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_STREAMING_NOT_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_VOLUME_OUT_OF_RANGE;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_POSITION_OUT_OF_RANGE;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_SOUND_FORMAT;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_OUT_OF_MEMORY;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_ALREADY_PREPARED;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_IS_PLAYING;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Vector;

import com.mosync.internal.android.MoSyncSound.AudioStore;

import android.app.Activity;
import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.SoundPool;
import android.util.Log;

/**
 * Class that implements sound syscalls.
 */
public class MoSyncAudio implements OnCompletionListener, OnPreparedListener, OnErrorListener
{

	final static int MA_AUDIO_DATA_STREAM = 1;

	private MoSyncThread mMoSyncThread;

	private SoundPool mSoundPool = null;

	private MediaPlayer mMediaPlayer = null;
	int mStreamingAudioInstance = 0;

	int mNumAudioData = 1;
	int mNumAudioInstance = 1;

	int mPreparingAudioData = 0;
	int mActiveStreamingAudio = 0;

	Hashtable<Integer, AudioData> mAudioData =
			new Hashtable<Integer, AudioData>();

	Hashtable<Integer, AudioInstance> mAudioInstance =
			new Hashtable<Integer, AudioInstance>();

	final static int AUDIO_NOT_PREPARED = 0;
	final static int AUDIO_PREPARING = 1;
	final static int AUDIO_PREPARED = 2;
	final static int AUDIO_PREPARE_ERROR = 3;

	class AudioData
	{
		private String mFileName;
		private int mAudioID;
		private int mFlags;
		private int mLength;
		private boolean mStreaming;
		private int mPrepared;
		Vector<Integer> mInstances;

		public AudioData(
				String fileName,
				int audioID,
				int flags,
				int length,
				boolean streaming)
		{
			mFileName = fileName;
			mAudioID = audioID;
			mFlags = flags;
			mLength = length;
			mStreaming = streaming;
			mPrepared = AUDIO_NOT_PREPARED;
		}

		String getFileName()
		{
			return mFileName;
		}

		int getAudioID()
		{
			return mAudioID;
		}

		int getFlags()
		{
			return mFlags;
		}

		int getLength()
		{
			return mLength;
		}

		boolean isStreaming()
		{
			return mStreaming;
		}

		int getPreparedState()
		{
			return mPrepared;
		}

		void setPreparedState(int state)
		{
			mPrepared = state;
		}

		void addInstance(int instance)
		{
			mInstances.add(instance);
		}

		void removeInstance(int instance)
		{
			mInstances.remove(instance);
		}

		void release(Hashtable<Integer, AudioInstance> audioInstances)
		{
			Iterator<Integer> i = mInstances.iterator();

			while(i.hasNext())
			{
				Integer instance = i.next();
				if(audioInstances.contains(instance))
					audioInstances.remove(instance);
			}

			mInstances.clear();
		}

	};

	class AudioInstance
	{
		private AudioData mAudioData;
		private float mVolume;
		private int mLooping;
		private int mLoopsLeft;
		private boolean mPlaying;
		private boolean mPaused;
		private int mStreamID;

		public AudioInstance(AudioData audioData)
		{
			mAudioData = audioData;
			mVolume = 1.0f;
			mLooping = 0;
			mLoopsLeft = 1;
			mPlaying = false;
			mPaused = false;
			mStreamID = -1;
		}

		AudioData getAudioData()
		{
			return mAudioData;
		}

		void setVolume(float volume)
		{
			mVolume = volume;
		}

		float getVolume()
		{
			return mVolume;
		}

		void setLooping(int looping)
		{
			mLooping = looping;
			mLoopsLeft = looping+1;
		}

		int getLooping()
		{
			return mLooping;
		}

		boolean shouldLoop()
		{
			if(mLooping == 0)
				return false;

			mLoopsLeft--;
			if(mLoopsLeft == 0)
				return false;

			return true;
		}

		boolean isPlaying()
		{
			return mPlaying;
		}

		/**
		 *
		 * @param playing
		 */
		void setPlaying(boolean playing)
		{
			mLoopsLeft = mLooping+1;
			mPlaying = playing;
		}

		boolean isPaused()
		{
			return mPaused;
		}

		void setPaused(boolean paused)
		{
			mPaused = paused;
		}

		void setStreamID(int streamID)
		{
			mStreamID = streamID;
		}

		int getStreamID()
		{
			return mStreamID;
		}
	}

	int maAudioDataCreateFromResource(
			String mime,
			int data,
			int offset,
			int length,
			int flags)
	{

		AudioStore audioStore = mMoSyncThread.mMoSyncSound.obtainAudioStoreObject(data);

		if(audioStore != null)
			return maAudioDataCreateFromURL(mime,
											audioStore.mTemporaryFileName,
											flags);


		ByteBuffer audioData = mMoSyncThread.getBinaryResource(data);
		if (null != audioData)
		{

			if(offset > 0)
			{
				audioData.position(offset);
				audioData = audioData.slice();
			}

			if(-1 != length)
				audioData.limit(length);
		}
		else
		{

			byte[] byteData = mMoSyncThread.getUnloadedBinaryResourceAsByteArray(data);

			if(byteData == null)
				return MA_AUDIO_ERR_INVALID_DATA;

			audioData = ByteBuffer.wrap(byteData, offset, length);
			if(audioData == null)
				return MA_AUDIO_ERR_INVALID_DATA;
		}

		// Create file name.
		String fileName = "MOSYNCTEMP:audioPool" + mNumAudioData + ".tmp";

		int savedBytes = 0;

		try
		{
			// Create a temporary audio file.
			FileOutputStream audioFileOutputStream =
				getActivity().openFileOutput(
					fileName,
					Context.MODE_WORLD_READABLE | Context.MODE_WORLD_WRITEABLE);

			// Open a file channel to write audio data to.
			FileChannel channel = audioFileOutputStream.getChannel();

			// Write data.
			savedBytes = channel.write(audioData);

			// Close the channel, also closes the output stream.
			channel.close();

		}
		catch (Exception e)
		{
			e.printStackTrace();
			return MA_AUDIO_ERR_INVALID_DATA;
		}

		return audioCreateFromFile(fileName, flags, savedBytes);
	}

	int maAudioDataCreateFromURL(
		String mime,
		String url,
		int flags)
	{
		if(url.startsWith("http://") || url.startsWith("https://"))
			return audioCreateFromStream(url, flags, 0);

		return audioCreateFromFile(url, flags, 0);
	}

	int audioCreateFromFile(
		String fileName,
		int flags,
		int length)
	{
		if(flags != MA_AUDIO_DATA_STREAM)
		{
			if(mSoundPool == null)
			{
				mSoundPool = new SoundPool(10, AudioManager.STREAM_MUSIC, 0);
				if(mSoundPool == null)
					return MA_AUDIO_ERR_INVALID_INSTANCE;
			}
		}

		// pool ID -1 means that the file should be streamed from disk or http
		int poolID = -1;

		try
		{
			FileDescriptor fileDesc =
					getActivity().openFileInput(
					fileName).getFD();

			if(flags != MA_AUDIO_DATA_STREAM)
			{
					// Now open the file in the Sound Pool
					poolID = mSoundPool.load(fileDesc, 0, length, 1);
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
			return MA_AUDIO_ERR_INVALID_FILE;
		}

		// Add entry to audio resource table.
		mAudioData.put(mNumAudioData, new AudioData(fileName, poolID, flags, length, false));

		mNumAudioData++;
		return mNumAudioData-1;
	}

	int audioCreateFromStream(
			String fileName,
			int flags,
			int length)
	{
		// can only use a MA_AUDIO_DATA_STREAM for streaming sounds
		if(flags != MA_AUDIO_DATA_STREAM)
		{
			return MA_AUDIO_ERR_INVALID_FILE;
		}

		// Add entry to audio resource table.
		AudioData ad = new AudioData(fileName, 0, flags, length, true);
		mAudioData.put(mNumAudioData, ad);

		mNumAudioData++;
		return mNumAudioData-1;
	}

	int maAudioDataDestroy(int audioData)
	{
		AudioData d = mAudioData.remove(audioData);

		if(null == d)
			return -100;

		d.release(mAudioInstance);

/*
		String name = d.getFileName();
		if(name.startsWith("MOSYNCTEMP:audioPool"))
		{
			File f = new File(name);
			if(f.exists())
				f.delete();
		}
*/

		if(mAudioData.size() == 0)
		{
			// if all sounds has been deleted
			// it's safe to delete the sound pool.
			mSoundPool.release();
			mSoundPool = null;
		}

		return MA_AUDIO_ERR_OK;
	}

	int maAudioInstanceCreate(int audioData)
	{
		AudioData audioDataObject = mAudioData.get(audioData);

		if(audioDataObject == null)
			return -100;

		AudioInstance instance = new AudioInstance(audioDataObject);

		mAudioInstance.put(mNumAudioInstance, instance);

		mNumAudioInstance++;
		return mNumAudioInstance-1;
	}

	int maAudioInstanceDestroy(int audioInstance)
	{
		if(!mAudioInstance.contains(audioInstance))
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioInstance instance = mAudioInstance.get(audioInstance);

		if(instance.isPlaying())
			maAudioStop(audioInstance);

		AudioData data = instance.getAudioData();
		data.removeInstance(audioInstance);

		mAudioInstance.remove(audioInstance);

		return MA_AUDIO_ERR_OK;
	}

	AudioInstance getAudioInstance(int audio)
	{
		return mAudioInstance.get(audio);
	}

	int maAudioGetLength(int audio)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return -2;

		AudioData data = instance.getAudioData();

		if(MA_AUDIO_DATA_STREAM != data.getFlags())
			return -2;

		return mMediaPlayer.getDuration();
	}

	int maAudioSetNumberOfLoops(int audio, int loops)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioData data = instance.getAudioData();

		instance.setLooping(loops);

		if(MA_AUDIO_DATA_STREAM == data.getFlags())
		{

			if(loops == -1)
				mMediaPlayer.setLooping(true);
			else if(loops == 0)
				mMediaPlayer.setLooping(false);
		}
		else
		{
			instance.setLooping(loops);
		}

		return MA_AUDIO_ERR_OK;
	}

	int maAudioPrepare(int audio, int async)
	{
		if(mPreparingAudioData != 0)
			return MA_AUDIO_ERR_INVALID_DATA;

		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return 0;

		if(instance.isPaused())
			return 0;

		AudioData data = instance.getAudioData();

		if(data.getPreparedState() == AUDIO_PREPARED)
			return MA_AUDIO_ERR_ALREADY_PREPARED;

		if(MA_AUDIO_DATA_STREAM != data.getFlags())
		{
			data.setPreparedState(AUDIO_PREPARED);
			return MA_AUDIO_ERR_OK;
		}

		mPreparingAudioData = audio;

		try
		{
			if(prepareMediaPlayer(audio))
			{
				if(0 == async)
				{
					mMediaPlayer.prepare();
					data.setPreparedState(AUDIO_PREPARED);
					return MA_AUDIO_ERR_OK;
				}
				else
				{
					mMediaPlayer.setOnPreparedListener(this);
					mMediaPlayer.prepareAsync();
				}
			}
		}
		catch(Exception e)
		{
			data.setPreparedState(AUDIO_PREPARE_ERROR);
			return MA_AUDIO_ERR_INVALID_DATA;
		}

		return 0;
	}

	boolean prepareMediaPlayer(int audio)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return false;

		AudioData data = instance.getAudioData();

		mStreamingAudioInstance = audio;

		if(null == mMediaPlayer)
		{
			mMediaPlayer = new MediaPlayer();
			mMediaPlayer.setOnCompletionListener(this);
		}
		else
		{
			if(mMediaPlayer.isPlaying())
				mMediaPlayer.stop();
			mMediaPlayer.reset();
		}

		try
		{
			if(data.isStreaming())
			{
				mMediaPlayer.setDataSource(data.getFileName());
				mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
			}
			else
			{
				FileDescriptor fileDesc =
					getActivity().openFileInput(
						data.getFileName()).getFD();

				if (null == fileDesc)
					return false;

				mMediaPlayer.setDataSource(fileDesc, 0, data.getLength());
			}
		}
		catch (Exception e)
		{
			return false;
		}

		return true;
	}

	int maAudioPlay(int audio)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioData data = instance.getAudioData();

		if(instance.isPaused())
		{
			instance.setPaused(false);

			if(MA_AUDIO_DATA_STREAM == data.getFlags())
			{
				mMediaPlayer.start();
			}
			else
			{
				mSoundPool.resume(instance.getStreamID());
			}
			return MA_AUDIO_ERR_OK;
		}

		if(MA_AUDIO_DATA_STREAM == data.getFlags())
		{
			try
			{
				int pstate = data.getPreparedState();

				if(AUDIO_PREPARING == pstate ||
					AUDIO_PREPARE_ERROR == pstate)
					return -100;

				if(AUDIO_NOT_PREPARED == pstate)
				{
					if(!prepareMediaPlayer(audio))
						return -100;

					mMediaPlayer.prepare();
					data.setPreparedState(AUDIO_PREPARED);
				}

				mMediaPlayer.setVolume(instance.getVolume(), instance.getVolume());
				mMediaPlayer.start();

				mActiveStreamingAudio = audio;
			}
			catch (Exception e)
			{
				Log.e("maAudioPlay","Couldn't play STREAM " + audio);
				return -100;
			}

		}
		else
		{
			int streamID = mSoundPool.play(data.getAudioID(), instance.getVolume(),
					instance.getVolume(), 1, instance.getLooping(), 1.0f);

			instance.setStreamID(streamID);
		}

		instance.setPlaying(true);

		return MA_AUDIO_ERR_OK;
	}

	int maAudioSetPosition(int audio, int milliseconds)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioData data = instance.getAudioData();

		// Only available for streamed sounds
		if((MA_AUDIO_DATA_STREAM == data.getFlags())
			&& (null != mMediaPlayer))
		{
			mMediaPlayer.seekTo(milliseconds);
		}

		return MA_AUDIO_ERR_OK;
	}

	int maAudioGetPosition(int audio)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioData data = instance.getAudioData();

		// Only streaming sounds has a position
		if((MA_AUDIO_DATA_STREAM == data.getFlags())
			&& (null != mMediaPlayer))
		{
			return mMediaPlayer.getCurrentPosition();
		}

		return -100;
	}

	int maAudioSetVolume(int audio, float volume)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		if((volume < 0.0f) || (audio > 100.0f))
			return MA_AUDIO_ERR_VOLUME_OUT_OF_RANGE;

		AudioData data = instance.getAudioData();

		instance.setVolume(volume);

		if((MA_AUDIO_DATA_STREAM == data.getFlags())
				&& (null != mMediaPlayer))
		{
			mMediaPlayer.setVolume(instance.getVolume(), instance.getVolume());
		}
		else
		{
			int id = instance.getStreamID();
			if(id == -1)
				return -100;

			mSoundPool.setVolume(id, instance.getVolume(), instance.getVolume());
		}

		return MA_AUDIO_ERR_OK;
	}

	int maAudioPause(int audio)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioData data = instance.getAudioData();

		//if((false == instance.isPlaying()) || (true == instance.isPaused()))
		if(true == instance.isPaused())
			return MA_AUDIO_ERR_OK;

		if(MA_AUDIO_DATA_STREAM == data.getFlags())
		{
			if(null != mMediaPlayer)
				mMediaPlayer.pause();
		}
		else
		{
			int streamID = instance.getStreamID();
			if(streamID != -1)
				mSoundPool.pause(streamID);
		}

		instance.setPaused(true);

		return MA_AUDIO_ERR_OK;
	}

	int maAudioStop(int audio)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioData data = instance.getAudioData();

		if((true == instance.isPlaying()) || (true == instance.isPaused()))
			return MA_AUDIO_ERR_OK;

		if((MA_AUDIO_DATA_STREAM == data.getFlags())
				&& (null != mMediaPlayer))
		{
			mMediaPlayer.stop();
			mMediaPlayer.reset();

			mActiveStreamingAudio = 0;
		}
		else
		{
			int streamID = instance.getStreamID();
			if(streamID == -1)
				return -100;

			mSoundPool.stop(streamID);
		}

		instance.setPlaying(false);

		return MA_AUDIO_ERR_OK;
	}

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncAudio(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	public void onCompletion(MediaPlayer mp)
	{
		if(mActiveStreamingAudio > 0)
		{
			AudioInstance audio = getAudioInstance(mActiveStreamingAudio);

			if(audio.shouldLoop())
			{
				if( MA_AUDIO_ERR_OK == maAudioPlay(mActiveStreamingAudio))
				return;

				// oops.. we got an error during playback
				mActiveStreamingAudio = 0;
			}
		}

		int[] event = new int[2];
		event[0] = EVENT_TYPE_AUDIO_COMPLETED;
		event[1] = mActiveStreamingAudio;
		mMoSyncThread.postEvent(event);

	}

	public void onPrepared(MediaPlayer mp)
	{
		if(mPreparingAudioData != 0)
		{
			AudioData audioData = mAudioData.get(mPreparingAudioData);
			audioData.setPreparedState(AUDIO_PREPARED);
		}
		else
			mPreparingAudioData = -1;

		int[] event = new int[2];
		event[0] = EVENT_TYPE_AUDIO_PREPARED;
		event[1] = mPreparingAudioData;
		mMoSyncThread.postEvent(event);

		mPreparingAudioData = 0;
	}

	public boolean onError(MediaPlayer mp, int what, int extra)
	{
		AudioData audioData = mAudioData.get(mPreparingAudioData);

		audioData.setPreparedState(AUDIO_PREPARE_ERROR);
		mPreparingAudioData = 0;

		int[] event = new int[2];
		event[0] = EVENT_TYPE_AUDIO_PREPARED;
		event[1] = -1;
		mMoSyncThread.postEvent(event);

		return true;
	}

}
