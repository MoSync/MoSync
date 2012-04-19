#ifndef _EVENT_H_
#define _EVENT_H_

class EventHandler {
private:
	
public:
	static bool left_pressed, right_pressed, up_pressed, down_pressed, fire_pressed, pound_pressed, star_pressed;
	static bool left, right, up, down, fire, pound, star;
	static bool quit;
	static bool updated;
	static bool sleft, sright;
	
	static void updateEvents();
};


#endif // _EVENT_H_
