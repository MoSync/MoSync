Native UI
=========

> Provides the ability to create Native User Interface through HTML and JavaScript.

Widget Creation
---------------
There are two different ways of creating Native UI widgets. The first approach is to write HTML markups with custom attributes. The second way is to create the UI elements using JavaScript. Here we describe the usage of HTML markups in creating Native UI. Please see mosync.nativeui.create for details on creating widgets in JavaScript
here is an example of a code that creates a Native User Interface on the device:

		<div id="NativeUI">
			<!-- the element with id="mainScreen" is loaded to the device screen  by default -->
			<div widgetType="TabScreen" id="mainScreen">
				<div widgetType="Screen" id="firstScreen" title="Web Screen" icon_iOS="img/TabIconWebView.png" >
					<div widgetType="WebView" width="100%" height="100%" url="http://www.google.com"></div>
				</div>
				<div widgetType="Screen" id="SecondScreen" title="Widget Screen">
					<div widgetType="VerticalLayout" id="mainLayout" width="-1" height="-1">
						<div widgetType="Label" id="myLabel" width="100%" text="Here is a Label" fontSize="19"></div>
						<div widgetType="Button" id="myButton" width="100%" text="Click !" onevent="alert('cliekct')"></div>
					</div>
				</div>
			</div>
		</div>

All of the Native UI widgets should be wrapped inside a div tag with id "NativeUI". The system picks everything inside that tag and creates Native widgets for each corresponding tag. You can specify the type of the Native widget with the attribute "widgetType" and also use other attributes to set the properties of the widgets. For more information on the available widget types, properties,  and valid values please see Widget Types, Widget Events, and Widget Properties.

**Note:** Using HTML markup on Windows Phone 7 is not recommended due to performance issues. 
Icons
-----
To use icons for your tabviews you can use special attributes to assign icons to the tabs. Valid attribute types for the icons are <strong>"icon_android"</strong>, <strong>"icon_iOS"</strong>, and <strong>"icon"</strong>.

Events
------
Currently we only support binding click events through HTML markup. In order to bind a function to the click event of your widget use the "onevent" attribute.

Accessing Widgets From JavaScript
--------------------------------
To access the widgets created in HTML markup you can use document.getNativeElementById to get a object of Type mosync.nativeui.NativeWidgetElement. Here is an example of accessing one of the widgets defined above.

		//Get an instacne of the button created in the markup
		var myButton = document.getNativeElementById("myButton");
		//add an event listener to it
		myButton.addEventListener("Clicked", changeLabel);

Links
-----
 * Widget Types
 * Widget Events
 * Widget Properties
 * Resource

JavaScript Methods
------------------
