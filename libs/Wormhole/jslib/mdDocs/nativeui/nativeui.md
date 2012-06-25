Native UI
=========

> Provides the ability to create Native User Interface through HTML and JavaScript.

Widget Creation
---------------
There are two different ways of creating Native UI widgets. The first approach is to write HTML markups with custom attributes. The second way is to create the UI elements using JavaScript. Here we describe the usage of HTML markups in creating Native UI. Please see mosync.nativeui.create for details on creating widgets in JavaScript
here is an example of a code that creates a Native User Interface on the device:

		<div id="NativeUI">
			<!-- the element with id="mainScreen" is loaded to the device screen  by default -->
			<div data-widgetType="TabScreen" id="mainScreen">
				<div data-widgetType="Screen" id="firstScreen" data-title="Web Screen" data-icon_iOS="img/TabIconWebView.png" >
					<div data-widgetType="WebView" data-width="100%" data-height="100%" data-url="http://www.google.com"></div>
				</div>
				<div data-widgetType="Screen" id="SecondScreen" data-title="Widget Screen">
					<div data-widgetType="VerticalLayout" id="mainLayout" data-width="100%" data-height="100%">
						<div data-widgetType="Label" id="myLabel" data-width="100%" data-text="Here is a Label" data-fontSize="19"></div>
						<div data-widgetType="Button" id="myButton" data-width="100%" data-text="Click !" data-onevent="alert('Clicked!')"></div>
					</div>
				</div>
			</div>
		</div>

All of the Native UI widgets should be wrapped inside a div tag with id "NativeUI". The system picks everything inside that tag and creates Native widgets for each corresponding tag. You can specify the type of the Native widget with the attribute "data-widgetType" and also use other attributes to set the properties of the widgets. For more information on the available widget types, properties,  and valid values please see Widget Types, Widget Events, and Widget Properties.

**Note:**: You can skip the prefix "data-" in the attributes, the system supports both versions. When using a property in the JavaScript code, you should only use the properthy name without the "data-" prefix.

**Note:** Using HTML markup on Windows Phone 7 is not recommended due to performance issues. 

Icons
-----
To use icons for your tabviews you can use special attributes to assign icons to the tabs. Valid attribute types for the icons are <strong>"data-icon\_android"</strong>, <strong>"data-icon\_iOS"</strong>, and <strong>"data-icon"</strong>.

Events
------
Currently we only support binding click events through HTML markup. In order to bind a function to the click event of your widget use the "data-onevent" attribute.

Accessing Widgets From JavaScript
--------------------------------
To access the widgets created in HTML markup you can use document.getNativeElementById to get a object of Type mosync.nativeui.NativeWidgetElement. Here is an example of accessing one of the widgets defined above.

		// Get an instacne of the button created in the markup.
		var myButton = document.getNativeElementById("myButton");
		// Add an event listener to it.
		myButton.addEventListener("Clicked", changeLabel);

Links
-----
 * Widget Types
 * Widget Events
 * Widget Properties
 * Resource

JavaScript Methods
------------------
