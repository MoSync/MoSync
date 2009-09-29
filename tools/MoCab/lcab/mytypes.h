/*  Copyright 2001 Rien Croonenborghs, Ben Kibbey, Shaun Jackman, Ivan Brozovic

    This file is part of lcab.
    lcab is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    lcab is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with lcab; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef MYTYPESH
#define MYTYPESH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <err.h>
#include <dirent.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#include "getopt\getopt.h"
#include <stdio.h>
#define snprintf sprintf_s
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif
#endif

#include <time.h>

#include "warn.h"

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long int dword;
typedef unsigned long int CHECKSUM;

#define MAXINPUTFILES		(1024)
#define MAXSIZE			(4096)
#define DATABLOCKSIZE           (32768)

#define STRINGTERM              ('\0')
#define ATTR_ro                 (0x01)		/* read-only attrib */
#define ATTR_h                  (0x02)		/* hidden */
#define ATTR_s                  (0x04)		/* system */
#define ATTR_a                  (0x20)		/* archive */
#define ATTR_e                  (0x40)		/* run after extraction */
#define ATTR_utf                (0x80)		/* UTF names */
#define CH_START                (0x00)		/* header offset */
#define CFO_START               (0x24)		/* folder offset */
#define CFI_START               (0x2C)  	/* file offset */

#define VERSION			"1.0b11"

#endif
