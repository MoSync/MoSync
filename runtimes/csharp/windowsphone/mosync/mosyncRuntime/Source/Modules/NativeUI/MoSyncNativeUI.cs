using System.Collections.Generic;
using System;
using System.IO;
using System.Reflection;

namespace MoSync
{
        public interface IWidget
        {
            void AddChild(IWidget child);
            void InsertChild(IWidget child, int index);
            void RemoveChild(int index);
            void RemoveChild(IWidget child);
            void RemoveFromParent();
            void SetParent(IWidget parent);
            IWidget GetParent();
            bool HasParent();
            bool IsChild(IWidget child);
            void SetProperty(String property, String Value);
            String GetProperty(String property);
            void SetHandle(int handle);
            int GetHandle();
            void SetRuntime(Runtime runtime);
        }

        // A screen also needs to implement this interface.
        public interface IScreen
        {
            void Show();
        }

        public class MoSyncWidgetPropertyAttribute : Attribute
        {
            public MoSyncWidgetPropertyAttribute(String name, bool executeOnMainThread = true)
            {
                mName = name;
                mShouldExecuteOnMainThread = executeOnMainThread;
            }

            protected String mName;
            protected bool mShouldExecuteOnMainThread;

            public String Name
            {
                get { return mName; }
                set { mName = value; }
            }

            public bool ShouldExecuteOnMainThread
            {
                get { return mShouldExecuteOnMainThread; }
                set { mShouldExecuteOnMainThread = value; }
            }
        }

        public class InvalidPropertyNameException : System.Exception
        {
            public InvalidPropertyNameException() : base() { }
        }

        public class InvalidPropertyValueException : System.Exception
        {
            public InvalidPropertyValueException() : base() { }
        }

        public abstract class WidgetBase : IWidget
        {
            protected IWidget mParent = null;
            protected List<IWidget> mChildren = new List<IWidget>();
            protected int mHandle;
            protected Runtime mRuntime;


            public WidgetBase()
            {
            }

            public virtual void AddChild(IWidget child)
            {
                mChildren.Add(child);
            }

            public virtual void InsertChild(IWidget child, int index)
            {
                if (index >= 0 && index <= mChildren.Count)
                {
                    mChildren.Insert(index, child);
                }
            }

            public virtual void RemoveChild(int index)
            {
                if(index >= 0 && index < mChildren.Count)
                {
                    mChildren.RemoveAt(index);
                }
            }

            public virtual void RemoveChild(IWidget child)
            {
                mChildren.Remove(child);
            }

            public bool HasParent()
            {
                return mParent != null;
            }

            public bool IsChild(IWidget child)
            {
                return mChildren.Contains(child);
            }

            public void RemoveFromParent()
            {
                if (mParent != null)
                {
                    mParent.RemoveChild(this);
                }
            }

            public void SetParent(IWidget parent)
            {
                mParent = parent;
            }

            public IWidget GetParent()
            {
                return mParent;
            }

            private MoSyncWidgetPropertyAttribute GetPropertyAttribute(String name, out PropertyInfo propertyInfoOut)
            {
                Type type = this.GetType();
                propertyInfoOut = null;

                foreach (PropertyInfo pinfo in type.GetProperties())
                {
                    foreach (Attribute attr in pinfo.GetCustomAttributes(false))
                    {
                        if (attr.GetType() == typeof(MoSyncWidgetPropertyAttribute))
                        {
                            MoSyncWidgetPropertyAttribute e = (MoSyncWidgetPropertyAttribute)attr;
                            if (e.Name.Equals(name))
                            {
                                propertyInfoOut = pinfo;
                                return e;
                            }
                        }
                    }
                }

                return null;
            }

            private void SetProperty(PropertyInfo pinfo, String stringValue)
            {
                switch (pinfo.PropertyType.Name)
                {
                    case "Double":
                        pinfo.SetValue(this, Convert.ToDouble(stringValue), null);
                        break;
                    case "Float":
                        pinfo.SetValue(this, Convert.ToSingle(stringValue), null);
                        break;
                    case "Int32":
                        pinfo.SetValue(this, Convert.ToInt32(stringValue), null);
                        break;
                    case "String":
                        pinfo.SetValue(this, stringValue, null);
                        break;
                }
            }

            public void SetProperty(String property, String stringValue)
            {
                PropertyInfo pinfo;
                MoSyncWidgetPropertyAttribute pattr = GetPropertyAttribute(property, out pinfo);
                if (pinfo == null) throw new InvalidPropertyNameException();
                if (pattr.ShouldExecuteOnMainThread)
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            SetProperty(pinfo, stringValue);
                        });
                else
                    SetProperty(pinfo, stringValue);
            }

            private String GetProperty(PropertyInfo pinfo)
            {
                Object value = pinfo.GetValue(this, null);
                return value.ToString();
            }

            public String GetProperty(String property)
            {
                PropertyInfo pinfo;
                MoSyncWidgetPropertyAttribute pattr = GetPropertyAttribute(property, out pinfo);
                if (pinfo == null || pinfo.CanRead == false) throw new InvalidPropertyNameException();
                String ret = "";
                if (pattr.ShouldExecuteOnMainThread)
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        ret = GetProperty(pinfo);
                    });
                else
                    ret = GetProperty(pinfo);

                return ret;
            }

            public void SetHandle(int handle)
            {
                mHandle = handle;
            }

            public int GetHandle()
            {
                return mHandle;
            }

            public void SetRuntime(Runtime runtime)
            {
                mRuntime = runtime;
            }
        }

        public abstract class UIManager
        {
            public UIManager()
            {
            }

            public IWidget CreateWidget(string widgetName)
            {

                IWidget ret = null;
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    foreach (Type t in Assembly.GetCallingAssembly().GetTypes())
                    {
                        if (t.GetInterface("MoSync.IWidget", false) != null && t.Name == widgetName)
                        {
                            ret = Activator.CreateInstance(t) as IWidget;
                            break;
                        }
                    }
                });

                return ret;
            }
        }
}
