using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Threading;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;

namespace MoSync
{
    class Util
    {

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
            System.Diagnostics.Debug.WriteLine(text.Substring(0, text.Length-1));
        }

        public static void Log(byte[] bytes)
        {
            InitLogging();
            WriteBytesToFile(bytes, "log.txt");
            System.Diagnostics.Debug.WriteLine(bytes);
        }

        public static void CriticalError(String text)
        {
            Log(text);
            //System.Environment.Exit(1);
            //MessageBox.Show(text);
            Deployment.Current.Dispatcher.BeginInvoke(() => MessageBox.Show(text));
            //throw new Exception("ExitAppException");
        }

        public static void Exit(int res)
        {
            CriticalError("Exited!");
            throw new Exception("ExitAppException");
        }

        public static int CreateExtent(int w, int h)
        {
            return (w << 16) | h;
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
    }
}
