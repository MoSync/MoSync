using System;
using MoSync;

namespace MoSync
{
	public partial class MySillyTest : IExtensionModule
	{
		public delegate int Delegate_maSillyTest(int _test);
		public Delegate_maSillyTest maSillyTest = null;

		public String GetName()
		{
			return "MySillyTest";
		}

		public uint GetHash()
		{
			return 0x2a7ef2be;
		}

		public long Invoke(MoSync.Core core, int id, int a, int b, int c)
		{
			long result = MoSync.Constants.MA_EXTENSION_FUNCTION_UNAVAILABLE;
			switch (id)
			{
				case 1:
					if (maSillyTest == null)
						result = MoSync.Constants.IOCTL_UNAVAILABLE;
					else
						result = maSillyTest(a);
#if SYSCALL_LOG
			Util.Log("maSillyTest("+
				a+
				"): "+result+"\n");
#endif
					return result;
			}
			return result;
		}
	}
}
