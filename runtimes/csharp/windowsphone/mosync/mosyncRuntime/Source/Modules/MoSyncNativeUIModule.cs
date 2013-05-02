using System;
using Microsoft.Phone.Controls;
using System.Collections.Generic;
using System.Windows;
using System.Threading;
using MoSync.NativeUI;

namespace MoSync
{
    public class NativeUIModule : IIoctlModule
    {
        private NativeUI.AsyncNativeUIWindowsPhone mNativeUI;

        /**
         * A reference to the last shown screen.
         */
        private IScreen mCurrentScreen = null;
        private List<IWidget> mWidgets = new List<IWidget>();

        private Dictionary<int, Thread> mWidgetThreadDictionary;
        private Dictionary<int, Type> mWidgetTypeDictionary;

        // TODO SA: only for benchmarking - to remove
        public DateTime mAppLaunchedTime;

        #region Widget Add/Get

        /*
         * Ads a widget to the widgets array.
         * @param adControl The widget to be added.
         */
        public int AddWidget(IWidget widget)
        {
            mWidgets.Add(widget);
            return mWidgets.Count-1;
        }

        public IWidget GetWidget(int handle)
        {
            if (handle < 0 || handle >= mWidgets.Count)
                return null;
            IWidget w = mWidgets[handle];
            return w;
        }

        #endregion

        #region Asynchronous Widget Creation methods

        /**
         * Starts a new thread that handles a widget creation based on the already set widget handle
         * and widget type.
         * @param widgetHandle The handle of the widget that needs to be created.
         * @param widgetType The type of the widget that needs to be created.
         */
        private void StartWidgetCreationThread(int widgetHandle, Type widgetType)
        {
            mWidgetTypeDictionary.Add(widgetHandle, widgetType);

            Thread createWidgetThread = new Thread(() => CreateWidgetAsync(widgetHandle, widgetType));
            createWidgetThread.Name = "Creating widget " + widgetType.ToString() + " handle: " + widgetHandle.ToString();
            createWidgetThread.IsBackground = true;
            createWidgetThread.Start();

            // we need to create a map between the widget handle and the thread that handles its creation
            // in order to wait for the thread to finish if a add/remove/insert child syscall reaches the
            // runtime and the child was not created yet
            mWidgetThreadDictionary.Add(widgetHandle, createWidgetThread);
        }

        /**
         * Run in a new thread, this method creates a new widget based on a widget handle and widget type.
         * It runs the widget creation on the UI thread synchronously (because the Activator class can only
         * create UI widgets on the UI thread) and after the creation is over it runs the operation queue
         * on the UI thread.
         * @param widgetHandle The handle of the widget that needs to be created.
         * @param widgetType The type of the widget that needs to be created.
         */
        private void CreateWidgetAsync(int widgetHandle, Type widgetType)
        {
            IWidget widget = null;
            IWidget widgetMock = mWidgets[widgetHandle];
            // if the child was already created we won't create it again
            // this situation might rise in the following way:
            // the widget operation queue is run on an already created parent and a add child operation
            // is applied before the a child is created. The WaitForWidgetCreation method will create
            // the child on the UI thread (see WaitForWidgetCreation and CreateWidgetSync methods) so
            // we don't need to recreate the child here
            if (!(widgetMock is WidgetBaseMock))
            {
                return;
            }
            // create the widget on the UI thread sync
            MoSync.Util.RunActionOnMainThread(() =>
            {
                widget = Activator.CreateInstance(widgetType) as IWidget;
            }, true);
            widget.SetHandle(widgetHandle);
            widget.AddOperations((widgetMock as WidgetBaseMock).OperationQueue);
            widget.SetRuntime(widgetMock.GetRuntime());
            // lock the mWidgets array when this thread starts manipulating it
            lock (mWidgets)
            {
                mWidgets[widgetHandle] = widget;
            }

            // run the operation queue on the newly created widget
            MoSync.Util.RunActionOnMainThread(() =>
            {
                (widget as WidgetBaseWindowsPhone).RunOperationQueue();
            }, true);

            // TODO SA: remove - for benchmarking only
            System.Diagnostics.Debug.WriteLine("Widget created: " +
                DateTime.Now.Minute.ToString() + ":" +
                DateTime.Now.Second.ToString() + ":" +
                DateTime.Now.Millisecond.ToString());
            TimeSpan timeDifference = DateTime.Now - mAppLaunchedTime;
            System.Diagnostics.Debug.WriteLine("Time till app launch: " + timeDifference.Minutes.ToString() + ":" +
                timeDifference.Seconds.ToString() + ":" +
                timeDifference.Milliseconds.ToString());
        }

        /**
         * Must be run on the UI thread. Creates a widget in a synchrounous way.
         * @param widgetHandle The handle of the widget that needs to be created.
         * @param widgetType The type of the widget that needs to be created.
         */
        private void CreateWidgetSync(int widgetHandle, Type widgetType)
        {
            IWidget widget = Activator.CreateInstance(widgetType) as IWidget;
            IWidget widgetMock = mWidgets[widgetHandle];
            widget.SetHandle(widgetHandle);
            widget.AddOperations((widgetMock as WidgetBaseMock).OperationQueue);
            widget.SetRuntime(widgetMock.GetRuntime());
            // lock the mWidgets array when this thread starts manipulating it
            mWidgets[widgetHandle] = widget;

            (widget as WidgetBaseWindowsPhone).RunOperationQueue();

            // TODO SA: remove - for benchmarking only
            System.Diagnostics.Debug.WriteLine("Widget created: " +
                DateTime.Now.Minute.ToString() + ":" +
                DateTime.Now.Second.ToString() + ":" +
                DateTime.Now.Millisecond.ToString());
            TimeSpan timeDifference = DateTime.Now - mAppLaunchedTime;
            System.Diagnostics.Debug.WriteLine("Time till app launch: " + timeDifference.Minutes.ToString() + ":" +
                timeDifference.Seconds.ToString() + ":" +
                timeDifference.Milliseconds.ToString());
        }

        /**
         * Waits for the creation of a widget.
         * @param widgetHandle The handle of the widget that needs to be available after
         * the call of this method.
         */
        public void WaitForWidgetCreation(int widgetHandle)
        {
            IWidget widget = mWidgets[widgetHandle];
            // if the widget was already created, we don't have to wait
            if (!(widget is WidgetBaseMock))
                return;

            // if we're on the main thread, we create the widget in a synchrounous way.
            // this might happen when the operation queue is being run on the main thread and
            // a child widget hasn't been created. Because the widget creation thread does things
            // on the UI thread, we can't use the thread 'Join' method so instead we create the
            // widget in a synchronous way
            if (Deployment.Current.Dispatcher.CheckAccess())
            {
                Type widgetType = null;
                mWidgetTypeDictionary.TryGetValue(widgetHandle, out widgetType);
                if (widgetType != null)
                {
                    CreateWidgetSync(widgetHandle, widgetType);
                    return;
                }
            }
            else
            {
                // if the widget wasn't created iet, we get its creation thread and join it
                // with the current thread
                Thread widgetCreationThread = null;
                mWidgetThreadDictionary.TryGetValue(widgetHandle, out widgetCreationThread);

                if (widgetCreationThread != null)
                {
                    widgetCreationThread.Join();
                }
            }
        }

        /**
         * Waits for a widget creation and returns it.
         * @param widgetHandle The handle of the widget that needs to be returned.
         */
        public IWidget GetChildSync(int widgetHandle)
        {
            WaitForWidgetCreation(widgetHandle);

            return mWidgets[widgetHandle];
        }

        #endregion

        #region Widget type helper methods

        public Type GetWidgetType(int widgetHandle)
        {
            Type widgetType = null;
            mWidgetTypeDictionary.TryGetValue(widgetHandle, out widgetType);
            return widgetType;
        }

        #endregion

        #region Ioctls implementation

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            mNativeUI = new NativeUI.AsyncNativeUIWindowsPhone();
            mNativeUI.SetRuntime(runtime);
            //mWidgets.Add(null); // why?

            // initialize the widget thread dictionary
            mWidgetThreadDictionary = new Dictionary<int, Thread>();

            mWidgetTypeDictionary = new Dictionary<int, Type>();

            /**
             * This will add a OrientationChanged event handler to the Application.Current.RootVisual, this is application wide.
             */
            (Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).OrientationChanged += delegate(object from, Microsoft.Phone.Controls.OrientationChangedEventArgs args)
            {
                PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);

                int mosyncScreenOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT_UP;
                switch (currentPage.Orientation)
                {
                    case PageOrientation.Landscape:
                        mosyncScreenOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE;
                        break;
                    case PageOrientation.LandscapeLeft:
                        mosyncScreenOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT;
                        break;
                    case PageOrientation.LandscapeRight:
                        mosyncScreenOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
                        break;
                    case PageOrientation.Portrait:
                        mosyncScreenOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT_UP;
                        break;
                    case PageOrientation.PortraitDown:
                        mosyncScreenOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
                        break;
                    case PageOrientation.PortraitUp:
                        mosyncScreenOrientation = MoSync.Constants.MA_SCREEN_ORIENTATION_PORTRAIT_UP;
                        break;
                }

                // Post event handled Moblet.
                Memory eventData = new Memory(8);
                const int MAEventData_eventType = 0;
                const int MAEventData_orientation = 4;
                eventData.WriteInt32(MAEventData_eventType, MoSync.Constants.EVENT_TYPE_ORIENTATION_DID_CHANGE);
                eventData.WriteInt32(MAEventData_orientation, mosyncScreenOrientation);

                runtime.PostEvent(new Event(eventData));
            };

            ioctls.maWidgetCreate = delegate(int _widgetType)
            {
                String widgetTypeName = core.GetDataMemory().ReadStringAtAddress(_widgetType);

                Type widgetType = mNativeUI.VerifyWidget(widgetTypeName);
                if (widgetType == null)
                {
                    return MoSync.Constants.MAW_RES_INVALID_TYPE_NAME;
                }
                IWidget widget = new WidgetBaseMock();
                widget.SetRuntime(runtime);

                for (int i = 0; i < mWidgets.Count; i++)
                {
                    if (mWidgets[i] == null)
                    {
                        widget.SetHandle(i);
                        mWidgets[i] = widget;

                        StartWidgetCreationThread(i, widgetType);

                        return i;
                    }
                }

                mWidgets.Add(widget);
                widget.SetHandle(mWidgets.Count - 1);

                StartWidgetCreationThread(mWidgets.Count - 1, widgetType);

                return mWidgets.Count - 1;
            };

            ioctls.maWidgetDestroy = delegate(int _widget)
            {
				if (_widget < 0 || _widget >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                IWidget widget = mWidgets[_widget];
                if (widget != null)
                {
                    widget.RemoveFromParent();
                    mWidgets[_widget] = null;
                }
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
                mNativeUI.AddChild(parent, child);
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetRemoveChild = delegate(int _child)
            {
				if (_child < 0 || _child >= mWidgets.Count)
					return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                IWidget child = mWidgets[_child];
                // only the child is needed - it has a reference to its parent
                mNativeUI.RemoveChild(child);
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
                mNativeUI.InsertChild(parent, child, index);
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
                    mNativeUI.SetProperty(widget, property, value);
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
                    // String value = widget.GetProperty(property);
                    String value = mNativeUI.GetProperty(widget, property);
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
                {
                    return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                }

                IScreen screen = null;

                if(mWidgets[_screenHandle] is IScreen)
                {
                    screen = (IScreen)mWidgets[_screenHandle];
                }
                else
                {
                    return MoSync.Constants.MAW_RES_INVALID_SCREEN;
                }

                mCurrentScreen = screen;

                screen.Show();
                return MoSync.Constants.MAW_RES_OK;
            };

            ioctls.maWidgetScreenShowWithTransition = delegate(int _screenHandle, int _screenTransitionType, int _screenTransitionDuration)
            {
                // Windows Phone Toolkit screen transitions do not have an time argument so _screenTransitionDuration
                // will be ignored on Windows platform.
                if (_screenHandle < 0 || _screenHandle >= mWidgets.Count)
                {
                    return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                }

                IScreen screen = null;

                if (mWidgets[_screenHandle] is IScreen)
                {
                    screen = (IScreen)mWidgets[_screenHandle];
                }
                else
                {
                    return MoSync.Constants.MAW_RES_INVALID_SCREEN;
                }

                mCurrentScreen = screen;

                // If transition type is not available on this platform do show without transitions but return error code.
                if (!NativeUI.MoSyncScreenTransitions.isTransitionAvailable(_screenTransitionType))
                {
                    screen.ShowWithTransition(MoSync.Constants.MAW_TRANSITION_TYPE_NONE);
                    return MoSync.Constants.MAW_RES_INVALID_SCREEN_TRANSITION_TYPE;
                }

                screen.ShowWithTransition(_screenTransitionType);
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

        #endregion

        #region Back button handler

        /**
         * Handles the back button pressed event.
         * @return true if the event has been consumed, false otherwise.
         */
        public bool HandleBackButtonPressed()
        {
            if (mCurrentScreen != null)
            {
                return mCurrentScreen.HandleBackButtonPressed();
            }
            return false;
        }

        #endregion
    }
}
