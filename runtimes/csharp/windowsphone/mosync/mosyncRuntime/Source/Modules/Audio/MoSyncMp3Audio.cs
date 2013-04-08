/* Copyright (C) 2013 MoSync AB

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
using System.IO;
using Media;

namespace MoSync
{
	class Mp3AudioData : IAudioData
	{
		private Mp3MediaStreamSource mSource;

		public Mp3AudioData(Stream s)
		{
			mSource = new Mp3MediaStreamSource(s);
		}

		public IAudioInstance CreateInstance()
		{
			if (mSource == null)
			{
				throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_DATA);
			}

			return new AudioInstanceMediaElement(mSource);
		}

		public void Dispose()
		{
			mSource = null;
		}
	}

	public class Mp3Audio
	{
		public static IAudioData FromStream(Stream stream)
		{
			return new Mp3AudioData(stream);
		}

		public static IAudioData FromUrlOrFilePath(String url, bool shouldStream)
		{
			if (url.StartsWith("file://") || !url.Contains("://"))
			{
				String path = url.Replace("file://", "");
				FileModule.File file = new FileModule.File(path, FileAccess.Read);
				file.TryOpen();
				if (file.FileStream == null)
				{
					throw new MoSync.Util.ReturnValueException(MoSync.Constants.MA_AUDIO_ERR_INVALID_FILE);
				}
				return FromStream(file.FileStream);
			}
			else
			{
				return Audio.FromUrlOrFilePath(url, shouldStream);
			}
		}
	}
}
