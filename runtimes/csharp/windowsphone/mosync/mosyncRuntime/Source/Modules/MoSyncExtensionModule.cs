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
        long Invoke(int index, int a, int b, int c);
    }

    public class MA_HELLO_ExtensionFunctions
    {
        public delegate long Delegate_maPlot(int _x, int _y);
        public Delegate_maPlot maPlot = null;
    }

    public class MA_HELLO_ExtensionModule : IExtensionModule
    {
        protected MA_HELLO_ExtensionFunctions mFunctions = new MA_HELLO_ExtensionFunctions();
        public String GetName()
        {
            return "MA_HELLO";
        }

        public void Init(Core core, Runtime runtime)
        {
            Syscalls syscalls = runtime.GetSyscalls();

            mFunctions.maPlot = delegate(int _x, int _y)
            {
                syscalls.maPlot(_x, _y);
                return 0;
            };
        }

        public long Invoke(int index, int a, int b, int c)
        {
            switch (index)
            {
                case 0:
                    return mFunctions.maPlot(a, b);
            }

            return MoSync.Constants.IOCTL_UNAVAILABLE;
        }
    }

    public class ExtensionModule : ISyscallModule
    {
        private Dictionary<String, IExtensionModule> mModules = new Dictionary<String, IExtensionModule>();

        protected void InitExtensions(Core core, Runtime runtime)
        {
            foreach (Type t in Assembly.GetCallingAssembly().GetTypes())
            {
                IExtensionModule extensionGroupInstance = null;
                if (t.GetInterface("MoSync.IExtensionModule", false) != null)
                {
                    extensionGroupInstance = Activator.CreateInstance(t) as IExtensionModule;
                    mModules.Add(extensionGroupInstance.GetName(), extensionGroupInstance);
                }
            }

            foreach (KeyValuePair<String, IExtensionModule> module in mModules)
            {
                module.Value.Init(core, runtime);
            }
        }

        private IExtensionModule GetModule(String name)
        {
            IExtensionModule ret;
            if (!mModules.TryGetValue(name, out ret))
                return null;
            else
                return ret;
        }

        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {

            InitExtensions(core, runtime);

            syscalls.maInvokeExtension = delegate(int extensionId, int a, int b, int c)
            {
                int ptr = core.GetDataMemory().ReadInt32(extensionId+0);
                String module = core.GetDataMemory().ReadStringAtAddress(ptr);
                int index = core.GetDataMemory().ReadInt32(extensionId + 4);

                IExtensionModule extension = GetModule(module);
                if (extension == null)
                    return MoSync.Constants.IOCTL_UNAVAILABLE;
                else
                    return extension.Invoke(index, a, b, c);
            };
        }
    }
}
