    This app works only on Android and iOS and exemplifies the usage of
maOptionsBox syscall.
      ( *Displays a special kind of dialog that has a list of possible choices.
        * The list of options is displayed as buttons on iOS, and as text views 
        on Android.
        * By clicking any option the dialog gets dismissed and a 
        #EVENT_OPTIONS_BOX_BUTTON_CLICKED event is sent back.
        The dialog contains: a title, a cancel button, a destructive button, and
        a list of options. The destructive button is the first option in the list, 
        it's an iOS specific feature: it has different color than the other options,
        and indicates that it's action has destructive behavior. On Android it 
        is treated and it looks like a normal option.
      )
       
      After the user clicks the 'Show optoins dialog' button, the syscall gets called.
The app is notified upon user selection: in a label below the button. 
App can exit using the back button on Android.
