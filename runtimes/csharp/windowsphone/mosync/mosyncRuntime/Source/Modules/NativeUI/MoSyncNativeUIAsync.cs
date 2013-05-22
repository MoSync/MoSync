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
using System.Reflection;

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
            /**
             * Contains a reference to the runtime - needed by the get operations.
             */
            private Runtime mRuntime;

            /**
             * Contains the name of the property validation method that should be contained by all widgets.
             */
            private const string mValidateMethodName = "ValidateProperty";

            public AsyncNativeUIWindowsPhone(Runtime runtime)
                : base()
            {
                this.mRuntime = runtime;
            }

            /**
             * Handles the property setting for a widget. If the widget is not yet created, the
             * set operation goes into the operation queue.
             * @param widget The widget that recieved a set property syscall.
             * @param propertyName The name of the property to be set.
             * @param propertyValue The value of the property to be set.
             */
            public void SetProperty(IWidget widget, string propertyName, string propertyValue)
            {
                if (widget is WidgetBaseMock)
                {
                    // we check to see if the property is valid
                    Type widgetType = mRuntime.GetModule<NativeUIModule>().GetWidgetType(widget.GetHandle());
                    CheckPropertyValidity(widgetType, propertyName, propertyValue);

                    WidgetOperation setOperation = WidgetOperation.CreateSetOperation(propertyName, propertyValue);
                    widget.AddOperation(setOperation);
                }
                else
                {
                    widget.SetProperty(propertyName, propertyValue);
                }
            }

            /**
             * Handles the property getting for a widget. If the widget is not yet created, we return
             * the last valid set for that property. If there is no valid set, we wait for the widget creation
             * and then we return the value of the property.
             * @param widget The widget that recieved a get property syscall.
             * @param propertyName The name of the property for which a value is needed.
             */
            public String GetProperty(IWidget widget, string propertyName)
            {
                String propertyValue = null;
                if (widget is WidgetBaseMock)
                {
                    propertyValue = widget.GetLastValidSet(propertyName);

                    if (propertyValue == null)
                    {
                        widget = mRuntime.GetModule<NativeUIModule>().GetWidgetSync(widget.GetHandle());
                        propertyValue = widget.GetProperty(propertyName);
                    }
                }
                else
                {
                    propertyValue = widget.GetProperty(propertyName);
                }

                return propertyValue;
            }

            /**
             * Handles the add child. If the parent is not yet created, the add operation goes into the operation
             * queue. If the parent exists but the child hasn't been created, we wait for the child creation and then
             * add it to the parent.
             * @param parent The parent widget.
             * @param child The child widget.
             */
            public void AddChild(IWidget parent, IWidget child)
            {
                if (parent is WidgetBaseMock)
                {
                    WidgetOperation addChildOperation = WidgetOperation.CreateAddOperation(child.GetHandle());
                    parent.AddOperation(addChildOperation);
                }
                else
                {
                    if (child is WidgetBaseMock)
                    {
                        child = mRuntime.GetModule<NativeUIModule>().GetWidgetSync(child.GetHandle());
                    }

                    MoSync.Util.RunActionOnMainThread(() =>
                    {
                        child.SetParent(parent);
                        parent.AddChild(child);
                    }, true);
                }
            }

            /**
             * Handles the insert child. If the parent is not yet created, the insert operation goes into the operation
             * queue. If the parent exists but the child hasn't been created, we wait for the child creation and then
             * insert it into the parent.
             * @param parent The parent widget.
             * @param child The child widget.
             */
            public void InsertChild(IWidget parent, IWidget child, int index)
            {
                if (parent is WidgetBaseMock)
                {
                    WidgetOperation insertChildOperation = WidgetOperation.CreateInsertOperation(child.GetHandle(), index);
                    parent.AddOperation(insertChildOperation);
                }
                else
                {
                    if (child is WidgetBaseMock)
                    {
                        child = mRuntime.GetModule<NativeUIModule>().GetWidgetSync(child.GetHandle());
                    }

                    MoSync.Util.RunActionOnMainThread(() =>
                    {
                        child.SetParent(parent);
                        parent.InsertChild(child, index);
                    }, true);
                }
            }

            /**
             * Handles the remove child. If the parent is not yet created, the remove operation goes into the operation
             * queue. If the parent exists but the child hasn't been created, we wait for the child creation and then
             * remove it from the parent.
             * @param child The child widget.
             */
            public void RemoveChild(IWidget child)
            {
                IWidget parent = child.GetParent();
                if (parent is WidgetBaseMock)
                {
                    WidgetOperation removeChildOperation = WidgetOperation.CreateRemoveOperation(child.GetHandle());
                    parent.AddOperation(removeChildOperation);
                }
                else
                {
                    if (child is WidgetBaseMock)
                    {
                        child = mRuntime.GetModule<NativeUIModule>().GetWidgetSync(child.GetHandle());
                    }

                    MoSync.Util.RunActionOnMainThread(() =>
                    {
                        child.RemoveFromParent();
                    }, true);
                }
            }

            #region Property validation

            /**
             * Checks if a property has the correct name and value for a certain widget type.
             * @param widgetType The widget type on which we need to do the property validity checking.
             * @param propertyName The name of the property that needs to be checked.
             * @param propertyValue The value of the property that needs to be checked.
             * @throws InvalidPropertyNameException This exception is thrown if the set property name is incorrect.
             * @throws InvalidPropertyValueException This exception is thrown if the set property value is incorrect.
             */
            public void CheckPropertyValidity(Type widgetType, String propertyName, String propertyValue)
            {
                CheckPropertyName(widgetType, propertyName);
                CheckPropertyValue(widgetType, propertyName, propertyValue);
            }

            /**
             * Using reflection, calls a method on a widget that checks if the name for the property
             * that needs to be set is correct.
             * @param widgetType The widget type on which we need to do the property validity checking.
             * @param propertyName The name of the property that needs to be checked.
             * @throws InvalidPropertyNameException This exception is thrown if the set property name is incorrect.
             */
            private void CheckPropertyName(Type widgetType, String propertyName)
            {
                bool propertyExists = false;
                // we first check to see if the widgetType has that property implemented
                foreach (PropertyInfo pinfo in widgetType.GetProperties())
                {
                    foreach (Attribute attr in pinfo.GetCustomAttributes(false))
                    {
                        if (attr.GetType() == typeof(MoSyncWidgetPropertyAttribute))
                        {
                            MoSyncWidgetPropertyAttribute e = (MoSyncWidgetPropertyAttribute)attr;
                            if (e.Name.ToLower().Equals(propertyName.ToLower()))
                            {
                                propertyExists = true;
                            }
                        }
                    }
                }

                // if the property doesn't exist, we throw a InvalidPropertyNameException
                if (!propertyExists)
                {
                    throw new InvalidPropertyNameException();
                }
            }

            /**
             * Using reflection, calls a method on a widget that checks if the value for the property
             * that needs to be set is correct.
             * @param widgetType The widget type on which we need to do the property validity checking.
             * @param propertyName The name of the property that needs to be checked.
             * @param propertyValue The value of the property that needs to be checked.
             * @throws InvalidPropertyValueException This exception is thrown if the set property value is incorrect.
             */
            private void CheckPropertyValue(Type widgetType, String propertyName, String propertyValue)
            {
                try
                {
                    MethodInfo minfo = widgetType.GetMethod(mValidateMethodName);
                    string[] validateMethodParams = new string[] { propertyName, propertyValue };
                    object propertyValueValid = minfo.Invoke(null, validateMethodParams);
                    // if the widget validation returned false, we throw a InvalidPropertyValueException
                    if (propertyValueValid.Equals(false))
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                catch (Exception e)
                {
                    // TODO SA: should I throw the exception even if it's not InvalidPropertyValueException?

                    // if the exception caught is InvalidPropertyValueException, we pass it to the
                    // next level (syscall level)
                    if (e is InvalidPropertyValueException)
                    {
                        throw e;
                    }
                }
            }

            #endregion
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

        public OperationType Type
        {
            get
            {
                return mType;
            }
        }

        public string Property
        {
            get
            {
                return mPropertyName;
            }
        }

        public string Value
        {
            get
            {
                return mPropertyValue;
            }
        }

        public int Handle
        {
            get
            {
                return mHandle;
            }
        }

        public int Index
        {
            get
            {
                return mIndex;
            }
        }

        public WidgetOperation(OperationType type, string property, string value, int handle, int index)
        {
            this.mType = type;
            this.mPropertyName = property;
            this.mPropertyValue = value;
            this.mHandle = handle;
            this.mIndex = index;
        }

        static public WidgetOperation CreateSetOperation(string propertyName, string propertyValue)
        {
            WidgetOperation op = new WidgetOperation(OperationType.SET, propertyName, propertyValue, -1, -1);

            return op;
        }

        static public WidgetOperation CreateGetOperation(string propertyName)
        {
            WidgetOperation op = new WidgetOperation(OperationType.GET, propertyName, "", -1, -1);

            return op;
        }

        static public WidgetOperation CreateAddOperation(int handle)
        {
            WidgetOperation op = new WidgetOperation(OperationType.ADD, "", "", handle, -1);

            return op;
        }

        static public WidgetOperation CreateInsertOperation(int handle, int index)
        {
            WidgetOperation op = new WidgetOperation(OperationType.REMOVE, "", "", handle, index);

            return op;
        }

        static public WidgetOperation CreateRemoveOperation(int handle)
        {
            WidgetOperation op = new WidgetOperation(OperationType.REMOVE, "", "", handle, -1);

            return op;
        }
    }

    #endregion
}
