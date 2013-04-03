using System;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Media;

namespace MoSync
{
	public class SoundModule : ISyscallModule
	{
		MediaElement mElement = null;
		double mVolume = 100;

		public void Init(Syscalls mSyscalls, Core mCore, Runtime mRuntime)
		{
			mSyscalls.maSoundPlay = delegate(int _data, int _offset, int _size)
			{
				mSyscalls.maSoundStop();
				Resource audiores = mRuntime.GetResource(MoSync.Constants.RT_BINARY, _data);
				BoundedStream s = new BoundedStream((Stream)audiores.GetInternalObject(), _offset, _size);

				// Read MIME type. Mp3MediaStreamSource is not clever enough to bypass it.
				StringBuilder sb = new StringBuilder();
				int b;
				while ((b = s.ReadByte()) > 0)
				{
					sb.Append((char)b);
				}
				if (b < 0)
				{
					// The MIME type was interrupted.
					// Bad stream. We don't want to play it.
					return -2;
				}
				if (sb.ToString() != "audio/mpeg")
				{
					// We can only play MP3 files.
					return -3;
				}

				Mp3MediaStreamSource source = new Mp3MediaStreamSource(s);

				// all Controls code must be Dispatched to the proper thread,
				// or you'll get a fatal Exception.
				Deployment.Current.Dispatcher.BeginInvoke(() =>
				{
					mElement = new MediaElement();
					mElement.Volume = mVolume;
					mElement.SetSource(source);
					mElement.Play();
				});
				return 0;
			};

			mSyscalls.maSoundStop = delegate()
			{
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					if (mElement != null)
					{
						mElement.Stop();
					}
				});
			};

			mSyscalls.maSoundIsPlaying = delegate()
			{
				int result = 0;
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					if (mElement != null)
					{
						MediaElementState s = mElement.CurrentState;
						result = (s == MediaElementState.Buffering) || (s == MediaElementState.Playing) ? 1 : 0;
					}
				});
				return result;
			};

			mSyscalls.maSoundGetVolume = delegate()
			{
				return (int)mVolume;
			};

			mSyscalls.maSoundSetVolume = delegate(int _vol)
			{
				mVolume = _vol;
				if (mVolume > 100)
					mVolume = 100;
				else if (mVolume < 0)
					mVolume = 0;
				Deployment.Current.Dispatcher.BeginInvoke(() =>
				{
					if (mElement != null)
					{
						mElement.Volume = mVolume;
					}
				});
			};
		}
	}
}
