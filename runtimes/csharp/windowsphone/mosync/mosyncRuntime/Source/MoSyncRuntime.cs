using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Windows.Media.Imaging;
using System.Windows;
using System.Threading;
using System.Reflection;
using System.Windows.Input;
using Microsoft.Phone.Controls;

namespace MoSync
{
    // This is an interface for each group of syscalls.
    // Whenever a class implements this interface it
    // will be registered as a syscall module.
    // Each syscall module in the program
    // will be instansiated once. Directly afterwards the
    // Init function will be executed. There you
    // should set those delegates corresponding to the
    // group in the Syscalls instance to an implementation.

    public interface ISyscallModule
    {
        void Init(Syscalls syscalls, Core core, Runtime runtime);
    }

    public interface IIoctlModule
    {
        void Init(Ioctls ioctls, Core core, Runtime runtime);
    }

    public class Event
    {
        public Event(Memory eventData, Memory customEventData = null)
        {
            mEventData = eventData;
            mCustomEventData = customEventData;
        }

        public Memory GetEventData()
        {
            return mEventData;
        }

        public Memory GetCustomEventData()
        {
            return mCustomEventData;
        }

        private Memory mEventData;
        private Memory mCustomEventData;
    };

    // This is the runtime
    // It has a Syscalls instance which contains
    // delegates to the implementations of all syscalls
    // it also loads and manages resources
    public partial class Runtime
    {
        // these might overlap (i.e. the same group instance might be in both sets)
        private List<ISyscallModule> mSyscallModules = new List<ISyscallModule>();
        private List<IIoctlModule> mIoctlModules = new List<IIoctlModule>();
        private Dictionary<String, Object> mModules = new Dictionary<String, Object>();

        protected Core mCore;
        protected Syscalls mSyscalls;
        protected Ioctls mIoctls;
        protected IoctlInvoker mIoctlInvoker;

        protected Dictionary<int, Resource> mResources = new Dictionary<int, Resource>();
        protected int mCurrentResourceHandle;
        public readonly Dictionary<String, int> mLabels = new Dictionary<String, int>();

        private List<Event> mEvents = new List<Event>();
        private AutoResetEvent mEventWaiter = new AutoResetEvent(false);

        public delegate void Cleaner();
        private List<Cleaner> mCleaners = new List<Cleaner>();

        // Cleaners are called when the runtime is being closed down.
        // They should Close any open streams and empty any static variables.
        // They should be prepared for Init() to be called again,
        // triggered by maLoadProgram().
        public void RegisterCleaner(Cleaner c)
        {
            mCleaners.Add(c);
        }

        public void RunCleaners()
        {
            mCleaners.ForEach(delegate(Cleaner c)
            {
                c();
            });
            mCleaners.Clear();
        }

        private void InitSyscalls()
        {

            // find all classes that implements the ISyscallGroup interface
            // and make an instance of each and call their Init method.
            foreach (Type t in Assembly.GetCallingAssembly().GetTypes())
            {
                ISyscallModule syscallGroupInstance = null;
                IIoctlModule ioctlGroupInstance = null;
                if (t.GetInterface("MoSync.ISyscallModule", false) != null)
                {
                    syscallGroupInstance = Activator.CreateInstance(t) as ISyscallModule;
                    mSyscallModules.Add(syscallGroupInstance);
                }
                if (t.GetInterface("MoSync.IIoctlModule", false) != null)
                {
                    if (syscallGroupInstance != null)
                        ioctlGroupInstance = (IIoctlModule)syscallGroupInstance;
                    else
                        ioctlGroupInstance = Activator.CreateInstance(t) as IIoctlModule;
                    mIoctlModules.Add(ioctlGroupInstance);
                }

                if (syscallGroupInstance != null)
                    mModules.Add(t.Name, syscallGroupInstance);
                else if (ioctlGroupInstance != null)
                    mModules.Add(t.Name, ioctlGroupInstance);
            }

            foreach (ISyscallModule module in mSyscallModules)
            {
                module.Init(mSyscalls, mCore, this);
            }

            foreach (IIoctlModule module in mIoctlModules)
            {
                module.Init(mIoctls, mCore, this);
            }
        }

        public T GetModule<T>()
        {
            Object ret;
            if (!mModules.TryGetValue(typeof(T).Name, out ret))
                return default(T);
            else
                return (T)ret;
        }

        // this could probably be generated
        const int MAEvent_type = 0;
        const int MAEvent_point_x = 4;
        const int MAEvent_point_y = 8;
        const int MAEvent_point_touchId = 12;

        private void MouseLeftButtonDown(Object sender, MouseButtonEventArgs e)
        {
            PhoneApplicationFrame mainPage = (PhoneApplicationFrame)Application.Current.RootVisual;
            Memory mem = new Memory(4 * 4);
            mem.WriteInt32(MAEvent_type, MoSync.Constants.EVENT_TYPE_POINTER_PRESSED);
            mem.WriteInt32(MAEvent_point_x, (int)e.GetPosition(mainPage).X); // x
            mem.WriteInt32(MAEvent_point_y, (int)e.GetPosition(mainPage).Y); // y
            mem.WriteInt32(MAEvent_point_touchId, 0);
            PostEvent(new Event(mem));
        }

        private void MouseLeftButtonUp(Object sender, MouseButtonEventArgs e)
        {
            PhoneApplicationFrame mainPage = (PhoneApplicationFrame)Application.Current.RootVisual;
            Memory mem = new Memory(4 * 4);
            mem.WriteInt32(MAEvent_type, MoSync.Constants.EVENT_TYPE_POINTER_RELEASED);
            mem.WriteInt32(MAEvent_point_x, (int)e.GetPosition(mainPage).X); // x
            mem.WriteInt32(MAEvent_point_y, (int)e.GetPosition(mainPage).Y); // y
            mem.WriteInt32(MAEvent_point_touchId, 0);
            PostEvent(new Event(mem));
        }

        private void MouseMove(Object sender, MouseEventArgs e)
        {
            PhoneApplicationFrame mainPage = (PhoneApplicationFrame)Application.Current.RootVisual;
            Memory mem = new Memory(4 * 4);
            mem.WriteInt32(MAEvent_type, MoSync.Constants.EVENT_TYPE_POINTER_DRAGGED);
            mem.WriteInt32(MAEvent_point_x, (int)e.GetPosition(mainPage).X); // x
            mem.WriteInt32(MAEvent_point_y, (int)e.GetPosition(mainPage).Y); // y
            mem.WriteInt32(MAEvent_point_touchId, 0);
            PostEvent(new Event(mem));
        }

        public Runtime(Core core)
        {
            mCore = core;
            mSyscalls = new Syscalls();
            mIoctls = new Ioctls();
            mIoctlInvoker = new IoctlInvoker(mCore, mIoctls);

            PhoneApplicationFrame mainPage = (PhoneApplicationFrame)Application.Current.RootVisual;

            mainPage.MouseLeftButtonDown += MouseLeftButtonDown;
            mainPage.MouseMove += this.MouseMove;
            mainPage.MouseLeftButtonUp += MouseLeftButtonUp;

            RegisterCleaner(() =>
            {
                Util.RunActionOnMainThreadSync(() =>
                {
                    mainPage.MouseLeftButtonDown -= MouseLeftButtonDown;
                    mainPage.MouseMove -= this.MouseMove;
                    mainPage.MouseLeftButtonUp -= MouseLeftButtonUp;
                });
            });

            InitSyscalls();

            mSyscalls.maGetEvent = delegate(int ptr)
            {
                if (mEvents.Count != 0)
                {
                    lock (mEvents)
                    {
                        const int MAEvent_data = 4;
                        Event evt = mEvents[0];
                        Memory eventData = evt.GetEventData();
                        mEvents.RemoveAt(0);
                        Memory customEventData = evt.GetCustomEventData();
                        if (customEventData != null)
                        {
                            mCore.GetDataMemory().WriteMemoryAtAddress(mCore.GetCustomEventDataPointer(),
                                customEventData, 0, customEventData.GetSizeInBytes());
                            eventData.WriteInt32(MAEvent_data, mCore.GetCustomEventDataPointer());
                        }
                        mCore.GetDataMemory().WriteMemoryAtAddress(ptr, eventData, 0, eventData.GetSizeInBytes());
                    }
                    return 1;
                }
                else
                {
                    return 0;
                }
            };

            mSyscalls.maWait = delegate(int timeout)
            {
                if (timeout <= 0)
                    timeout = 1 << 15;
                mEventWaiter.WaitOne(timeout);
            };

            mSyscalls.maIOCtl = delegate(int id, int a, int b, int c)
            {
                return mIoctlInvoker.InvokeIoctl(id, a, b, c);
            };

            mSyscalls.maDestroyObject = delegate(int res)
            {
                mResources[res].SetResourceType(MoSync.Constants.RT_PLACEHOLDER);
                mResources[res].SetInternalObject(null);
            };
        }

        // will reset the runtime.
        public void Init()
        {
        }

        public void PostEvent(Event evt)
        {
            lock (mEvents)
            {
                mEvents.Add(evt);
            }

            mEventWaiter.Set();
        }

        public void PostCustomEvent(int type, Memory customEvent)
        {
            Memory eventData = new Memory(8);
            eventData.WriteInt32(0, type);
            PostEvent(new Event(eventData, customEvent));
        }

        // resource specific packed integer.
        private uint ReadUnsignedVarInt(Stream file)
        {
            uint res = 0;
            int nBytes = 0;
            while (true)
            {
                uint b = MoSync.Util.StreamReadUint8(file);
                res |= (b & 0x7f) << (nBytes * 7);
                if (b > 0x7f)
                    break;
                nBytes++;
                if (nBytes >= 4)
                {
                    // fail
                    throw new Exception("ReadUnsignedVarInt");
                }
            }
            return res;
        }

        public bool LoadResources(Stream file)
        {
            if (MoSync.Util.StreamReadInt8(file) != 'M')
                return false;
            if (MoSync.Util.StreamReadInt8(file) != 'A')
                return false;
            if (MoSync.Util.StreamReadInt8(file) != 'R')
                return false;
            if (MoSync.Util.StreamReadInt8(file) != 'S')
                return false;

            uint numResources = ReadUnsignedVarInt(file);
            uint resSize = ReadUnsignedVarInt(file);


            mCurrentResourceHandle = 1;

            while (true)
            {
                byte type = MoSync.Util.StreamReadUint8(file);
                if (type == 0) break;

                uint size = ReadUnsignedVarInt(file);
                Util.Log("Resource type " + type + ", size " + size + "\n");

                Resource resource = new Resource(null, type);
                mResources.Add(mCurrentResourceHandle, resource);

                switch (type)
                {
                    case MoSync.Constants.RT_PLACEHOLDER:
                        break;
                    case MoSync.Constants.RT_UBIN:
                    case MoSync.Constants.RT_BINARY:
                        Memory memory = new Memory((int)size);
                        memory.WriteFromStream(0, file, (int)size);
                        resource.SetInternalObject(memory);
                        break;
                    case MoSync.Constants.RT_IMAGE:
                        byte[] bytes = new byte[size];
                        file.Read(bytes, 0, (int)size);
                        using (MemoryStream ms = new MemoryStream(bytes, 0, bytes.Length))
                        {
                            /*
                            MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                BitmapImage im = new BitmapImage();
                                im.CreateOptions = BitmapCreateOptions.None;
                                im.SetSource(ms);
                                WriteableBitmap wb = new WriteableBitmap(im);
                                resource.SetInternalObject(wb);
                            });
                            */

                            resource.SetInternalObject(MoSync.Util.CreateWriteableBitmapFromStream(ms));
                            ms.Close();
                        }
                        break;
                    case MoSync.Constants.RT_LABEL:
                        bytes = new byte[size];
                        file.Read(bytes, 0, (int)size);
                        if (bytes[size - 1] != 0)
                            throw new Exception("invalid label (no null terminator)");
                        String s = System.Text.Encoding.UTF8.GetString(bytes, 0, (int)size-1);
                        if (mLabels.ContainsKey(s))
                            throw new Exception("duplicate label");
                        mLabels.Add(s, mCurrentResourceHandle);
                        break;
                    default:
                        Util.Log("Unknown resource type "+type+", size "+size+"\n");
                        file.Seek(size, SeekOrigin.Current);
                        break;
                }

                mCurrentResourceHandle++;
            }

            return true;
        }

        public Syscalls GetSyscalls()
        {
            return mSyscalls;
        }

        public Resource GetResource(int type, int handle)
        {
            Resource ret;
            if (!mResources.TryGetValue(handle, out ret))
            {
                return null;
            }
            else
            {
                /*
                 * should work for ubins and bins.
                if (ret.GetResourceType() != type)
                    return null;
                */
                return ret;
            }
        }

        public void SetResource(int handle, Resource res)
        {
            if (mResources[handle].GetInternalObject() != null)
                throw new Exception("Attempted to overwrite resource");
            SetResourceRaw(handle, res);
        }

        public void SetResourceRaw(int handle, Resource res)
        {
            mResources[handle] = res;
        }

        public int AddResource(Resource res)
        {
            mResources.Add(mCurrentResourceHandle++, res);
            return mCurrentResourceHandle - 1;
        }
    }
}
