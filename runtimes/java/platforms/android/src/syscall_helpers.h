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

#define SYSCALL(a) final a

#define DEVICE_KEYS(m) m(0) m(1) m(2) m(3) m(4)	m(5) m(6) m(7) m(8) m(9) \
				       m(STAR) m(POUND) m(CLEAR)
#define DEVICE_DPAD_KEYS(m) m(LEFT) m(RIGHT) m(UP) m(DOWN)
#define DEVICE_SPECIAL(m) m(DPAD_CENTER, FIRE) m(SOFT_LEFT, SOFTLEFT) m(SOFT_RIGHT, SOFTRIGHT) m(MENU, SOFTLEFT) m(BACK, SOFTRIGHT)

#define MAX_SOUND_CHANNELS 32

#define DYNARES_BIT 0x40000000

