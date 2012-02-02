>Widget Types
>============
>
Screen
------
>A screen is the root of all widgets currently visible on a screen. See Screen properties for the properties available.
>
TabScreen
---------
>A tab screen is a special type of screen that can have any number of sub-screens each switchable using a tab bar. See Tab screen properties for the properties available.
>
StackScreen
-----------
>A stack screen is a special type of screen that manages navigation between a set of screens.
>
Button
------
>A button is a widget that represent a physical button that can be pressed. See Button properties for the properties available.
>
Image
-----
>An image is a widget that can be used to display an image. See Image properties for the properties available.
>
ImageButton
-----------
>An image button is a button that will also affect the appearance of the background image when pressed. See Image button properties for the properties available.
>
Label
-----
>A label is a widget that can be used to display static non-editable text. See Label properties for the properties available.
>
EditBox
-------
>An editbox is an editable label. See Editbox properties for the properties available.
>
ListView
--------
>A list view is a vertical list of widgets that is also scrollable.
>
ListViewItem
------------
>A list view item is a special kind of layout compatible with the list view. That has a predefined common layout for adding text, an icon etc. See List view item properties for the properties available.
>
CheckBox
--------
>A check box is a widget that acts like a physical switch. When pressed it will toggle its internal state that can either be checked or non-checked. See Check box properties for the properties available. If you want to display it with a "light" indicator, as it is on iOS use a Toggle Button instead.
>
HorizontalLayout
----------------
>A horizontal layout is a layout that stacks widgets in the horizontal axis. See Horizontal layout properties for the properties available.
>
VerticalLayout
--------------
>A vertical layout is a layout that stacks widgets in the vertical axis. See Vertical layout properties for the properties available.
>
RelativeLayout
--------------
>A relative layout is a layout that layouts widgets relative to its coordinate system.
>
SearchBar
---------
>A search bar is a special kind of edit box that is used for searching. See Search bar properties for the properties available.
>
NavBar
------
>A nav bar is an iphone specific widget that shows a nav bar with an optional title and back button. See Nav bar properties for the properties available. See Navigation Bar properties for the properties available.
>
GLView
------
>A GL view is a widget that is used to display graphics rendered by the GPU using OpenGL|ES 1.0/1.1 calls. See GL view properties for the properties available.
>
GL2View
-------
>A GL view is a widget that is used to display graphics rendered by the GPU using OpenGL|ES 2.0 calls. See GL view properties for the properties available.
>
CameraPreview
-------------
>A Camera preview widget is a widget that can bound to a camera and contain the live veiw data from the camera.
>
WebView
-------
>A web view is a widget used to render web pages. See Web view properties for the properties available.
>
ProgressBar
-----------
>A progress bar is a visual indicator of progress in some operation. Displays a bar to the user representing how far the operation has progressed. A progress bar can also be made indeterminate, when the length of the task is unknown.
>
ActivityIndicator
-----------------
>An activity indicator is a visual indicator of progress in some operation. It shows a cyclic animation without an indication of progress. It is used when the length of the task is unknown.
>
Slider
------
>A Slider is an extension of ProgressBar that adds a draggable thumb. The user can touch the thumb and drag left or right to set the current progress level.
>
DatePicker
----------
>A Date Picker is a widget for selecting a date. The date can be selected by a year, month, and day spinners. The minimal and maximal date from which dates to be selected can be customized. The picker is initialized with the current system date.
>
TimePicker
----------
>A Time Picker is a widget for selecting time of day, in 24 hour mode. The hour and each minute digit can be controlled by vertical spinners. The hour can be entered by keyboard input. Availabe only on iOS for the moment. See Time Picker properties for the properties available.
>
NumberPicker
------------
>A Number Picker is a widget that enables the user to select a number from a predefined range. Note: for the moment it is available only on iOS. See Number Picker properties for the properties available.
>
VideoView
---------
>A Video View displays a video file. By default, it has attached a controller view that typically contains the buttons like "Play/Pause", "Rewind", "Fast Forward" and a progress slider. See Video View properties for the properties available.
>
ToggleButton
------------
>A Toggle Button is a widget that acts like a physical switch. Displays checked/unchecked states as a button with a "light" indicator and by default accompanied with the text "ON" or "OFF". It is available only on Android, and it's similar to the Check Box on iOS. When pressed it will toggle its internal state that can either be checked or non-checked. See Toggle button properties for the properties available.
>
ModalDialog
-----------
>A dialog is a sort of modal view, that can look different depending on the platform. A Dialog gets visible only after calling maWidgetModalDialogShow(). On Android it is a modal alert dialog. On iPad it is a PopoverController, and on iPhone it is a modal view. When a Dialog widget is created it is empty, it has no content. Any type of widget can be added inside it via maWidgetAddChild syscalls. To show a Dialog call maWidgetModalDialogShow(), to dismiss it call: maWidgetModalDialogHide(), and to delete it call maWidgetDestroy. See Modal Dialog properties for the properties available.
>
PanoramaView
------------
>A panorama control is a Windows Phone 7 specific control Available only on Windows Phone 7 See Panorama View properties for the properties available.
