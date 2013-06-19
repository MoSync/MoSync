using System;
using System.IO;
using System.Runtime.InteropServices;

using MoSync;

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

	public interface IMemory
	{
		void WriteUInt8(int address, byte b);
		void WriteInt8(int address, sbyte b);
		void WriteUInt16(int address, ushort b);
		void WriteInt16(int address, short b);
		void WriteUInt32(int address, uint b);
		void WriteInt32(int address, int b);
		void WriteInt64(int address, Int64 b);
		void WriteDouble(int address, double b);
		void WriteFloat(int address, float b);
		byte ReadUInt8(int address);
		sbyte ReadInt8(int address);
		ushort ReadUInt16(int address);
		short ReadInt16(int address);
		uint ReadUInt32(int address);
		int ReadInt32(int address);

		// reads a null-terminated ascii c string
		String ReadStringAtAddress(int address);

		// reads a null-terminated unicode string.
		String ReadWStringAtAddress(int address);

		void WriteStringAtAddress(int address, String str, int maxSize);
		void WriteWStringAtAddress(int address, String str, int maxSize);

		void ReadBytes(byte[] bytes, int src, int size);
		void WriteBytes(int dst, byte[] bytes, int size);

		// size equals the amount of integers
		void ReadIntegers(int[] integers, int src, int size);

		void WriteMemoryAtAddress(int dstaddress, Memory memory, int srcaddress, int length);
		void WriteFromStream(int dstaddress, Stream stream, int length);

		void FillRange(int dstaddress, byte val, int length);
	}

	public class Memory : IMemory
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
			mData[address + 0] = (byte)(b);
			mData[address + 1] = (byte)(b >> 8);
		}

		public void WriteInt16(int address, short b)
		{
			mData[address + 0] = (byte)((ushort)b);
			mData[address + 1] = (byte)((ushort)b >> 8);
		}

		public void WriteUInt32(int address, uint b)
		{
			mData[address + 0] = (byte)(b);
			mData[address + 1] = (byte)(b >> 8);
			mData[address + 2] = (byte)(b >> 16);
			mData[address + 3] = (byte)(b >> 24);
		}

		public void WriteInt32(int address, int b)
		{
			mData[address + 0] = (byte)((uint)b);
			mData[address + 1] = (byte)((uint)b >> 8);
			mData[address + 2] = (byte)((uint)b >> 16);
			mData[address + 3] = (byte)((uint)b >> 24);
		}

		public void WriteInt64(int address, Int64 b)
		{
			// low word first
			WriteInt32(address + 0, (int)(b & 0xffffffffL));
			WriteInt32(address + 4, (int)(b >> 32));
		}

		public void WriteDouble(int address, double b)
		{
			WriteInt64(address, BitConverter.DoubleToInt64Bits(b));
		}

		public void WriteFloat(int address, float b)
		{
			WriteInt32(address, MoSync.Util.ConvertToInt(b));
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
			return (ushort)((int)mData[address + 0] |
				((int)mData[address + 1] << 8));
		}

		public short ReadInt16(int address)
		{
			return (short)((int)mData[address + 0] |
				((int)mData[address + 1] << 8));
		}

		public uint ReadUInt32(int address)
		{
			return (uint)((int)mData[address + 0] |
				((int)mData[address + 1] << 8) |
				((int)mData[address + 2] << 16) |
				((int)mData[address + 3] << 24));
		}

		public int ReadInt32(int address)
		{
			return (int)((int)mData[address + 0] |
				((int)mData[address + 1] << 8) |
				((int)mData[address + 2] << 16) |
				((int)mData[address + 3] << 24));
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
			int i;
			for (i = 0; i < data.Length; i++)
			{
				if (i >= maxSize)
					break;
				WriteUInt8(address + i, (byte)data[i]);
			}

			if (i >= maxSize)
				return;
			WriteUInt8(address + i, 0);
		}

		public void WriteWStringAtAddress(int address, String str, int maxSize)
		{
			char[] data = str.ToCharArray();
			int i;
			for (i = 0; i < data.Length; i++)
			{
				if (i >= maxSize)
					break;
				WriteInt16(address + i * 2, (short)data[i]);
			}

			if (i >= maxSize)
				return;
			WriteInt16(address + i * 2, 0);
		}

		public void ReadBytes(byte[] bytes, int src, int size)
		{
			//System.Array.Copy(mData, src, bytes, 0, size);
			System.Buffer.BlockCopy(mData, src, bytes, 0, size);
		}

		public void WriteBytes(int dst, byte[] bytes, int size)
		{
			System.Buffer.BlockCopy(bytes, 0, mData, dst, size);
		}

		// size equals the amount of integers
		public void ReadIntegers(int[] integers, int src, int size)
		{
			// Use System.Buffer.BlockCopy if aligned?
			for (int i = 0; i < size; i++)
			{
				integers[i] = ReadInt32(src + i * 4);
			}
		}

		public void WriteMemoryAtAddress(int dstaddress, Memory memory, int srcaddress, int length)
		{
			byte[] srcBytes = memory.mData;
			//System.Array.Copy(srcBytes, srcaddress, mData, dstaddress, length);
			System.Buffer.BlockCopy(srcBytes, srcaddress, mData, dstaddress, length);
		}

		public void WriteFromStream(int dstaddress, Stream stream, int length)
		{
			int res = stream.Read(mData, dstaddress, length);
			if (res != length)
				throw new Exception("WriteFromStream(" + length + ") returned " + res);
		}

		public void FillRange(int dstaddress, byte val, int length)
		{
			if (val == 0)
			{
				System.Array.Clear(mData, dstaddress, length);
				return;
			}

			length += dstaddress;
			for (; dstaddress < length; dstaddress++)
			{
				mData[dstaddress] = val;
			}
		}

		public byte[] GetData()
		{
			return mData;
		}

		public Stream GetStream()
		{
			return new System.IO.MemoryStream((byte[])mData);
		}

		public Stream GetStream(int offset, int size)
		{
			return new System.IO.MemoryStream((byte[])mData, offset, size);
		}
	}
#if LIB
	public class SystemMemory : IMemory
	{
		public void WriteUInt8(int address, byte b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.WriteByte(ptr, b);
		}

		public void WriteInt8(int address, sbyte b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.WriteByte(ptr, (byte)b);
		}

		public void WriteUInt16(int address, ushort b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.StructureToPtr(b, ptr, false);  // TO BE TESTED
		}

		public void WriteInt16(int address, short b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.WriteInt16(ptr, b);
		}

		public void WriteUInt32(int address, uint b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.StructureToPtr(b, ptr, false); // TO BE TESTED
		}

		public void WriteInt32(int address, int b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.WriteInt32(ptr, b);
		}

		public void WriteInt64(int address, Int64 b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.WriteInt64(ptr, b);
		}

		public void WriteDouble(int address, double b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.StructureToPtr(b, ptr, false);
		}

		public void WriteFloat(int address, float b)
		{
			IntPtr ptr = new IntPtr(address);
			Marshal.StructureToPtr(b, ptr, false);
		}

		public byte ReadUInt8(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return Marshal.ReadByte(ptr);
		}

		public sbyte ReadInt8(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return (sbyte)Marshal.ReadByte(ptr); // TO BE TESTED
		}

		public ushort ReadUInt16(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return (ushort)Marshal.ReadInt16(ptr); // TO BE TESTED
		}

		public short ReadInt16(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return Marshal.ReadInt16(ptr);
		}

		public uint ReadUInt32(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return (uint)Marshal.ReadInt32(ptr);
		}

		public int ReadInt32(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return Marshal.ReadInt32(ptr);
		}

		// reads a null-terminated ascii c string
		public String ReadStringAtAddress(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return Marshal.PtrToStringAnsi(ptr);
		}

		// reads a null-terminated unicode string.
		public String ReadWStringAtAddress(int address)
		{
			IntPtr ptr = new IntPtr(address);
			return Marshal.PtrToStringUni(ptr);
		}

		public void WriteStringAtAddress(int address, String str, int maxSize)
		{
			IntPtr ptr = new IntPtr(address);

			str += '\0';

			int sizeChar = sizeof(char);

			for (int i = 0; i < str.Length; i++)
			{
				if (1 == sizeChar) WriteUInt8(address + i, (byte)str[i]);
				else if (2 == sizeChar) WriteUInt16(address + i*2, (UInt16)str[i]);
			}
		}

		public void WriteWStringAtAddress(int address, String str, int maxSize)
		{
			IntPtr ptr = new IntPtr(address);

			str += '\0';

			int sizeChar = sizeof(char);

			for (int i = 0; i < str.Length; i++)
			{
				if (2 == sizeChar) WriteUInt16(address + i*2, (UInt16)str[i]);
				else if (4 == sizeChar) WriteUInt32(address + i*4, (UInt32)str[i]);
			}
		}

		public void ReadBytes(byte[] bytes, int src, int size)
		{
			IntPtr ptr = new IntPtr(src);
			Marshal.Copy(ptr, bytes, 0, size);
		}

		public void WriteBytes(int dst, byte[] bytes, int size)
		{
			for (int i = 0; i < size; i++)
				WriteUInt8(dst + i * sizeof(byte), bytes[i]);
		}

		// size equals the amount of integers
		public void ReadIntegers(int[] integers, int src, int size)
		{
			IntPtr ptr = new IntPtr(src);
			Marshal.Copy(ptr, integers, 0, size);
		}

		public void WriteMemoryAtAddress(int dstaddress, Memory memory, int srcaddress, int length)
		{
			byte[] mem = memory.GetData();
			WriteBytes(dstaddress, mem, length);
		}

		public void WriteMemoryAtAddress(int dstaddress, SystemMemory memory, int srcaddress, int length)
		{
			byte[] mem = new byte[length];
			for (int i = 0; i < length; i++)
				mem[i] = memory.ReadUInt8(srcaddress + i);

			IntPtr ptr = new IntPtr(dstaddress);
			Marshal.Copy(mem, 0, ptr, length);
		}

		public void WriteFromStream(int dstaddress, Stream stream, int length)
		{
			byte[] buf = new byte[length];
			int res = stream.Read(buf, 0, length);
			if (res != length)
				throw new Exception("WriteFromStream(" + length + ") returned " + res);

			IntPtr ptr = new IntPtr(dstaddress);
			Marshal.Copy(buf, 0, ptr, length);
		}

		public void FillRange(int dstaddress, byte val, int length)
		{
			byte[] buf = new byte[length];
			for (int i = 0; i < length; i++)
				buf[i] = val;

			IntPtr ptr = new IntPtr(dstaddress);
			Marshal.Copy(buf, 0, ptr, length);
		}
	}
#endif
}
