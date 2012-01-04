using System;
using MoSync;

namespace MoSync
{
	public partial class MySillyTest : IExtensionModule
	{
		public void Init(Core core, Runtime runtime)
		{
			maSillyTest = delegate(int _str)
			{
				runtime.GetSyscalls().maSetColor(0x00ff00);
				runtime.GetSyscalls().maDrawText(5, 5, _str);
				runtime.GetSyscalls().maUpdateScreen();
				return 42;
			};
		}
	}
}
