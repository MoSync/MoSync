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

#ifndef SYSCALL_HELPERS_H
#define SYSCALL_HELPERS_H

#define SYSCALL(a) final a

#define maIOCtl_case(func) maIOCtl_##func##_case(func)
#define GVMR(name, type) name
#define GVS(name) mCore._SYSCALL_CONVERT_MAString(name)
#define GVWS(name) mCore._SYSCALL_CONVERT_MAWString(name)

#define NUMBER_KEYS(m) m(0) m(1) m(2) m(3) m(4)	m(5) m(6) m(7) m(8) m(9)
#define OTHER_KEYS(m) m(STAR) m(POUND)
#define ACTION_KEYS(m) m(LEFT) m(RIGHT) m(UP) m(DOWN) m(FIRE)

#define CUSTOM_KEYS(m)\
	m(SOFTLEFT, 6) m(SOFTRIGHT, 7) m(CLEAR, 8)	/*Nokia, SE & most Samsungs*/\
	m(SOFTLEFT, -6) m(SOFTRIGHT, -7) m(CLEAR, -8)	/*Samsung SGH-D6xx series*/\
	m(SOFTLEFT, 1) m(SOFTRIGHT, 4)	/*Siemens*/\
	m(SOFTLEFT, 21) m(SOFTRIGHT, 22)	/*Motorola*/\
	m(SOFTLEFT, -21) m(SOFTRIGHT, -22)	/*Motorola*/\

#define CUSTOM_KEYS_2(m)\
	m(BACK, -11) m(MENU, -20)	/*SE*/\

#define MAX_SOUND_CHANNELS 32

#define DYNARES_BIT 0x40000000

#endif	//SYSCALL_HELPERS_H
