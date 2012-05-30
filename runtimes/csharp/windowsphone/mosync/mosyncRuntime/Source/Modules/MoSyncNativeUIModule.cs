using System;
using Microsoft.Phone.Controls;
using System.Collections.Generic;

namespace MoSync
{
    public class NativeUIModule : IIoctlModule
    {
        private UIManager mNativeUI;
        private List<IWidget> mWidgets = new List<IWidget>();

		public IWidget GetWidget(int handle)
		{
			if (handle < 0 || handle >= mWidgets.Count)
				return null;
			IWidget w = mWidgets[handle];
			return w;
		}

        /*
         * Ads a widget to the widgets array.
         * @param adControl The widget to be added.
         */
        public int AddWidget(IWidget widget)
        {
            mWidgets.Add(widget);
            return mWidgets.Count-1;
        }

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            mNativeUI = new NativeUI.NativeUIWindowsPhone();
            //mWidgets.Add(null); // why?

            ioctls.maWidgetCreate = delegate(int _widgetType)
            {
                String widgetType = core.GetDataMemory().ReadStringAtAddress(_widgetType);
                IWidget widget = mNativeUI.CreateWidget(widgetType);
                if (widget == null)
                    return MoSync.Constants.MAW_RES_INVALID_TYPE_NAME;

                widget.SetRuntime(runtime);

                for (int i = 0; i < mWidgets.Count; i++)
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
				if (_widget < 0 || _widget >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                IWidget widget = mWidgets[_widget];
                widget.RemoveFromParent();
                mWidgets[_widget] = null;
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetAddChild = delegate(int _parent, int _child)
            {
				if (_parent < 0 || _parent >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
				IWidget parent = mWidgets[_parent];
				if (_child < 0 || _child >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
				IWidget child = mWidgets[_child];
                child.SetParent(parent);
                parent.AddChild(child);
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetRemoveChild = delegate(int _child)
            {
				if (_child < 0 || _child >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                IWidget child = mWidgets[_child];
                child.RemoveFromParent();
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetInsertChild = delegate(int _parent, int _child, int index)
            {
				if (_parent < 0 || _parent >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                IWidget parent = mWidgets[_parent];
				if (_child < 0 || _child >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
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
				if (_widget < 0 || _widget >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                IWidget widget = mWidgets[_widget];
                try
                {
                    widget.SetProperty(property, value);
                }
                catch (InvalidPropertyNameException)
                {
                    MoSync.Util.Log(widget.GetType().ToString() + " invalid property name: " + property);
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
				if (_widget < 0 || _widget >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
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
				if (_screenHandle < 0 || _screenHandle >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                IScreen screen = (IScreen)mWidgets[_screenHandle];
                screen.Show();
                return MoSync.Constants.MAW_RES_OK;
            };

            /*
             * Implementation for maWidgetScreenAddOptionsMenuItem
             *
             * @param _widget the widget handle
             * @param _title the option menu item title
             * @param _iconPath the option menu item path
             *        Note: if the _iconPredefined param is 1 then the _iconPath
             *              will store a code representing the name of the icon file,
             *              without extension. Otherwise it should contain the name of the
             *              file. (e.g. "applicationBarIcon1.png")
             * @param _iconPredefined if the value is 1 it means that we expect a predefined icon
             *        otherwise it will create the path using the _iconPath as it was previously
             *        explained
             */
            ioctls.maWidgetScreenAddOptionsMenuItem = delegate(int _widget, int _title, int _iconPath, int _iconPredefined)
            {
                //This represents the hardcoded folder name for the application bar icons
                String applicationBarIconsFolder = "/AppBar.Icons/";

                //if _widget < 0 => no screen parent
                if (_widget < 0 || _widget >= mWidgets.Count)
                    return MoSync.Constants.MAW_RES_INVALID_HANDLE;

                IScreen screen = (IScreen)mWidgets[_widget];

                //Read the icon path
                string iconPath = core.GetDataMemory().ReadStringAtAddress(_iconPath);

                //If the iconPath is not empty and we don't have a predefined icon
                //then we have an ApplicationBarButton object with a given icon and text.
                if (!iconPath.Equals("") && 0 == _iconPredefined && screen.GetApplicationBar().Buttons.Count < 5)
                {
                    //Read the text
                    string buttonText = core.GetDataMemory().ReadStringAtAddress(_title);

                    //Create the native object.
                    Microsoft.Phone.Shell.ApplicationBarIconButton btn = new Microsoft.Phone.Shell.ApplicationBarIconButton();

                    //Create the icon path.
                    btn.IconUri = new Uri(applicationBarIconsFolder + iconPath, UriKind.RelativeOrAbsolute);
                    btn.Text = buttonText;

                    //Associate an index to the native object.
                    int btnIndex = screen.AddApplicationBarItemIndex(btn);

                    btn.Click += new EventHandler(
                        delegate(object from, EventArgs target)
                        {
                            Memory eventData = new Memory(12);
                            const int MAWidgetEventData_eventType = 0;
                            const int MAWidgetEventData_widgetHandle = 4;
                            const int MAWidgetEventData_itemIndex = 8;
                            eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_OPTIONS_MENU_ITEM_SELECTED);
                            eventData.WriteInt32(MAWidgetEventData_widgetHandle, _widget);
                            eventData.WriteInt32(MAWidgetEventData_itemIndex, btnIndex);
                            //Posting a CustomEvent
                            runtime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                        });

                    screen.GetApplicationBar().Buttons.Add(btn);
                    screen.EnableApplicationBar();
                    return btnIndex;
                }
                //If the iconPath is not empty and we have a predefined icon
                //then we have an ApplicationBarButton object with a predefined icon and text.
                else if (!iconPath.Equals("") && _iconPredefined > 0 && screen.GetApplicationBar().Buttons.Count < 5)
                {
                    //Read the text.
                    string buttonText = core.GetDataMemory().ReadStringAtAddress(_title);

                    //Create the native object.
                    Microsoft.Phone.Shell.ApplicationBarIconButton btn = new Microsoft.Phone.Shell.ApplicationBarIconButton();

                    //Create the icon path.
                    btn.IconUri = new Uri(applicationBarIconsFolder + iconPath + ".png", UriKind.RelativeOrAbsolute);
                    btn.Text = buttonText;

                    //Associate an index to the native object.
                    int btnIndex = screen.AddApplicationBarItemIndex(btn);

                    btn.Click += new EventHandler(
                        delegate(object from, EventArgs target)
                        {
                            Memory eventData = new Memory(12);
                            const int MAWidgetEventData_eventType = 0;
                            const int MAWidgetEventData_widgetHandle = 4;
                            const int MAWidgetEventData_itemIndex = 8;
                            eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_OPTIONS_MENU_ITEM_SELECTED);
                            eventData.WriteInt32(MAWidgetEventData_widgetHandle, _widget);
                            eventData.WriteInt32(MAWidgetEventData_itemIndex, btnIndex);
                            //Posting a CustomEvent
                            runtime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                        });

                    screen.GetApplicationBar().Buttons.Add(btn);
                    screen.EnableApplicationBar();

                    //Return the index associated to the item.
                    return btnIndex;
                }
                //If the iconPath is empty then we have an ApplicationBarMenuItem.
                else
                {
                    //Read the text.
                    string menuItemText = core.GetDataMemory().ReadStringAtAddress(_title);

                    //Create the native object.
                    Microsoft.Phone.Shell.ApplicationBarMenuItem menuItem = new Microsoft.Phone.Shell.ApplicationBarMenuItem();
                    menuItem.Text = menuItemText;

                    //Associate an index to the native object.
                    int menuIndex = screen.AddApplicationBarItemIndex(menuItem);

                    menuItem.Click += new EventHandler(
                        delegate(object from, EventArgs target)
                        {
                            Memory eventData = new Memory(12);
                            const int MAWidgetEventData_eventType = 0;
                            const int MAWidgetEventData_widgetHandle = 4;
                            const int MAWidgetEventData_itemIndex = 8;
                            eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_OPTIONS_MENU_ITEM_SELECTED);
                            eventData.WriteInt32(MAWidgetEventData_widgetHandle, _widget);
                            eventData.WriteInt32(MAWidgetEventData_itemIndex, menuIndex);
                            //Posting a CustomEvent
                            runtime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                        });

                    screen.GetApplicationBar().MenuItems.Add(menuItem);
                    screen.EnableApplicationBar();

                    //Return the index associated to the item.
                    return menuIndex;
                }
            };
        }
    }
}
