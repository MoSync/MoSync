
using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;
using System.Collections.Generic;
using System.Reflection;

namespace MoSync
{
    public interface IExtensionModule
    {
        void Init(Core core, Runtime runtime);
        String GetName();
		uint GetHash();
		long Invoke(MoSync.Core core, int index, int a, int b, int c);
    }

    public class ExtensionModule : ISyscallModule, IIoctlModule
    {
		private List<IExtensionModule> mModules = new List<IExtensionModule>();
        protected void InitExtensions(Core core, Runtime runtime)
        {
			//Assembly[] assemblies = AppDomain.CurrentDomain.GetAssemblies();
			//foreach(Assembly a in assemblies)
			//foreach (Type t in a.GetTypes())
			foreach(Type t in Assembly.GetExecutingAssembly().GetTypes())
            {
                IExtensionModule extensionGroupInstance = null;
                if (t.GetInterface("MoSync.IExtensionModule", false) != null)
                {
                    extensionGroupInstance = Activator.CreateInstance(t) as IExtensionModule;
                    mModules.Add(extensionGroupInstance);
                }
            }

            foreach (IExtensionModule module in mModules)
            {
                module.Init(core, runtime);
            }
        }

		public void AddModule(IExtensionModule mod)
		{
			mModules.Add(mod);
		}

        private IExtensionModule GetModule(String name, out int handle)
        {
			int i = 0;
			foreach (IExtensionModule module in mModules)
			{
				if (module.GetName().Equals(name))
				{
					handle = i;
					return module;
				}

				i++;
			}

			handle = MoSync.Constants.MA_EXTENSION_MODULE_UNAVAILABLE;
			return null;
        }

        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            syscalls.maExtensionFunctionInvoke = delegate(int extensionId, int a, int b, int c)
            {
				int _module = extensionId >> 8;
				if (_module >= 0 && _module < mModules.Count)
				{
					IExtensionModule module = mModules[_module];
					int function = extensionId & 0xff;
					return module.Invoke(core, function, a, b, c);
				}

				return MoSync.Constants.MA_EXTENSION_FUNCTION_UNAVAILABLE;
            };
        }

		public void Init(Ioctls ioctls, Core core, Runtime runtime)
		{
            InitExtensions(core, runtime);

			ioctls.maExtensionModuleLoad = delegate(int _name, int _hash)
			{
				int handle = MoSync.Constants.MA_EXTENSION_MODULE_UNAVAILABLE;

				String name = core.GetDataMemory().ReadStringAtAddress(_name);
				if(GetModule(name, out handle) != null)
				{
					return handle;
				}

				return handle;
			};

			ioctls.maExtensionFunctionLoad = delegate(int _module, int _index)
			{
				int handle = MoSync.Constants.MA_EXTENSION_FUNCTION_UNAVAILABLE;

				if (_module >= 0 && _module < mModules.Count)
				{
					// maybe have method count as a generated part of the extension
					return (_module << 8) | (_index&0xff);
				}

				return handle;
			};
		}
    }
}
