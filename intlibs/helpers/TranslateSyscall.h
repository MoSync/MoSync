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

//Returns NULL if not found.

#ifdef __cplusplus
extern "C" {
#endif
const char* translateSyscall(int num);

/**
 * Returns a syscall ID from it's name.
 *
 * @param name Name of syscall to look up
 *
 * @return < 0 - Not found
 *         > 0 - The requested syscall ID
 */
int translateSyscallID ( const char *name );

#ifdef __cplusplus
}
#endif
