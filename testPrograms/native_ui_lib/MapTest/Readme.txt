Purpose:
The test program shows the native ui map functionality and provides UI elements for doing
the following:
	- add/remove multiple random map pins
	- set/get visible area
	- set zoom level
	- center the map
	- a map control is visible and available for interraction to the user

IMPORTANT:
	ADD YOU OWN MAP CREDENTIALS INSIDE THE MAP CONSTRUCTOR:
	MainScreen.cpp:
	mMap = new Map("google maps test credentials", "bing maps test credentials");