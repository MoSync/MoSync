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

        private static void DebugWrite(String text)
        {
            if (text[text.Length - 1] == '\n')
                text = text.Substring(0, text.Length - 1);
            System.Diagnostics.Debug.WriteLine(text);
        }

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

        public static void CriticalError(String text)
        {
            Log(text);
            //System.Environment.Exit(1);
            //MessageBox.Show(text);
            if (System.Diagnostics.Debugger.IsAttached)
            {
                System.Diagnostics.Debugger.Break();
            }
            Deployment.Current.Dispatcher.BeginInvoke(() => MessageBox.Show(text));
            throw new ExitException(-1);
        }

        public static void Exit(int res)
        {
            //CriticalError("Exited!");
            throw new ExitException(res);
        }

        public static int CreateExtent(int w, int h)
        {
            return (w << 16) | h;
        }

        public static int ExtentX(int extent)
        {
            return (extent>>16)&0xffff;
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
            if (Thread.CurrentThread == sStartupThread)
            {
                action();
                return;
            }

            using (AutoResetEvent are = new AutoResetEvent(false))
            {
                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    action();
                    are.Set();
                });
                are.WaitOne();
            }
        }

        public static void WriteTextToFile(string message, string file, FileMode fileMode = FileMode.Append)
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

        public static void WriteBytesToFile(byte[] bytes, string file, FileMode fileMode = FileMode.Append)
        {
            using (IsolatedStorageFile isolatedStorage = IsolatedStorageFile.GetUserStoreForApplication())
            using (BinaryWriter binaryWriter = new BinaryWriter(new IsolatedStorageFileStream(file, fileMode, isolatedStorage)))
            {
                if (bytes != null)
                    binaryWriter.Write(bytes);
                binaryWriter.Close();
            }
        }

        public static void convertStringToColor(string value, out System.Windows.Media.SolidColorBrush brush)
        {
            brush = null;
            if (value.Length == 8 && value[0].Equals('0') && value[1].Equals('x'))
            {
                //converting the string from value into RGB bytes
                byte R = Byte.Parse(value.Substring(2, 2), System.Globalization.NumberStyles.HexNumber);
                byte G = Byte.Parse(value.Substring(4, 2), System.Globalization.NumberStyles.HexNumber);
                byte B = Byte.Parse(value.Substring(6, 2), System.Globalization.NumberStyles.HexNumber);
                byte A = 255;
                brush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromArgb(A, R, G, B));
            }
            else if (value.Length == 7 && value[0].Equals('#'))
            {
                //converting the string from value into RGB bytes
                byte R = Byte.Parse(value.Substring(1, 2), System.Globalization.NumberStyles.HexNumber);
                byte G = Byte.Parse(value.Substring(3, 2), System.Globalization.NumberStyles.HexNumber);
                byte B = Byte.Parse(value.Substring(5, 2), System.Globalization.NumberStyles.HexNumber);
                byte A = 255;
                brush = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromArgb(A, R, G, B));
            }
        }

        public static WriteableBitmap CreateWriteableBitmapFromStream(Stream stream)
        {
            WriteableBitmap wb = null;
            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                BitmapImage im = new BitmapImage();
                im.CreateOptions = BitmapCreateOptions.None;
                im.SetSource(stream);
                wb = new WriteableBitmap(im);
            });

            return wb;
        }
    }
}
