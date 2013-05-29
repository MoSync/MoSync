using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{
	public class MemoryModule : ISyscallModule
	{
		public void Init(Syscalls syscalls, Core core, Runtime runtime)
		{
			syscalls.memset = delegate(int dst, int val, int num)
			{
				core.GetDataMemory().FillRange(dst, (byte)val, num);
				return dst;
			};

			syscalls.memcpy = delegate(int dst, int src, int num)
			{
				core.GetDataMemory().WriteMemoryAtAddress(dst, core.GetDataMemory(), src, num);
				return dst;
			};

			syscalls.strcpy = delegate(int dst, int src)
			{
#if !LIB
				byte[] mem = core.GetDataMemory().GetData();
				int origDst = dst;
				src--;
				do
				{
					src++;
					mem[dst] = mem[src];
					dst++;
				} while (mem[src] != 0);
				return origDst;
#else
				string source = core.GetDataMemory().ReadStringAtAddress(src);
				Byte[] bytes = new Byte[source.Length * sizeof(char)];
				System.Buffer.BlockCopy(source.ToCharArray(), 0, bytes, 0, bytes.Length);

				core.GetDataMemory().WriteBytes(dst, bytes, source.Length * sizeof(char));
				return dst;
#endif
			};

			syscalls.strcmp = delegate(int str1, int str2)
			{
#if !LIB
				byte[] mem = core.GetDataMemory().GetData();
				while (mem[str1] != 0 && mem[str1] == mem[str2])
				{
					str1++;
					str2++;
				}
				return (mem[str1] - mem[str2]);
#else
				string s1 = core.GetDataMemory().ReadStringAtAddress(str1);
				string s2 = core.GetDataMemory().ReadStringAtAddress(str2);

				return s1.CompareTo(s2);
#endif
			};

			syscalls.maCreateData = delegate(int placeholder, int size)
			{
				MemoryStream mem = null;
				try
				{
					mem = new MemoryStream(size);
					mem.SetLength(size);
				}
				catch (OutOfMemoryException e)
				{
					MoSync.Util.Log(e);
					return MoSync.Constants.RES_OUT_OF_MEMORY;
				}

				Resource res = runtime.GetResource(MoSync.Constants.RT_PLACEHOLDER, placeholder);
				res.SetInternalObject(mem);
				res.SetResourceType(MoSync.Constants.RT_BINARY);
				return MoSync.Constants.RES_OK;
			};

			syscalls.maWriteData = delegate(int data, int src, int offset, int size)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, data);
				Stream mem = (Stream)res.GetInternalObject();
				mem.Seek(offset, SeekOrigin.Begin);
#if !LIB
				mem.Write(core.GetDataMemory().GetData(), src, size);
#else
				byte[] bytes = new byte[size];
				core.GetDataMemory().ReadBytes(bytes, src, size);
				mem.Write(bytes, 0, size); //TO BE TESTED
#endif
			};

			syscalls.maReadData = delegate(int data, int dst, int offset, int size)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, data);
				Stream mem = (Stream)res.GetInternalObject();
				mem.Seek(offset, SeekOrigin.Begin);
#if !LIB
				mem.Read(core.GetDataMemory().GetData(), dst, size);
#else
				byte[] bytes = new byte[size];
				mem.Read(bytes, 0, size);
				core.GetDataMemory().WriteBytes(dst, bytes, size); //TO BE TESTED
#endif
			};

			syscalls.maGetDataSize = delegate(int data)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, data);
				Stream mem = (Stream)res.GetInternalObject();
				return (int)mem.Length;
			};

			syscalls.maCopyData = delegate(int _params)
			{
				throw new Exception("maCopyData not implemented");
			};
		}
	}
}
