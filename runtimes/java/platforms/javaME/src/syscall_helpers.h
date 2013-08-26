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
