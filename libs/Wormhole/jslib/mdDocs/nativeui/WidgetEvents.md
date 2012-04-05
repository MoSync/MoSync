>Widget Events
>=============
>
PointerPressed
--------------
>A pointer pressed event has occurred.
PointerReleased
----------------
>A pointer released event has occurred.
ContentLoaded
--------------
>This event is not used and has been deprecated.
Clicked
--------
>Sent when something is clicked.
ItemClicked
------------
>Sent when an item in a list view is clicked.
TabChanged
-----------
>Sent when a tab widget has changed to a new tab.
GLViewReady
------------
>Sent when an GLView has been initialized and is ready for setup.
WebViewURLChanged
------------------
>Sent when the URL of a web view changes.
StackScreenPopped
------------------
>Sent when a screen has been popped from a stack screen.
SliderValueChanged
-------------------
>Sent when the progress level has been changed. This includes changes that were initiated by the user through a touch gesture, or arrow key/trackball as well as changes that were initiated programmatically.
DatePickerValueChanged
-----------------------
>Sent when the user changes the date in a Date Picker.
NumberPickerValueChanged
-------------------------
>Sent when the user changes the time in a Time Picker.
VideoStateChanged
------------------
>Sent when the user changes the value in a Number Picker.
EditBoxEditingDidBegin
-----------------------
>Sent from the Video View when the state of the video has changed.
EditBoxEditingDidBegin
-----------------------
>Sent from the Edit box when it gains focus(the user selects the widget). The virtual keyboard is shown.
EditBoxEditingDidEnd
---------------------
>Sent from the Edit box when it loses focus. The virtual keyboard is hidden.
EditBoxTextChanged
-------------------
>Sent from the Edit box when the text was changed.
EditBoxReturn
--------------
>Sent from the Edit box when the return button was pressed. On iphone platform the virtual keyboard is not closed after receiving this event. The virtual keyboard can be hided by setting the "showKeyboard" to "false". Not available on Windows Phone 7.1.
WebViewHookInvoked
-------------------
>A web view hook has captured a url.
DialogDismissed
----------------
>Send by a Dialog widget when when a user dismisses a popover by tapping outside of it. It will not be sent if it was dismissed by calling maWidgetModalDialogHide(). Available only on the iPad.
