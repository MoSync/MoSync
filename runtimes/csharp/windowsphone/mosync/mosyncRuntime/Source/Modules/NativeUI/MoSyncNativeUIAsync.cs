using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using System.Collections.Generic;

namespace MoSync
{
    namespace NativeUI
    {
        #region Async UIManager

        /**
         * Handles the set, get, add, insert and remove operations on a widget.
         */
        public class AsyncNativeUIWindowsPhone : UIManager
        {
            private PhoneApplicationFrame mFrame;

            /**
             * Contains a reference to the runtime Core - needed by the get operations.
             */
            private Runtime mRuntime;

            public AsyncNativeUIWindowsPhone()
                : base()
            {
                //This should always be a PhoneApplicationFrame.
                mFrame = (PhoneApplicationFrame)Application.Current.RootVisual;
            }

            public void SetProperty(IWidget widget, string propertyName, string propertyValue)
            {
                if (widget is WidgetBaseMock)
                {
                    WidgetOperation setOperation = new WidgetOperation(propertyName, propertyValue);
                    widget.AddOperation(setOperation);
                }
                else
                {
                    widget.SetProperty(propertyName, propertyValue);
                }
            }

            public String GetProperty(IWidget widget, string propertyName)
            {
                if (widget is WidgetBaseMock)
                {
                    String propertyValue = widget.GetLastValidSet(propertyName);

                    if (propertyValue != null)
                    {
                        return propertyValue;
                    }

                    widget = mRuntime.GetModule<NativeUIModule>().GetChildSync(widget.GetHandle());
                }

                return widget.GetProperty(propertyName);
            }

            public void AddChild(IWidget parent, IWidget child)
            {
                if (parent is WidgetBaseMock)
                {
                    WidgetOperation addChildOperation = new WidgetOperation(WidgetOperation.OperationType.ADD, child.GetHandle());
                    parent.AddOperation(addChildOperation);
                }
                else
                {
                    if (child is WidgetBaseMock)
                    {
                        child = mRuntime.GetModule<NativeUIModule>().GetChildSync(child.GetHandle());
                    }
                    System.Diagnostics.Debug.WriteLine("MoSyncNativeUIAsync: Add child; Parent: " +
                        parent.GetType().ToString() + "(" + parent.GetHandle().ToString() + ")" +
                        "Child: " +
                        child.GetType().ToString() + "(" + child.GetHandle().ToString() + ")");
                    parent.AddChild(child);
                }
            }

            public void InsertChild(IWidget parent, IWidget child, int index)
            {
                if (parent is WidgetBaseMock)
                {
                    WidgetOperation insertChildOperation = new WidgetOperation(child.GetHandle(), index);
                    parent.AddOperation(insertChildOperation);
                }
                else
                {
                    if (child is WidgetBaseMock)
                    {
                        child = mRuntime.GetModule<NativeUIModule>().GetChildSync(child.GetHandle());
                    }
                    parent.InsertChild(child, index);
                }
            }

            public void RemoveChild(IWidget child)
            {
                if (child.GetParent() is WidgetBaseMock)
                {
                    WidgetOperation removeChildOperation = new WidgetOperation(WidgetOperation.OperationType.REMOVE, child.GetHandle());
                    child.GetParent().AddOperation(removeChildOperation);
                }
                else
                {
                    if (child is WidgetBaseMock)
                    {
                        child = mRuntime.GetModule<NativeUIModule>().GetChildSync(child.GetHandle());
                    }
                    child.RemoveFromParent();
                }
            }

            public void SetRuntime(Runtime runtime)
            {
                this.mRuntime = runtime;
            }
        }

        #endregion
    }

    #region WidgetBaseMock and WidgetOperation classes

    /**
     * This class serves as a placeholder for a widget that has not been created yet. After the
     * widget is created, all the data from this mock is copied in the new object and the mock
     * is replaced with the new widget.
     */
    public class WidgetBaseMock : WidgetBase
    {
        public WidgetBaseMock()
        {
            mOperationQueue = new Queue<WidgetOperation>();
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
         * The insert operation comes with the index where the insert needs to be done.
         */
        private int mIndex;

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
         * Public getter for the index property (used by the INSERT operation).
         */
        public int Index
        {
            get
            {
                return mIndex;
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
         * Constructor that creates a ADD or REMOVE widget operation.
         * @param type The operation type (ADD, INSERT or REMOVE).
         * @param handle The handle of the child widget.
         */
        public WidgetOperation(OperationType type, int handle)
        {
            if (type == OperationType.ADD || type == OperationType.REMOVE)
            {
                mType = type;
                mHandle = handle;
                mPropertyName = "";
                mPropertyValue = "";
            }
            else
            {
                // TODO SA: think of a better option for a default case
                // if the type is not one of the mentioned before, ADD will be considered by default as
                // the operation type;
                mType = OperationType.ADD;
                mHandle = handle;
                mPropertyName = "";
                mPropertyValue = "";
            }
        }

        /**
         * Constructor that creates INSERT widget operation.
         * @param handle The handle of the child widget.
         * @param index The index where the child widget needs to be inserted.
         */
        public WidgetOperation(int handle, int index)
        {
            mType = OperationType.INSERT;
            mHandle = handle;
            mPropertyName = "";
            mPropertyValue = "";
            mIndex = index;
        }
    }

    #endregion
}
