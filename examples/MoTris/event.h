/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef _EVENT_H_
#define _EVENT_H_

class EventHandler {
private:

public:
	static bool left_pressed, right_pressed, up_pressed, down_pressed, fire_pressed, pound_pressed, star_pressed;
	static bool left, right, up, down, fire, pound, star;
	static bool quit;
	static bool updated;

	static void updateEvents();
};


#endif // _EVENT_H_
