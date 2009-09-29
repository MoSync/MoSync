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


//errors are negative. mere problems are positive. success is zero.
typedef enum _XRESULT {
  XR_OK=0, XR_PARSE_ERROR=-1, XR_TAG_NOT_FOUND=1, XR_BUFFER_OVERFLOW=-2,
  XR_COMM_ERROR=-3, XR_INTERNAL_ERROR=-4
} XRES;

#define XISERROR(a) ((a) < 0)
#define XISPROBLEM(a) ((a) > 0)
#define XISOK(a) (!(a))

#define PRINTLINE printiln("line %i", __LINE__)
#define TESTXML(a) { XRES temp = (a); if(XISPROBLEM(temp)) { return temp; }\
  else if(XISERROR(temp)) { PRINTLINE; return temp; } }
#define TX TESTXML
#define TXFAIL(a) { PRINTLINE; return (a); }

XRES XMLInit(Handle httpSocket);

#define MAXLEVEL 8
#define MAXTAGSIZE 32 //including terminator

//will skip all other tags.
//may return XR_TAG_NOT_FOUND if a previously started tag ends. if so, don't call EndTag() for it.
XRES XMLStartTag(const char* expectedTag); 

XRES XMLEndTag();  //every successfully started tag must be ended

/*//*pPtr is not null-terminated, but '<'-terminated.
//uses the most recently started tag.
XRES XMLGetTagContent(const char** pPtr);

//pEnd is a pointer to the first char following the content, a '<'.
int XMLGetTagContentEnd(const char** pPtr, const char** pEnd);*/

//bufsize includes the terminating zero.
//size is strlen(buffer) on success, not modified on failure.
XRES XMLReadTagContent(char* buffer, int bufsize, int* size);

XRES XMLFinish();  //will return error if there are open tags
