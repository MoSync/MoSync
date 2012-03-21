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
using System;
using System.IO;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Media;

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

	class AudioInstanceSoundEffect : IAudioInstance
	{
		private SoundEffectInstance mSoundEffectInstance = null;
		private SoundEffect mSoundEffect = null;

		public AudioInstanceSoundEffect(SoundEffect soundEffect)
		{
			this.mSoundEffect = soundEffect;
			this.mSoundEffectInstance = soundEffect.CreateInstance();
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
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
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
			throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_GENERIC);
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
	}

	public class Audio
	{
		public static IAudioData FromUrlOrFilePath(String url, bool shouldStream)
		{
			if (shouldStream)
			{
				UriKind uriKind = UriKind.Absolute;

				if (url.StartsWith("file://") == true)
				{
					url = url.Replace("file://", "");
					uriKind = UriKind.Relative;
				}

				return new AudioDataMediaPlayer(new Uri(url, uriKind));
			}
			else
			{
				if (url.StartsWith("file://") == true)
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
				SoundEffect se = SoundEffect.FromStream(stream);
				return new AudioDataSoundEffect(se);
			}
		}
	}
}
