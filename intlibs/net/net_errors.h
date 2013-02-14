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

#define NET_ERRORS(m)\
	m(280001, ERR_HTTP_METHOD_INVALID, "HTTP method invalid")\
	m(280002, ERR_HTTP_READONLY_WRITE, "Attempted to write to a read-only HTTP connection")\
	m(280003, ERR_URL_SPACE, "Whitespace is not allowed in URLs")\
	m(280004, ERR_CONN_READFROM, "readFrom not allowed on this connection")\
	m(280005, ERR_CONN_WRITETO, "writeTo not allowed on this connection")\

DECLARE_ERROR_ENUM(NET)
