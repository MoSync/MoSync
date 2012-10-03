/* Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Threading;
using System.IO;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Media;
using Microsoft.Phone.Controls;

namespace MoSync
{
	class AudioDataSoundEffect : IAudioData
	{
		private SoundEffect mSoundEffect = null;

		public AudioDataSoundEffect(SoundEffect soundEffect)
		{
			this.mSoundEffect = soundEffect;
		}

		public IAudioInstance CreateInstance()
		{
			return new AudioInstanceSoundEffect(mSoundEffect);
		}

		public void Dispose()
		{
			if (mSoundEffect != null)
			{
				mSoundEffect.Dispose();
			}

			mSoundEffect = null;
		}
	}

	// this class is used to create a one shot timer that
	// is triggered the last time a sound loops in order to turn off looping then
	// and allow a fixed amount of loops
	class LoopHelper
	{
		private int mNumLoops = 0;
		private DispatcherTimer mNumLoopsTimer = null;
		private IAudioInstance mAudioInstance = null;
		private Action mCallback = null;

		public LoopHelper(IAudioInstance audioInstance, Action callback)
		{
			mAudioInstance = audioInstance;
			mCallback = callback;
		}

		public void SetNumLoops(int loops)
		{
			mNumLoops = loops;
		}

		void StopLooping(object o, EventArgs a)
		{
			mCallback();
			mNumLoopsTimer.Stop();
		}

		public void Start()
		{
			if (mNumLoopsTimer == null && mNumLoops > 0)
			{
				mNumLoopsTimer = new DispatcherTimer();
				mNumLoopsTimer.Tick += new EventHandler(StopLooping);
				int len = mAudioInstance.GetLength();
				mNumLoopsTimer.Interval = TimeSpan.FromMilliseconds(mNumLoops * len - len / 2);
				mNumLoopsTimer.Start();
			}

			if (mNumLoopsTimer != null)
				mNumLoopsTimer.Start();
		}

		public void Stop()
		{
			if (mNumLoopsTimer != null)
			{
				mNumLoopsTimer.Stop();
				mNumLoopsTimer = null;
			}
		}

		public void Pause()
		{
			if (mNumLoopsTimer != null)
			{
				mNumLoopsTimer.Stop();
			}
		}
	}

	class AudioInstanceSoundEffect : IAudioInstance
	{
		private SoundEffectInstance mSoundEffectInstance = null;
		private SoundEffect mSoundEffect = null;
		private int mNumLoops = 0;
		private LoopHelper mLoopHelper = null;

		public AudioInstanceSoundEffect(SoundEffect soundEffect)
		{
			this.mSoundEffect = soundEffect;
			this.mSoundEffectInstance = soundEffect.CreateInstance();

			mLoopHelper = new LoopHelper(this, () =>
				{
					this.mSoundEffectInstance.IsLooped = false;
				});
		}

		public void Play()
		{
			if (mNumLoops != 0)
			{
				mSoundEffectInstance.IsLooped = true;
			}
			else
			{
				mSoundEffectInstance.IsLooped = false;
			}
			mLoopHelper.Start();
			mSoundEffectInstance.Play();
		}

		public void Stop()
		{
			mLoopHelper.Stop();
			mSoundEffectInstance.Stop();
		}

		public void Pause()
		{
			mLoopHelper.Pause();
			mSoundEffectInstance.Pause();
		}

		public void SetPosition(int _millis)
		{
			if (_millis == 0)
			{
				SoundState prevState = mSoundEffectInstance.State;
				Stop();
				if (prevState == SoundState.Playing)
				{
					Play();
				}

				return;
			}

			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public int GetPosition()
		{
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public int GetLength()
		{
			return (int)mSoundEffect.Duration.TotalMilliseconds;
		}

		public void SetVolume(float vol)
		{
			mSoundEffectInstance.Volume = vol;
		}

		public void SetNumberOfLoops(int numLoops)
		{
			mNumLoops = numLoops;
			mLoopHelper.SetNumLoops(numLoops);
		}

		public void Dispose()
		{
			if (mSoundEffectInstance != null)
			{
				mSoundEffectInstance.Dispose();
			}

			mSoundEffectInstance = null;
		}

		public void Prepare(Action async)
		{
			if (async != null)
			{
				async();
			}
		}

		public void Update()
		{
		}
	}

	class AudioDataMediaPlayer : IAudioData
	{
		private Uri mPathToMedia = null;

		public AudioDataMediaPlayer(Uri pathToMedia)
		{
			this.mPathToMedia = pathToMedia;
		}

		public IAudioInstance CreateInstance()
		{
			if(mPathToMedia == null)
			{
				throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_DATA);
			}

			return new AudioInstanceMediaPlayer(mPathToMedia);
		}

		public void Dispose()
		{
			mPathToMedia = null;
		}
	}

	class AudioInstanceMediaPlayer : IAudioInstance
	{
		private Uri mUri = null;
		private Song mSong = null;
		private int mNumLoops = 0;

		public AudioInstanceMediaPlayer(Uri uri)
		{
			this.mUri = uri;
			CreateSong();
		}

		private void CreateSong()
		{
			this.mSong = Song.FromUri(mUri.ToString(), mUri);
		}

		public void Play()
		{
			if (mSong == null)
			{
				CreateSong();
			}

			if (MediaPlayer.Queue.ActiveSong != mSong)
			{
				MediaPlayer.Stop();
			}

			MediaPlayer.Play(mSong);
		}

		public void Stop()
		{
			MediaPlayer.Stop();
			if (mSong != null)
			{
				Dispose();
			}
		}

		public void Pause()
		{
			MediaPlayer.Pause();
		}

		public void SetPosition(int _millis)
		{
			if (_millis == 0)
			{
				MediaState prevState = MediaPlayer.State;
				Stop();
				if (prevState == MediaState.Playing)
				{
					Play();
				}

				return;
			}

			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public int GetPosition()
		{
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public int GetLength()
		{
			if (mSong == null)
			{
				throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_DATA);
			}

			return (int)mSong.Duration.TotalMilliseconds;
		}

		public void SetVolume(float vol)
		{
			MediaPlayer.Volume = vol;
		}

		public void SetNumberOfLoops(int numLoops)
		{
			mNumLoops = numLoops;
		}

		public void Dispose()
		{
			if (mSong != null)
			{
				mSong.Dispose();
			}

			mSong = null;
		}

		public void Prepare(Action async)
		{
			if (mSong == null)
			{
				CreateSong();
			}

			if (async != null)
			{
				async();
			}
		}

		public void Update()
		{
		}
	}


	class AudioDataMediaElement : IAudioData
	{
		private Uri mPathToMedia = null;
		private Stream mStream = null;

		public AudioDataMediaElement(Uri pathToMedia)
		{
			this.mPathToMedia = pathToMedia;
		}

		public AudioDataMediaElement(Stream stream)
		{
			this.mStream = stream;
		}


		public IAudioInstance CreateInstance()
		{
			if (mPathToMedia == null && mStream == null)
			{
				throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_DATA);
			}

			if (mPathToMedia != null)
				return new AudioInstanceMediaElement(mPathToMedia);
			else
				return new AudioInstanceMediaElement(mStream);
		}

		public void Dispose()
		{
			mPathToMedia = null;
		}
	}

	class AudioInstanceMediaElement : IAudioInstance
	{
		private Uri mUri = null;
		private Stream mStream = null;
		private MediaElement mMediaElement = null;
		private int mNumLoops = 0;

		public AudioInstanceMediaElement(Uri uri)
		{
			this.mUri = uri;
			CreateMediaElement();
		}

		public AudioInstanceMediaElement(Stream stream)
		{
			this.mStream = stream;
			CreateMediaElement();
		}

		private void CreateMediaElement()
		{
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				this.mMediaElement = new MediaElement();
				this.mMediaElement.AutoPlay = false;
				if (mUri != null)
					this.mMediaElement.Source = mUri;
				else if (mStream != null)
					this.mMediaElement.SetSource(mStream);
			});
		}

		public void Play()
		{
			if (mMediaElement == null)
			{
				CreateMediaElement();
			}

			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				mMediaElement.Play();
			});
		}

		public void Stop()
		{
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				mMediaElement.Stop();
			});
		}

		public void Pause()
		{
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				mMediaElement.Pause();
			});
		}

		public void SetPosition(int _millis)
		{
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				mMediaElement.Position = TimeSpan.FromMilliseconds((double)_millis);
			});
		}

		public int GetPosition()
		{
			int pos = 0;
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				pos = (int)mMediaElement.Position.TotalMilliseconds;
			});
			return pos;
		}

		public int GetLength()
		{
			int len = 0;
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				len = (int)mMediaElement.NaturalDuration.TimeSpan.TotalMilliseconds;
			});
			return len;
		}

		public void SetVolume(float vol)
		{
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				mMediaElement.Volume = vol;
			});
		}

		public void SetNumberOfLoops(int numLoops)
		{
			mNumLoops = numLoops;
		}

		public void Dispose()
		{
			mMediaElement = null;
		}

		public void Prepare(Action async)
		{
			if (mMediaElement == null)
			{
				CreateMediaElement();
			}

			if (async != null)
			{
				async();
			}
		}

		public void Update()
		{
		}
	}

	public class AudioInstanceDynamic : IAudioInstance
	{
		private DynamicSoundEffectInstance mSoundEffectInstance = null;
		private Action mOnBufferNeededCallback = null;

		private byte[] mBuffer = null;

		private void BufferNeeded(object sender, EventArgs a)
		{
			if (mOnBufferNeededCallback != null)
				mOnBufferNeededCallback();
		}

		public AudioInstanceDynamic(int sampleRate, int numChannels, int bufferSize)
		{
			AudioChannels ac = AudioChannels.Stereo;
			if (numChannels == 1)
			{
				ac = AudioChannels.Mono;
			}
			else if (numChannels != 2)
			{
				throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_SOUND_FORMAT);
			}

			this.mSoundEffectInstance = new DynamicSoundEffectInstance(sampleRate, ac);
			this.mSoundEffectInstance.BufferNeeded += new EventHandler<EventArgs>(BufferNeeded);

			mBuffer = new byte[bufferSize * sizeof(short)];
		}

		public void Play()
		{
			mSoundEffectInstance.Play();
		}

		public void Stop()
		{
			mSoundEffectInstance.Stop();
		}

		public void Pause()
		{
			mSoundEffectInstance.Pause();
		}

		public void SetPosition(int _millis)
		{
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public int GetPosition()
		{
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public int GetLength()
		{
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public void SetVolume(float vol)
		{
			mSoundEffectInstance.Volume = vol;
		}

		public void SetNumberOfLoops(int numLoops)
		{
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public void Dispose()
		{
			if (mSoundEffectInstance != null)
			{
				mSoundEffectInstance.Dispose();
			}

			mOnBufferNeededCallback = null;
			mSoundEffectInstance = null;
		}

		public void Prepare(Action async)
		{
			if (async != null)
			{
				async();
			}
		}

		public int GetPendingBufferCount()
		{
			return mSoundEffectInstance.PendingBufferCount;
		}

		public void SetOnBufferNeededCallback(Action a)
		{
			mOnBufferNeededCallback = a;
		}

		public void Update()
		{
		}

		public void SubmitBuffer(byte[] buf, int offset, int length)
		{
			System.Buffer.BlockCopy(buf, offset, mBuffer, 0, length);

			int halfLen = length/2;
			mSoundEffectInstance.SubmitBuffer(mBuffer, 0, halfLen);
			mSoundEffectInstance.SubmitBuffer(mBuffer, halfLen, halfLen);
		}
	}


	public class Audio
	{
		public static IAudioData FromUrlOrFilePath(String url, bool shouldStream)
		{
			if (shouldStream)
			{
				UriKind uriKind = UriKind.Absolute;

				if (url.StartsWith("file://") == true || url.Contains("://") == false)
				{
					// gaaah need to use media element here.....
					// Media player doesn't seem to be able to stream from isostore..
					url = url.Replace("file://", "");
					//url = url.Insert(0, "isostore:");
					uriKind = UriKind.Relative;
					return new AudioDataMediaElement(new Uri(url, uriKind));
				}

				Uri uri = new Uri(url, uriKind);
				return new AudioDataMediaPlayer(uri);
			}
			else
			{
				if (url.StartsWith("file://") == true || url.Contains("://") == false)
				{
					String filepath = url.Replace("file://", "");
					FileModule.File file = new FileModule.File(filepath, FileAccess.Read);
					file.TryOpen();
					if (file.FileStream == null)
					{
						throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_FILE);
					}
					IAudioData ret = FromStream(file.FileStream, false);
					file.Close();
					return ret;
				}
				else
				{
					// maybe create a better error code for this..
					// we don't support anything else than loading from files directly to memory.
					throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_FILENAME);
				}
			}

			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
		}

		public static IAudioData FromStream(Stream stream, bool shouldStream)
		{
			if (shouldStream)
			{
				throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_STREAMING_NOT_SUPPORTED);
			}
			else
			{
				try
				{
					SoundEffect se = SoundEffect.FromStream(stream);
					return new AudioDataSoundEffect(se);
				}
				catch (Exception)
				{
					throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_DATA);
				}
			}
		}

		public static AudioInstanceDynamic CreateDynamic(int _sampleRate, int _numChannels, int _bufferSize)
		{
			try
			{
				return new AudioInstanceDynamic(_sampleRate, _numChannels, _bufferSize);
			}
			catch (Exception)
			{
				throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_SOUND_FORMAT);
			}

		}
	}
}
