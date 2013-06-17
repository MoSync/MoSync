using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Threading;
using System.Windows.Media.Imaging;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;

namespace MoSync
{
	public class Util
	{
		public static readonly DateTime Epoch = new DateTime(1970, 1, 1, 0, 0, 0, 0);
		public static readonly long EpochFileTime = Epoch.ToFileTime();
		public static readonly long EpochFileTimeUtc = Epoch.ToFileTimeUtc();

		public static long ToUnixTime(DateTime dt)
		{
			return (dt.ToFileTime() - EpochFileTime) / 10000000;
		}

		public static long ToUnixTimeUtc(DateTime dt)
		{
			return ToUnixTimeUtc(dt.ToFileTimeUtc());
		}

		public static long ToUnixTimeUtc(long fileTime)
		{
			return (fileTime - EpochFileTimeUtc) / 10000000;
		}

		public static uint NextPowerOfTwo(int minPow, uint val)
		{
			uint i = (uint)(1 << minPow);
			while (i < val)
			{
				i <<= 1;
			}
			return i;
		}

		public static sbyte StreamReadInt8(Stream stream)
		{
			return (sbyte)stream.ReadByte();
		}

		public static short StreamReadInt16(Stream stream)
		{
			int a = stream.ReadByte();
			int b = stream.ReadByte();
			return (short)((b << 8) | a);
		}

		public static int StreamReadInt32(Stream stream)
		{
			int a = stream.ReadByte();
			int b = stream.ReadByte();
			int c = stream.ReadByte();
			int d = stream.ReadByte();
			return (d << 24) | (c << 16) | (b << 8) | a;
		}

		public static byte StreamReadUint8(Stream stream)
		{
			return (byte)stream.ReadByte();
		}

		public static ushort StreamReadUint16(Stream stream)
		{
			int a = stream.ReadByte();
			int b = stream.ReadByte();
			return (ushort)((b << 8) | a);
		}

		public static uint StreamReadUint32(Stream stream)
		{
			int a = stream.ReadByte();
			int b = stream.ReadByte();
			int c = stream.ReadByte();
			int d = stream.ReadByte();
			return (uint)((d << 24) | (c << 16) | (b << 8) | a);
		}

		public static double ConvertToDouble(int a, int b)
		{
			/*
			ulong value = (ulong)a;
			value <<= 32;
			value |= (uint)b;
			*/
			ulong value = (ulong)b;
			value <<= 32;
			value |= (uint)a;

			double retVal = BitConverter.Int64BitsToDouble((long)value);
			return retVal;
		}

		public static float ConvertToFloat(int a)
		{
			return BitConverter.ToSingle(BitConverter.GetBytes(a), 0);
		}

		public static int ConvertToInt(float v)
		{
			return BitConverter.ToInt32(BitConverter.GetBytes(v), 0);
		}
		private static bool sLoggingStarted = false;
		private static void InitLogging()
		{
			if (!sLoggingStarted)
			{
				WriteTextToFile(null, "log.txt", FileMode.Create);
				sLoggingStarted = true;
			}
		}

		// make these thread safe.
		public static void Log(String text)
		{
			Console.Write(text);
			InitLogging();
			WriteTextToFile(text, "log.txt");
			if (System.Diagnostics.Debugger.IsAttached)
			{
				DebugWrite(text);
			}
		}

		public static void Log(Exception e)
		{
			Log(e.Message);
			Log(e.StackTrace);
		}

		private static void DebugWrite(String text)
		{
			// handle multiple lines
			int pos = 0;
			while (pos < text.Length)
			{
				int eol = text.IndexOf('\n', pos);
				int len;
				if (eol > pos)
				{
					len = eol - pos;
                    if (eol + 1 < text.Length && Char.IsControl(text[eol]))
                        len -= 1;
				}
				else
				{
					len = text.Length - pos;
				}
				System.Diagnostics.Debug.WriteLine(text.Substring(pos, len));
				if (eol < 0)
					break;
				pos = eol + 1;
			}
		}

		// make these thread safe.
		public static void Log(byte[] bytes)
		{
			InitLogging();
			WriteBytesToFile(bytes, "log.txt");
			if (System.Diagnostics.Debugger.IsAttached)
			{
				String text = System.Text.Encoding.UTF8.GetString(bytes, 0, bytes.Length);
				DebugWrite(text);
			}
		}

		public class ExitException : Exception
		{
			public readonly int result;
			public ExitException(int res)
			{
				result = res;
			}
		};

		public class ReturnValueException : Exception
		{
			public readonly int result;
			public ReturnValueException(int res)
			{
				result = res;
			}
		}

		public static void ShowMessage(String text, bool log=true, String caption="")
		{
			if(log)
				Log(text);

			if(caption == "")
				Deployment.Current.Dispatcher.BeginInvoke(() => MessageBox.Show(text));
			else
				Deployment.Current.Dispatcher.BeginInvoke(() => MessageBox.Show(text, caption, MessageBoxButton.OK));
		}

		public static void CriticalError(String text)
		{
			Log(text);

// Enable this if you want the debugger to automatically break here.
#if false
			if (System.Diagnostics.Debugger.IsAttached)
			{
				System.Diagnostics.Debugger.Break();
			}
#endif
			Deployment.Current.Dispatcher.BeginInvoke(() => MessageBox.Show(text));
			throw new ExitException(-1);
		}

		public static void Exit(int res)
		{
			//CriticalError("Exited!");
			//throw new ExitException(res);
		}

		public static int CreateExtent(int w, int h)
		{
			return (w << 16) | h;
		}

		public static int ExtentX(int extent)
		{
			return (extent >> 16) & 0xffff;
		}

		public static int ExtentY(int extent)
		{
			return (extent) & 0xffff;
		}

		static Thread sStartupThread;

		// This must be run once from the main ui thread to save
		// which thread is the main ui thread.
		public static void InitStartupThread()
		{
			sStartupThread = Thread.CurrentThread;
		}

		public static void RunActionOnMainThreadSync(Action action)
		{
			RunActionOnMainThread(action, true);
		}


		static Action sPreRunOnMainThreadAction = null;
		public static void SetPreRunOnMainThreadAction(Action action)
		{
			sPreRunOnMainThreadAction = action;
		}

		static Action sPostRunOnMainThreadAction = null;
		public static void SetPostRunOnMainThreadAction(Action action)
		{
			sPostRunOnMainThreadAction = action;
		}

		public static void RunActionOnMainThread(Action action, bool sync)
		{
            // if we're already on the UI thread, run the action
            // TODO SA: verify if this is correct
            if (Deployment.Current.Dispatcher.CheckAccess())
            {
                action();
                return;
            }

            // TODO SA: verify if this does something
			if (Thread.CurrentThread == sStartupThread)
			{
				action();
				return;
			}

			if (sync)
			{
				if (sPreRunOnMainThreadAction != null)
					sPreRunOnMainThreadAction();

				using (AutoResetEvent are = new AutoResetEvent(false))
				{
					Deployment.Current.Dispatcher.BeginInvoke(() =>
					{
						action();
						are.Set();
					});
					are.WaitOne();
				}

				if (sPostRunOnMainThreadAction != null)
					sPostRunOnMainThreadAction();
			}
			else
			{
				Deployment.Current.Dispatcher.BeginInvoke(() =>
				{
					action();
				});
			}
		}

		public static void WriteTextToFile(string message, string file, FileMode fileMode = FileMode.Append)
		{
			lock (typeof(MoSync.Util))
			{
				using (IsolatedStorageFile isolatedStorage = IsolatedStorageFile.GetUserStoreForApplication())
				using (StreamWriter streamWriter = new StreamWriter(new IsolatedStorageFileStream(file, fileMode, isolatedStorage)))
				{
					if (message != null)
						streamWriter.Write(message);
					//streamWriter.WriteLine(message);

					streamWriter.Close();
				}
			}
		}

		public static void WriteBytesToFile(byte[] bytes, string file, FileMode fileMode = FileMode.Append)
		{
			lock (typeof(MoSync.Util))
			{
				using (IsolatedStorageFile isolatedStorage = IsolatedStorageFile.GetUserStoreForApplication())
				using (BinaryWriter binaryWriter = new BinaryWriter(new IsolatedStorageFileStream(file, fileMode, isolatedStorage)))
				{
					if (bytes != null)
						binaryWriter.Write(bytes);
					binaryWriter.Close();
				}
			}
		}

        /**
         * Converts a color string into a SolidColorBrush.
         * @param value The color string.
         * @param brush The resulting brush.
         * @throws InvalidPropertyValueException,
         */
		public static void ConvertStringToColor(string value, out System.Windows.Media.SolidColorBrush brush)
		{
            brush = null;
            System.Windows.Media.SolidColorBrush tempBrush = null;
            bool invalidColor = false;

            MoSync.Util.RunActionOnMainThread(() =>
            {
                try
                {
                    // value starts with "0x"
                    if (value.Length == 8 && value[0].Equals('0') && value[1].Equals('x'))
                    {
                        //converting the string from value into RGB bytes
                        byte R = Byte.Parse(value.Substring(2, 2), System.Globalization.NumberStyles.HexNumber);
                        byte G = Byte.Parse(value.Substring(4, 2), System.Globalization.NumberStyles.HexNumber);
                        byte B = Byte.Parse(value.Substring(6, 2), System.Globalization.NumberStyles.HexNumber);
                        byte A = 255;
                        tempBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromArgb(A, R, G, B));
                    }
                    // value starts with "#"
                    else if (value.Length == 7 && value[0].Equals('#'))
                    {
                        //converting the string from value into RGB bytes
                        byte R = Byte.Parse(value.Substring(1, 2), System.Globalization.NumberStyles.HexNumber);
                        byte G = Byte.Parse(value.Substring(3, 2), System.Globalization.NumberStyles.HexNumber);
                        byte B = Byte.Parse(value.Substring(5, 2), System.Globalization.NumberStyles.HexNumber);
                        byte A = 255;
                        tempBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromArgb(A, R, G, B));
                    }
                    // value doesn't have the normal hex sign
                    else if (value.Length == 6)
                    {
                        //converting the string from value into RGB bytes
                        byte R = Byte.Parse(value.Substring(0, 2), System.Globalization.NumberStyles.HexNumber);
                        byte G = Byte.Parse(value.Substring(2, 2), System.Globalization.NumberStyles.HexNumber);
                        byte B = Byte.Parse(value.Substring(4, 2), System.Globalization.NumberStyles.HexNumber);
                        byte A = 255;
                        tempBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromArgb(A, R, G, B));
                    }
                    // special case for 0
                    else if (value.Length == 1 && value[0].Equals('0'))
                    {
                        tempBrush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromArgb(255, 0, 0, 0));
                    }
                    else
                    {
                        invalidColor = true;
                    }
                }
                catch
                {
                    // a error occured during parsing
                    invalidColor = true;
                }
            }, true);

            if (invalidColor)
            {
                throw new InvalidPropertyValueException();
            }
            else
            {
                brush = tempBrush;
            }
		}

		public static WriteableBitmap CreateWriteableBitmapFromStream(Stream stream)
		{
			WriteableBitmap wb = null;
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				BitmapImage im = new BitmapImage();
				im.CreateOptions = BitmapCreateOptions.None;
				try
				{
					im.SetSource(stream);
					wb = new WriteableBitmap(im);
				}
				catch (Exception e)
				{
					Util.Log(e.StackTrace);
					wb = null;
				}
			});
			return wb;
		}

		public static WriteableBitmap CreateWriteableBitmapFromMemory(Memory memory)
		{
			WriteableBitmap wb = null;

			BitmapImage im = new BitmapImage();
			im.CreateOptions = BitmapCreateOptions.None;
			im.SetSource(memory.GetStream());
			wb = new WriteableBitmap(im);

			return wb;
		}

		public static bool CopySeekableStreams(Stream src, int srcOfs, Stream dst, int dstOfs, int count)
		{
			src.Seek(srcOfs, SeekOrigin.Begin);
			dst.Seek(dstOfs, SeekOrigin.Begin);

			for (int i = 0; i < count; i++)
			{
				int read = src.ReadByte();
				if(read == -1) return false;
				dst.WriteByte((byte)read);
			}

			return true;
		}
	}
}
