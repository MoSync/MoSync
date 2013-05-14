using System.Collections.Generic;
using System;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Threading;
using MoSync.NativeUI;

namespace MoSync
{
    #region IWidget interface

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
        Runtime GetRuntime();
        String GetLastValidSet(String property);
        void AddOperations(Queue<WidgetOperation> queue);
        void AddOperation(WidgetOperation op);
        void RunOperationQueue();
    }

    #endregion

    #region IScreen interface

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

    #endregion

    #region MoSyncWidgetPropertyAttribute, InvalidPropertyNameException and InvalidPropertyValueException classes

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

    #endregion

    #region WidgetBase class

    public abstract class WidgetBase : IWidget
    {
        protected IWidget mParent = null;
        protected List<IWidget> mChildren = new List<IWidget>();
        protected int mHandle;
        protected Runtime mRuntime;

        /**
         * Contains operations that are stacked if the syscalls reach the runtime before
         * the view is created. After the view becomes available for manipulation, the
         * queue is emptied and the operations are triggered on the widget.
         */
        protected Queue<WidgetOperation> mOperationQueue;

        public void RunOperationQueue()
        {
        }

        public WidgetBase()
        {
            mOperationQueue = new Queue<WidgetOperation>();
        }

        #region Setters/Getters for the runtime and handle

        public void SetRuntime(Runtime runtime)
        {
            mRuntime = runtime;
        }

        public Runtime GetRuntime()
        {
            return mRuntime;
        }

        public void SetHandle(int handle)
        {
            mHandle = handle;
        }

        public int GetHandle()
        {
            return mHandle;
        }

        #endregion

        #region Operation queue properties and helper methods

        /**
         * Public getter for the OperationQueue. Needed for copying the operations from the WidgetBaseMock
         * queue into the newly created widget.
         */
        public Queue<WidgetOperation> OperationQueue
        {
            get { return mOperationQueue; }
        }

        /**
         * Adds an operation to the operation queue.
         * @param op The operation to be added.
         */
        public void AddOperation(WidgetOperation op)
        {
            // we need to check if the widget was already created or not when the AddOperation is called
            // if so, we'll run the operation on the widget instead of inserting it into the queue
            IWidget widget = mRuntime.GetModule<NativeUIModule>().GetWidget(this.GetHandle());
            if (widget is WidgetBaseMock)
            {
                mOperationQueue.Enqueue(op);
            }
            else
            {
                // we need to check if the widget we got from the runtime can be cast to WidgetBaseWindowsPhone
                if (typeof(WidgetBaseWindowsPhone).IsAssignableFrom(widget.GetType()))
                {
                    MoSync.Util.RunActionOnMainThread(() =>
                    {
                        (widget as WidgetBaseWindowsPhone).RunOperation(op);
                    }, true);
                }
            }
        }

        /**
         * Adds all the values of the operation queue sent as a parameter to the
         * current operation queue.
         */
        public void AddOperations(Queue<WidgetOperation> queue)
        {
            while (queue.Count != 0)
            {
                mOperationQueue.Enqueue(queue.Dequeue());
            }
        }

        /**
         * Runs a WidgetOperation on the current widget.
         * @param operation The widget operation (could be a ADD, INSERT, REMOVE, SET or GET) that
         * needs to be applied on the current widget.
         */
        public void RunOperation(WidgetOperation op)
        {
            // nothing to do here since this method should be overriden by the subclasses that
            // have a view on which to run the operation
        }

        #endregion

        #region Add/Insert/Remove child and parent/child helper methods

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

        public IWidget GetChild(int offset)
        {
            if (offset <= mChildren.Count)
            {
                return mChildren[offset];
            }
            return null;
        }

        public int GetChildCount()
        {
            return mChildren.Count;
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

        #endregion

        #region Set/Get property methods

        protected void SetProperty(PropertyInfo pinfo, String stringValue)
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

        public void SetProperty(String property, String stringValue)
        {
            PropertyInfo pinfo;
            MoSyncWidgetPropertyAttribute pattr = GetPropertyAttribute(property, out pinfo);
            Exception exception = null;
            IWidget i = this;
            if (pinfo == null) throw new InvalidPropertyNameException();
            if (pattr.ShouldExecuteOnMainThread)
            {
                MoSync.Util.RunActionOnMainThread(() =>
                    {
                        try
                        {
                            SetProperty(pinfo, stringValue);
                        }
                        catch (Exception e)
                        {
                            exception = e;
                        }
                    }, false);
                if (null != exception)
                    if (exception.InnerException is InvalidPropertyValueException)
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

        public String GetLastValidSet(String property)
        {
            String res = null;

            WidgetOperation[] operations = mOperationQueue.ToArray();

            // get the last valid set
            for (int i = operations.Length - 1; i >= 0; i--)
            {
                WidgetOperation operation = operations[i];
                if (operation.Type == WidgetOperation.OperationType.SET &&
                    operation.Property.Equals(property))
                {
                    res = operation.Value;
                    break;
                }
            }

            return res;
        }

        public String GetProperty(String property)
        {
            String ret = GetPropertyValue(property);
            return ret;
        }

        protected String GetPropertyValue(String property)
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

        #endregion

        #region Property attribute helper methods

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

        #endregion
    }

    #endregion

    #region UIManager abstract class

    public abstract class UIManager
    {
        public UIManager()
        {
        }

        /**
         * Verifies the existance of the widget type and returns that type if it exists.
         * @param widgetTypeName The name of the widget type.
         * @return The type of the widget as a 'Type' object.
         */
        public Type VerifyWidget(string widgetTypeName)
        {
            Type widgetType = null;

            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                foreach (Type t in Assembly.GetCallingAssembly().GetTypes())
                {
                    if (t.GetInterface("MoSync.IWidget", false) != null && t.Name == widgetTypeName)
                    {
                        widgetType = t;
                        break;
                    }
                }
            });

            return widgetType;
        }

        /**
         * Verifies if the widget type exists and creates it if so.
         */
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

    #endregion
}
