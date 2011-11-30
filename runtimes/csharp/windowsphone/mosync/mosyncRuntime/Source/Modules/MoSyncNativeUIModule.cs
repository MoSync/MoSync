using System;
using Microsoft.Phone.Controls;
using System.Collections.Generic;

namespace MoSync
{
    public class NativeUIModule : IIoctlModule
    {
        private UIManager mNativeUI;
        private List<IWidget> mWidgets = new List<IWidget>();

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            mNativeUI = new NativeUI.NativeUIWindowsPhone();
            mWidgets.Add(null); // why?

            ioctls.maWidgetCreate = delegate(int _widgetType)
            {
                String widgetType = core.GetDataMemory().ReadStringAtAddress(_widgetType);
                IWidget widget = mNativeUI.CreateWidget(widgetType);
                if (widget == null)
                    return MoSync.Constants.MAW_RES_INVALID_TYPE_NAME;

                widget.SetRuntime(runtime);

                for (int i = 1; i < mWidgets.Count; i++)
                {
                    if (mWidgets[i] == null)
                    {
                        widget.SetHandle(i);
                        mWidgets[i] = widget;
                        return i;
                    }
                }

                mWidgets.Add(widget);
                widget.SetHandle(mWidgets.Count - 1);
                return mWidgets.Count-1;
            };

            ioctls.maWidgetDestroy = delegate(int _widget)
            {
                IWidget widget = mWidgets[_widget];
                widget.RemoveFromParent();
                mWidgets[_widget] = null;
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetAddChild = delegate(int _parent, int _child)
            {
                IWidget parent = mWidgets[_parent];
                IWidget child = mWidgets[_child];
                child.SetParent(parent);
                parent.AddChild(child);
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetRemoveChild = delegate(int _child)
            {
                IWidget child = mWidgets[_child];
                child.RemoveFromParent();
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetInsertChild = delegate(int _parent, int _child, int index)
            {
                IWidget parent = mWidgets[_parent];
                IWidget child = mWidgets[_child];
                parent.InsertChild(child, index);
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetStackScreenPush = delegate(int _stackScreen, int _newScreen)
            {
                IScreen stackScreen = (IScreen)mWidgets[_stackScreen];
                IScreen newScreen = (IScreen)mWidgets[_newScreen];
                (stackScreen as MoSync.NativeUI.StackScreen).Push(newScreen);
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetStackScreenPop = delegate(int _stackScreen)
            {
                IScreen stackScreen = (IScreen)mWidgets[_stackScreen];
                (stackScreen as MoSync.NativeUI.StackScreen).Pop();
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetSetProperty = delegate(int _widget, int _property, int _value)
            {
                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                String value = core.GetDataMemory().ReadStringAtAddress(_value);
                IWidget widget = mWidgets[_widget];
                try
                {
                    widget.SetProperty(property, value);
                }
                catch (InvalidPropertyNameException e)
                {
                    MoSync.Util.Log(e);
                    return MoSync.Constants.MAW_RES_INVALID_PROPERTY_NAME;
                }
                catch (InvalidPropertyValueException e)
                {
                    MoSync.Util.Log(e);
                    return MoSync.Constants.MAW_RES_INVALID_PROPERTY_VALUE;
                }

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetGetProperty = delegate(int _widget, int _property, int _value, int _bufSize)
            {
                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                IWidget widget = mWidgets[_widget];
                try
                {
                    String value = widget.GetProperty(property);
                    core.GetDataMemory().WriteStringAtAddress(_value, value, _bufSize);
                }
                catch (InvalidPropertyNameException e)
                {
                    MoSync.Util.Log(e);
                    return MoSync.Constants.MAW_RES_INVALID_PROPERTY_NAME;
                }

                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetScreenShow = delegate(int _screenHandle)
            {
                IScreen screen = (IScreen)mWidgets[_screenHandle];
                screen.Show();
                return MoSync.Constants.MAW_RES_OK;
            };
        }
    }
}
