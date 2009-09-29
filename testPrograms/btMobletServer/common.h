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

#ifndef COMMON_H
#define COMMON_H

//#define SERVER_UUID "102030405060708090A0B0C0D0E0F010"
#define SERVER_UUID_1(m) m(10203040)
#define SERVER_UUID_2(m) m(50607080)
#define SERVER_UUID_3(m) m(90A0B0C0)
#define SERVER_UUID_4(m) m(D0E0F010)

#define S(s) #s
#define P(s) 0x##s

#define SERVER_UUID_STRING SERVER_UUID_1(S) SERVER_UUID_2(S) SERVER_UUID_3(S) SERVER_UUID_4(S)

#define SERVER_MAUUID_DECL {{ SERVER_UUID_1(P), SERVER_UUID_2(P), SERVER_UUID_3(P), SERVER_UUID_4(P) }}

#endif	//COMMON_H
