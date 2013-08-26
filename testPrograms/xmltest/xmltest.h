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
