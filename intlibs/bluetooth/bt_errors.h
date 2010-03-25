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

#define BT_ERRORS(m)\
	m(120001, BTERR_MAX_SDP_LEVEL, "Maximum SDP level reached")\
	m(120002, BTERR_DISCOVERY_IN_PROGRESS, "Discovery already in progress")\
	m(120003, BTERR_NEGATIVE_BUFFER_SIZE, "Negative buffer size")\
	m(120004, BTERR_READ_BUFFER_OVERFLOW, "Read buffer overflow")\
	m(120005, BTERR_NONAME, "No name supplied")

DECLARE_ERROR_ENUM(BT)
