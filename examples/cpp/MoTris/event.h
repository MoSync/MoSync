/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _EVENT_H_
#define _EVENT_H_

class EventHandler {
private:

public:
	static bool left_pressed, right_pressed, up_pressed, down_pressed, fire_pressed, pound_pressed, star_pressed;
	static bool left, right, up, down, fire, pound, star;
	static bool lsk_pressed, rsk_pressed;
	static bool lsk, rsk;
	static bool pointer_pressed, pointer_released;
	static MAPoint2d point;
	static bool quit;
	static bool updated;
	static bool focus;

	static void updateEvents();
};


#endif // _EVENT_H_
