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

#include "../maapi.h"
#include "../shared/misc.h"
#include "../shared/print.h"
#include "xmlparser.h"

#define SPEECH_BUFFERSIZE 128

//typedef struct _GD_POINT { double x, y; } GD_POINT;
/*typedef struct _XML_POINTER {
  const char* ptr;
  int len;
} XML_POINTER;*/

//#define MAX_GDPOINTS 32  //maybe too small. overflow is a real possibility.
typedef struct _GD_EDGE {
  //int nPoints;
  //bool isReversed;
  //GD_POINT points[MAX_GDPOINTS];
  char name[SPEECH_BUFFERSIZE];
} GD_EDGE;

#define MAX_GDEDGES 32  //maybe too small. overflow is a real possibility.
typedef struct _GD_EDGES {
  int nEdges;
  GD_EDGE edges[MAX_GDEDGES];
} GD_EDGES;

XRES readXML(GD_EDGES* aEdges, Handle hSock);
XRES runXML(Handle hSock);
