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
using System.Threading;

namespace MoSync
{
	public class AudioModule : IIoctlModule
	{

		List<IAudioData> mAudioData = new List<IAudioData>();
		List<IAudioInstance> mAudioInstances = new List<IAudioInstance>();

#if false
		AudioInstanceUpdater mAudioInstanceUpdater = null;
		public class AudioInstanceUpdater
		{
			private bool mRunning = true;

			List<IAudioInstance> mAudioInstances = null;

			public AudioInstanceUpdater(List<IAudioInstance> audioInstances)
			{
				this.mAudioInstances = audioInstances;
			}


			public void SetIsRunning(bool r)
			{
				mRunning = r;
			}

			public void Loop()
			{
				while (mRunning == true)
				{
					lock (mAudioInstances)
					{
						for (int i = 0; i < mAudioInstances.Count; i++)
						{
							mAudioInstances[i].Update();
						}
					}

					Thread.Sleep(1);
				}
			}
		}
#endif


		public void Init(Ioctls ioctls, Core core, Runtime runtime)
		{

#if false
			mAudioInstanceUpdater = new AudioInstanceUpdater(mAudioInstances);
			Thread thread = new Thread(new ThreadStart(mAudioInstanceUpdater.Loop));
			thread.Start();
#endif
			ioctls.maAudioDataCreateFromURL = delegate(int _mime, int _url, int _flags)
			{
				int ret = MoSync.Constants.MA_AUDIO_ERR_GENERIC;

				try
				{
					String url = core.GetDataMemory().ReadStringAtAddress(_url);
					String mime = core.GetDataMemory().ReadStringAtAddress(_mime);
					bool shouldStream = (_flags & MoSync.Constants.MA_AUDIO_DATA_STREAM) != 0;
					IAudioData ad;
					if (mime == "audio/mpeg")
					{
						ad = Mp3Audio.FromUrlOrFilePath(url, shouldStream);
					}
					else
					{
						ad = Audio.FromUrlOrFilePath(url, shouldStream);
					}
					lock (mAudioData)
					{
						mAudioData.Add(ad);
						ret = mAudioData.Count - 1;
					}
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
					String mime = core.GetDataMemory().ReadStringAtAddress(_mime);
					IAudioData ad;
					if (mime == "audio/mpeg")
					{
						ad = Mp3Audio.FromStream(s);
					}
					else
					{
						ad = Audio.FromStream(s, (_flags & MoSync.Constants.MA_AUDIO_DATA_STREAM) != 0);
					}
					lock (mAudioData)
					{
						mAudioData.Add(ad);
						ret = mAudioData.Count - 1;
					}
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
					lock (mAudioData)
					{
						IAudioData ad = mAudioData[_audioData];
						ad.Dispose();
						mAudioData[_audioData] = null;
					}
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
					lock (mAudioInstances)
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
					}
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
					lock (mAudioInstances)
					{
						IAudioData ad = mAudioData[_audioData];
						mAudioInstances.Add(ad.CreateInstance());
						ret = mAudioInstances.Count - 1;
					}
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

			ioctls.maAudioInstanceCreateDynamic = delegate(int _sampleRate, int _numChannels, int _bufferSize)
			{
				int ret = MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				try
				{
					lock (mAudioInstances)
					{
						AudioInstanceDynamic aid = Audio.CreateDynamic(_sampleRate, _numChannels, _bufferSize);
						mAudioInstances.Add(aid);
						ret = mAudioInstances.Count - 1;

						aid.SetOnBufferNeededCallback(() =>
							{
								// Send initialized event.
								MoSync.Memory eventData = new MoSync.Memory(8);
								eventData.WriteInt32(MoSync.Struct.MAEvent.type, MoSync.Constants.EVENT_TYPE_AUDIO_COMPLETED);
								eventData.WriteInt32(MoSync.Struct.MAEvent.audioInstance, ret);
								runtime.PostEvent(new Event(eventData));
							});
					}
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

			ioctls.maAudioGetPendingBufferCount = delegate(int _instance)
			{
				int ret = MoSync.Constants.MA_AUDIO_ERR_GENERIC;
				try
				{
					lock (mAudioInstances)
					{
						AudioInstanceDynamic ai = (AudioInstanceDynamic)mAudioInstances[_instance];
						ret = ai.GetPendingBufferCount();
					}
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

			ioctls.maAudioSubmitBuffer = delegate(int _instance, int _pointer, int _numBytes)
			{
				try
				{
					lock (mAudioInstances)
					{
						AudioInstanceDynamic ai = (AudioInstanceDynamic)mAudioInstances[_instance];
#if !LIB
						ai.SubmitBuffer(core.GetDataMemory().GetData(), _pointer, _numBytes);
#else
						//This code needs propper testing.
						//Since the new Memory class, SystemMemory needs to be used while
						//building MoSync as a Library some of the functions that were used before,
						//such ass Memory.GetData() make no sense in the current context (share memory
						//poll in between C++ and C#), making it hard/impossible to implement. This is a
						//workaround and should be treated like one (with some testing done right)

						Byte[] bytes = new Byte[_numBytes];
						core.GetDataMemory().ReadBytes(bytes, _pointer, _numBytes);
						ai.SubmitBuffer(bytes, 0, _numBytes);
#endif
					}
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

			ioctls.maAudioInstanceDestroy = delegate(int _audioInstance)
			{
				try
				{
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ai.Dispose();
						mAudioInstances[_audioInstance] = null;
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ai.Play();
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ai.Stop();
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ai.Pause();
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ai.SetNumberOfLoops(loops);
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ai.SetPosition(_milliseconds);
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ret = ai.GetPosition();
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ret = ai.GetLength();
					}
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
					lock (mAudioInstances)
					{
						IAudioInstance ai = mAudioInstances[_audioInstance];
						ai.SetVolume(volume);
					}
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
