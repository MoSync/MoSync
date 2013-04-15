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
            void ShowWithTransition(int screenTransitionType);
            Microsoft.Phone.Shell.ApplicationBar GetApplicationBar();
            void EnableApplicationBar();
            bool GetApplicationBarVisibility();
            void SetApplicationBarVisibility(bool value);
            int AddApplicationBarItemIndex(Object item);
            bool HandleBackButtonPressed();

            /**
             * Check if a given child screen is shown.
             * @param child Given child.
             * @return true if the child is shown, false otherwise.
             */
            bool isChildShown(IScreen child);
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

            /**
             * Set by widget right after the view becomes available for manipulation.
             */
            protected bool isViewCreated = false;

            /**
             * Contains operations that are stacked if the syscalls reach the runtime before
             * the view is created. After the view becomes available for manipulation, the
             * queue is emptied and the operations are triggered on the widget.
             */
            protected Queue<WidgetOperation> operationQueue;

            public WidgetBase()
            {
                operationQueue = new Queue<WidgetOperation>();
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

            public IWidget getChild(int offset)
            {
                if (offset <= mChildren.Count)
                {
                    return mChildren[offset];
                }
                return null;
            }

            public int getChildCount()
            {
                return mChildren.Count;
            }

            public void SetParent(IWidget parent)
            {
                mParent = parent;
            }

            public IWidget GetParent()
            {
                return mParent;
            }

            protected MoSyncWidgetPropertyAttribute GetPropertyAttribute(String name, out PropertyInfo propertyInfoOut)
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
                            if (e.Name.ToLower().Equals(name.ToLower()))
                            {
                                propertyInfoOut = pinfo;
                                return e;
                            }
                        }
                    }
                }

                return null;
            }

            protected void SetProperty(PropertyInfo pinfo, String stringValue)
            {
                if (!isViewCreated)
                {
                    // if the widget view is not ready for manipulation, all the setters go into
                    // the queue
                    WidgetOperation setOperation = new WidgetOperation(pinfo.Name, stringValue);
                    operationQueue.Enqueue(setOperation);
                }
                else
                {
                    switch (pinfo.PropertyType.Name)
                    {
                        case "Double":
                            pinfo.SetValue(this, Convert.ToDouble(stringValue, System.Globalization.CultureInfo.InvariantCulture), null);
                            break;
                        case "Float":
                            pinfo.SetValue(this, Convert.ToSingle(stringValue, System.Globalization.CultureInfo.InvariantCulture), null);
                            break;
                        case "Int32":
                            pinfo.SetValue(this, Convert.ToInt32(stringValue), null);
                            break;
                        case "String":
                            pinfo.SetValue(this, stringValue, null);
                            break;
                    }
                }
            }

            public void SetProperty(String property, String stringValue)
            {
                PropertyInfo pinfo;
                MoSyncWidgetPropertyAttribute pattr = GetPropertyAttribute(property, out pinfo);
                Exception exception = null;
                if (pinfo == null) throw new InvalidPropertyNameException();
                if (pattr.ShouldExecuteOnMainThread)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            try
                            {
                                SetProperty(pinfo, stringValue);
                            }
                            catch (Exception e)
                            {
                                exception = e;
                            }
                        });
                    if (null != exception)
                        if(exception.InnerException is InvalidPropertyValueException)
                            throw new InvalidPropertyValueException();
                }
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

        /**
         * Encapsulates a widget operation. The operation types are defined within this
         * class: SET, GET, ADD, INSERT and REMOVE.
         */
        public class WidgetOperation
        {
            public enum OperationType
            {
                SET,
                GET,
                ADD,
                INSERT,
                REMOVE
            };

            private OperationType mType;
            /**
             * If the operation type is SET or GET, the mPropertyName will contain the name of the property
             * that needs to be SET/GET.
             */
            private string mPropertyName;
            /**
             * If the operation type is SET, the mPropertyValue will contain the value of the property that
             * needs to be SET.
             */
            private string mPropertyValue;

            /**
             * If the operation type is ADD, INSERT or REMOVE the mHandle will contain the handle of the widget
             * that needs to be added/inserted/removed.
             */
            private int mHandle;

            /**
             * Public getter for the operation type.
             */
            public OperationType Type
            {
                get
                {
                    return mType;
                }
            }

            /**
             * Public getter for the property name.
             */
            public string Property
            {
                get
                {
                    return mPropertyName;
                }
            }

            /**
             * Public getter for the property value.
             */
            public string Value
            {
                get
                {
                    return mPropertyValue;
                }
            }

            /**
             * Public getter for the child widget handle.
             */
            public int Handle
            {
                get
                {
                    return mHandle;
                }
            }

            /**
             * Constructor that creates a SET widget operation.
             * @param property The name of the property that needs to be set.
             * @param value The value of the property that needs to be set.
             */
            public WidgetOperation(string property, string value)
            {
                mType = OperationType.SET;
                mHandle = -1;
                mPropertyName = property;
                mPropertyValue = value;
            }

            /**
             * Constructor that creates a GET widget operation.
             * @param property The name of the property for the GET operation.
             */
            public WidgetOperation(string property)
            {
                mType = OperationType.GET;
                mHandle = -1;
                mPropertyName = property;
                mPropertyValue = "";
            }

            /**
            * Constructor that creates a ADD, INSERT or REMOVE widget operation.
            * @param type The operation type (ADD, INSERT or REMOVE).
            */
            public WidgetOperation(OperationType type, int handle)
            {
                if (type == OperationType.ADD || type == OperationType.INSERT || type == OperationType.REMOVE)
                {
                    mType = type;
                    mHandle = handle;
                    mPropertyName = "";
                    mPropertyValue = "";
                }
                else
                {
                    // if the type is not one of the mentioned before, ADD will be considered by default as
                    // the operation type;
                    mType = OperationType.ADD;
                    mHandle = handle;
                    mPropertyName = "";
                    mPropertyValue = "";
                }
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
