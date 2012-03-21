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
using System.Collections.Generic;
using System.IO;

namespace MoSync
{
    public class AudioModule : IIoctlModule, ISyscallModule
    {
		public void Init(Syscalls syscalls, Core core, Runtime runtime)
		{
			syscalls.maSoundPlay = delegate(int _sound_res, int _offset, int _size)
			{
				// not implemented, but I don't wanna throw exceptions.
				return -1;
			};
		}

		List<IAudioData> mAudioData = new List<IAudioData>();
		List<IAudioInstance> mAudioInstances = new List<IAudioInstance>();

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
			ioctls.maAudioDataCreateFromURL = delegate(int _mime, int _url, int _flags)
			{
				int ret = MoSync.Constants.MA_AUDIO_ERR_GENERIC;

				try
				{
					String url = core.GetDataMemory().ReadStringAtAddress(_url);
					IAudioData ad = Audio.FromUrlOrFilePath(url, (_flags & MoSync.Constants.MA_AUDIO_DATA_STREAM) != 0);
					mAudioData.Add(ad);
					ret = mAudioData.Count - 1;
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return ret;
			};

            ioctls.maAudioDataCreateFromResource = delegate(int _mime, int _data, int _offset, int _length, int _flags)
            {
				int ret = MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				try
				{
					Resource audiores = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
					BoundedStream s = new BoundedStream((Stream)audiores.GetInternalObject(), _offset, _length);
					IAudioData ad = Audio.FromStream(s, (_flags & MoSync.Constants.MA_AUDIO_DATA_STREAM) != 0);
					mAudioData.Add(ad);
					ret =  mAudioData.Count - 1;
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return ret;
            };

            ioctls.maAudioDataDestroy = delegate(int _audioData)
            {
				try
				{
					IAudioData ad = mAudioData[_audioData];
					ad.Dispose();
					mAudioData[_audioData] = null;
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
            };

			ioctls.maAudioPrepare = delegate(int _audioInstance, int async)
			{
				try
				{
					IAudioInstance ad = mAudioInstances[_audioInstance];
					if (async == 0)
					{
						ad.Prepare(null);
					}
					else
					{
						ad.Prepare(() =>
							{
								// Send initialized event.
								MoSync.Memory eventData = new MoSync.Memory(8);
								eventData.WriteInt32(MoSync.Struct.MAEvent.type, MoSync.Constants.EVENT_TYPE_AUDIO_PREPARED);
								eventData.WriteInt32(MoSync.Struct.MAEvent.audioInstance, _audioInstance);
								runtime.PostEvent(new Event(eventData));
							}
						);
					}
					mAudioData[_audioInstance] = null;
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;

			};

            ioctls.maAudioInstanceCreate = delegate(int _audioData)
            {
				int ret = MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				try
				{
					IAudioData ad = mAudioData[_audioData];
					mAudioInstances.Add(ad.CreateInstance());
					ret = mAudioInstances.Count - 1;
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return ret;
            };

            ioctls.maAudioInstanceDestroy = delegate(int _audioInstance)
            {
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ai.Dispose();
					mAudioInstances[_audioInstance] = null;
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
            };

            ioctls.maAudioPlay = delegate(int _audioInstance)
            {
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ai.Play();
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
            };


			ioctls.maAudioStop = delegate(int _audioInstance)
			{
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ai.Stop();
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
			};

			ioctls.maAudioPause = delegate(int _audioInstance)
			{
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ai.Pause();
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
			};

			ioctls.maAudioSetNumberOfLoops = delegate(int _audioInstance, int loops)
			{
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ai.SetNumberOfLoops(loops);
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
			};


			// SoundEffectInstances nor the MediaPlayer doesn't support setting position,
			// however we can make a special case where the sound is reset if _milliseconds equals to zero.
			// We could implement a better SoundEffectInstance using DynamicSoundEffectInstance
			// parsing wavefiles ourselves.. But that would require some work.

			ioctls.maAudioSetPosition = delegate(int _audioInstance, int _milliseconds)
			{
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ai.SetPosition(_milliseconds);
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
			};


			// SoundEffectInstances doesnt support getting the location of the sound
			// this of course could be approximated by saving a time stamp when the sound
			// starts to play, buuut no.
			ioctls.maAudioGetPosition = delegate(int _audioInstance)
			{
				int ret = MoSync.Constants.MA_AUDIO_ERR_OK;
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ret = ai.GetPosition();
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return ret;
			};

			ioctls.maAudioGetLength = delegate(int _audioInstance)
			{
				int ret = MoSync.Constants.MA_AUDIO_ERR_OK;
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ret = ai.GetLength();
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return ret;
			};

			ioctls.maAudioSetVolume = delegate(int _audioInstance, float volume)
			{
				try
				{
					IAudioInstance ai = mAudioInstances[_audioInstance];
					ai.SetVolume(volume);
				}
				catch (MoSync.Util.ReturnValueException rve)
				{
					return rve.result;
				}
				catch (Exception)
				{
					return MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				}

				return MoSync.Constants.MA_AUDIO_ERR_OK;
			};
        }
    }
}
