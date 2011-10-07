using System;
using System.IO;

namespace MoSync
{	

    // a class that wraps memory
    // This is the interface to the data memory segment for the mosync core.
    // It is also used for binary resources among other things.
    // This should probably implement the stream interface in order to be able to pass it to functions that require streams.

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
            byte[] bytes = BitConverter.GetBytes(b);
            System.Array.Copy(bytes, 0, mData, address, bytes.Length);
        }

        public void WriteInt16(int address, short b)
        {
            byte[] bytes = BitConverter.GetBytes(b);
            System.Array.Copy(bytes, 0, mData, address, bytes.Length);
        }

        public void WriteUInt32(int address, uint b)
        {
            byte[] bytes = BitConverter.GetBytes(b);
            System.Array.Copy(bytes, 0, mData, address, bytes.Length);
        }

        public void WriteInt32(int address, int b)
        {
            byte[] bytes = BitConverter.GetBytes(b);
            System.Array.Copy(bytes, 0, mData, address, bytes.Length);
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
            return System.Text.UTF8Encoding.UTF8.GetString(mData, address, endaddress-address);
        }

        // reads a null-terminated unicode string.
        public String ReadWStringAtAddress(int address)
        {
            int endaddress = address;
            while (ReadInt16(endaddress) != 0) endaddress+=2;
            return System.Text.UnicodeEncoding.Unicode.GetString(mData, address, endaddress - address);
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
	}
}

