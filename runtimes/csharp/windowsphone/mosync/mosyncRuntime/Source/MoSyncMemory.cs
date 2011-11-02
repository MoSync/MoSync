using System;
using System.IO;

namespace MoSync
{

    // a class that wraps memory
    // This is the interface to the data memory segment for the mosync core.
    // It is also used for binary resources among other things.
    // This should probably implement the stream interface in order to be able to pass it to functions that require streams.
    // TODO: maybe this should implement another interface (like Stream)
    // so that code can be efficiently shared between binary resources
    // and unloaded binary resources. Now ubins are loaded into memory also,
    // in order to get something up and running quickly.
    public class Memory
    {
        protected byte[] mData;
        protected int mSizeInBytes;

        public Memory(int sizeInBytes)
        {
            mSizeInBytes = sizeInBytes;
            mData = new byte[mSizeInBytes];
        }

        public void WriteUInt8(int address, byte b)
        {
            mData[address] = b;
        }

        public void WriteInt8(int address, sbyte b)
        {
            mData[address] = (byte)b;
        }

        public void WriteUInt16(int address, ushort b)
        {
            //byte[] bytes = BitConverter.GetBytes(b);
            //System.Array.Copy(bytes, 0, mData, address, bytes.Length);
            mData[address + 0] = (byte)(b & 0xff);
            mData[address + 1] = (byte)((b >> 8) & 0xff);
        }

        public void WriteInt16(int address, short b)
        {
            //byte[] bytes = BitConverter.GetBytes(b);
            //System.Array.Copy(bytes, 0, mData, address, bytes.Length);
            mData[address + 0] = (byte)(b & 0xff);
            mData[address + 1] = (byte)((b >> 8) & 0xff);
        }

        public void WriteUInt32(int address, uint b)
        {
            /*
            byte[] bytes = BitConverter.GetBytes(b);
            System.Array.Copy(bytes, 0, mData, address, bytes.Length);
            */
            mData[address + 0] = (byte)(b & 0xff);
            mData[address + 1] = (byte)((b >> 8) & 0xff);
            mData[address + 2] = (byte)((b >> 16) & 0xff);
            mData[address + 3] = (byte)((b >> 24) & 0xff);
        }

        public void WriteInt32(int address, int b)
        {
            //byte[] bytes = BitConverter.GetBytes(b);
            //System.Array.Copy(bytes, 0, mData, address, bytes.Length);
            mData[address + 0] = (byte)(b & 0xff);
            mData[address + 1] = (byte)((b >> 8) & 0xff);
            mData[address + 2] = (byte)((b >> 16) & 0xff);
            mData[address + 3] = (byte)((b >> 24) & 0xff);

        }

        public byte ReadUInt8(int address)
        {
            return mData[address];
        }

        public sbyte ReadInt8(int address)
        {
            return (sbyte)mData[address];
        }

        public ushort ReadUInt16(int address)
        {
            return BitConverter.ToUInt16(mData, address);
        }

        public short ReadInt16(int address)
        {
            return BitConverter.ToInt16(mData, address);
        }

        public uint ReadUInt32(int address)
        {
            return BitConverter.ToUInt32(mData, address);
        }

        public int ReadInt32(int address)
        {
            return BitConverter.ToInt32(mData, address);
        }

        public int GetSizeInBytes()
        {
            return mSizeInBytes;
        }

        // reads a null-terminated ascii c string
        public String ReadStringAtAddress(int address)
        {
            int endaddress = address;
            while (mData[endaddress] != 0) endaddress++;
            return System.Text.UTF8Encoding.UTF8.GetString(mData, address, endaddress - address);
        }

        // reads a null-terminated unicode string.
        public String ReadWStringAtAddress(int address)
        {
            int endaddress = address;
            while (ReadInt16(endaddress) != 0) endaddress += 2;
            return System.Text.UnicodeEncoding.Unicode.GetString(mData, address, endaddress - address);
        }

        public void WriteStringAtAddress(int address, String str, int maxSize)
        {
            char[] data = str.ToCharArray();
            int i = 0;
            for (i = 0; i < data.Length; i++)
            {
                if (i > maxSize)
                    break;
                WriteUInt8(address + i, (byte)data[i]);
            }

            if (i > maxSize)
                return;
            WriteUInt8(address + i, 0);
        }

        public void WriteWStringAtAddress(int address, String str, int maxSize)
        {
            char[] data = str.ToCharArray();
            for (int i = 0; i < data.Length; i++)
            {
                if (i > maxSize)
                    break;
                WriteInt16(address + i * 2, (short)data[i]);
            }
        }

        public void ReadBytes(byte[] bytes, int src, int size)
        {
            System.Array.Copy(mData, src, bytes, 0, size);
        }

        public void WriteMemoryAtAddress(int dstaddress, Memory memory, int srcaddress, int length)
        {
            byte[] srcBytes = memory.mData;
            System.Array.Copy(srcBytes, srcaddress, mData, dstaddress, length);
        }

        public void WriteFromStream(int dstaddress, Stream stream, int length)
        {
            stream.Read(mData, dstaddress, length);
        }

        public void FillRange(int dstaddress, byte val, int length)
        {
            for (int i = 0; i < length; i++)
            {
                WriteUInt8(dstaddress + i, val);
            }
        }

        public byte[] GetData()
        {
            return mData;
        }

        public Stream GetStream()
        {
            return new System.IO.MemoryStream(mData);
        }
    }
}
