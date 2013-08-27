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

package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_AUDIO_PREPARED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_AUDIO_COMPLETED;

import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_INSTANCE;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_DATA;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_INVALID_FILE;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_VOLUME_OUT_OF_RANGE;
import static com.mosync.internal.generated.MAAPI_consts.MA_AUDIO_ERR_ALREADY_PREPARED;

import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.ShortBuffer;
import java.nio.channels.FileChannel;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Vector;

import com.mosync.internal.android.MoSyncSound.AudioStore;

import android.app.Activity;
import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.AudioTrack;
import android.media.AudioFormat;
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

	private AudioTrack mAudioTrack = null;

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
		private boolean mDynamic;
		private int mSampleRate;
		private int mChannels;
		private int mBufferSize;

		public AudioInstance(AudioData audioData)
		{
			mAudioData = audioData;
			mDynamic = false;
			init();
		}

		public AudioInstance(int sampleRate, int channels, int bufferSize)
		{
			mDynamic = true;
			mSampleRate = sampleRate;
			mChannels = channels;
			mBufferSize = bufferSize;
			init();
		}

		private void init()
		{
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
		void setPlaying(boolean playing, boolean looping)
		{
			if (!looping)
			{
				mLoopsLeft = mLooping+1;
			}
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

		boolean isDynamic()
		{
			return mDynamic;
		}
	}

	int maAudioDataCreateFromResource(
			String mime,
			int data,
			int offset,
			int length,
			int flags)
	{

		AudioStore audioStore = mMoSyncThread.mMoSyncSound.
			obtainAudioStoreObject(data, offset);

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

//		d.release(mAudioInstance);

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
			if (d.getFlags() != MA_AUDIO_DATA_STREAM)
			{
				mSoundPool.release();
				mSoundPool = null;
			}
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

	int maAudioInstanceCreateDynamic(int sampleRate, int numChannels, int bufferSize)
	{
		if(numChannels<1 || numChannels>2)
			return -100;

		int channelConfig = AudioFormat.CHANNEL_OUT_MONO;
		if(numChannels == 2)
			channelConfig = AudioFormat.CHANNEL_OUT_STEREO;

		int minSize =AudioTrack.getMinBufferSize( sampleRate, channelConfig, AudioFormat.ENCODING_PCM_16BIT );

		if(minSize > bufferSize)
			return -100;

		mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
									sampleRate,
									channelConfig,
									AudioFormat.ENCODING_PCM_16BIT,
									bufferSize,
									AudioTrack.MODE_STREAM);

		if(mAudioTrack == null)
			return -100;

		AudioInstance instance = new AudioInstance(sampleRate, numChannels, bufferSize);

		mAudioInstance.put(mNumAudioInstance, instance);

		mNumAudioInstance++;
		return mNumAudioInstance-1;
	}

	int maAudioSubmitBuffer(int audioInstance, int mem, int memSize)
	{
		if(!mAudioInstance.contains(audioInstance))
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		ByteBuffer slice = mMoSyncThread.getMemorySlice(mem, memSize);
		ShortBuffer shortSlice = slice.asShortBuffer();
		short[] array = shortSlice.array();

		if(array == null)
			return -100;

		mAudioTrack.write(array, 0, memSize);

		return MA_AUDIO_ERR_OK;
	}

	int maAudioGetPendingBufferCount(int audioInstance)
	{
		if(!mAudioInstance.contains(audioInstance))
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		return MA_AUDIO_ERR_OK;
	}

	int maAudioInstanceDestroy(int audioInstance)
	{
		if(!mAudioInstance.contains(audioInstance))
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		AudioInstance instance = mAudioInstance.get(audioInstance);

		if(instance.isPlaying())
			maAudioStop(audioInstance);

//		AudioData data = instance.getAudioData();
//		data.removeInstance(audioInstance);

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
//		else
//		{
//			instance.setLooping(loops);
//		}

		return MA_AUDIO_ERR_OK;
	}

	int maAudioPrepare(int audio, int async)
	{
		if(mPreparingAudioData != 0)
			return MA_AUDIO_ERR_INVALID_DATA;

		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
		{
			return MA_AUDIO_ERR_INVALID_INSTANCE;
		}

		if(instance.isPaused())
		{
			if (1 == async)
			{
				postAudioEvent(EVENT_TYPE_AUDIO_PREPARED, audio);
			}
			return MA_AUDIO_ERR_OK;
		}

		AudioData data = instance.getAudioData();

		if(data.getPreparedState() == AUDIO_PREPARED)
			return MA_AUDIO_ERR_ALREADY_PREPARED;

		if(MA_AUDIO_DATA_STREAM != data.getFlags())
		{
			data.setPreparedState(AUDIO_PREPARED);
			if (1 == async)
			{
				postAudioEvent(EVENT_TYPE_AUDIO_PREPARED, audio);
			}
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
		return maAudioPlay(audio, false);
	}

	int maAudioPlay(int audio, boolean looping)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		if(instance.isDynamic())
		{

		}
		else
		{
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
		}

		instance.setPlaying(true, looping);

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

		if(instance.isDynamic())
		{

		}
		else
		{
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
		}

		instance.setPaused(true);

		return MA_AUDIO_ERR_OK;
	}

	int maAudioStop(int audio)
	{
		AudioInstance instance = getAudioInstance(audio);
		if(null == instance)
			return MA_AUDIO_ERR_INVALID_INSTANCE;

		if(instance.isDynamic())
		{

		}
		else
		{
			AudioData data = instance.getAudioData();

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
		}

		instance.setPlaying(false, false);

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
				if( MA_AUDIO_ERR_OK == maAudioPlay(mActiveStreamingAudio, true))
					return;

				// oops.. we got an error during playback
				mActiveStreamingAudio = 0;
			}
		}

		postAudioEvent(EVENT_TYPE_AUDIO_COMPLETED, mActiveStreamingAudio);
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

		postAudioEvent(EVENT_TYPE_AUDIO_PREPARED, mPreparingAudioData);

		mPreparingAudioData = 0;
	}

	private void postAudioEvent(int type, int data)
	{
		int[] event = new int[2];
		event[0] = type;
		event[1] = data;
		mMoSyncThread.postEvent(event);
	}

	public boolean onError(MediaPlayer mp, int what, int extra)
	{
		AudioData audioData = mAudioData.get(mPreparingAudioData);

		audioData.setPreparedState(AUDIO_PREPARE_ERROR);
		mPreparingAudioData = 0;

		postAudioEvent(EVENT_TYPE_AUDIO_PREPARED, -1);

		return true;
	}

}
