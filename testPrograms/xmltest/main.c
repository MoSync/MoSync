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

#include "xmltest.h"

int MAMain() {
  int res, size;
  //char xmlBuffer[XML_BUFFER_SIZE];
  //gXMLFile = xmlBuffer;
  maSetColor(COLOR_BLACK);

  /*//test atoi
  int i = atoi("1234567890");
  i = atoi("3124567890"); //should overflow
  i = atoi("2147483647");
  i = atoi("-2147483648");
  i = atoi("2147483648"); //should overflow
  i = atoi("-2147483649");  //should overflow
  //i = atoi("9223372036854775807");  //should overflow*/

  println("HTTP GET...");
  maSocketOpenHttpGet("link.astando.se/guido/LvRouting.asmx/GenerateRouteGuido?fromX=100502&fromY=77476&toX=100591&toY=77368", &size);
  maWait(WAIT_SOCKET);
  res = maSocketState();
  if(res <= 0) {
    printiln("SOHG err %i", res);
  } else {
    printiln("ConLen %i", size);
    res = runXML(res);
    printiln("res %i", res);
  }

//end:
  println("PAK to exit");
  PAKAndRelease();
  maExit(0);
  return 0;
}

void GD_strncpy_utf8_to_latin1(char* dst, const char* src) {
  static const char* suffixes[] = {
    "torget", "gata", "gatan", "vägen", "brinken", "allén", "backen", "leden", "motet",
    "gränden", "gränd", "parken", "lunden", "platsen"
  };
  static const int nSuffixes = sizeof(suffixes) / sizeof(const char*);
  int dp=0, i, j, nBytes;
  for(j=0; src[j]; j += nBytes) {
    for(i=0; i<nSuffixes; i++) {
      if(StringEqual_utf8_latin1(src+j, suffixes[i])) {
	dst[dp++] = '-';
	break;
      }
    }
    dst[dp++] = ConvertUtf8ToLatin1(src+j, &nBytes);
  }
  dst[dp] = 0;
}

XRES runXML(Handle hSock) {
  GD_EDGES e;
  int i;
  TX(readXML(&e, hSock));

  //calculate and print interesting stuff
  printiln("%i edges", e.nEdges);
  if(!e.nEdges) {
    println("No edges! Bailing out...");
    return 0;
  }
  PressAndRelease(MAK_FIRE);
  //int nReversed=0;
  //int nTotalPoints=0;
  //double xMax=DBL_MIN, xMin=DBL_MAX, yMax=DBL_MIN, yMin=DBL_MAX;
  for(i=0; i<e.nEdges; i++) {
    const char* name = e.edges[i].name;
    char buffer[SPEECH_BUFFERSIZE + 32];
    GD_strncpy_utf8_to_latin1(buffer, name);
    println(buffer);
    //StartSpeaking(buffer);
    PressAndRelease(MAK_FIRE);
    /*if(edge->isReversed)
      nReversed++;
    nTotalPoints += edge->nPoints;
    for(int j=0; j<edge->nPoints; j++) {
      GD_POINT* point = &edge->points[j];
      if(dgt(point->x, xMax))
	xMax = point->x;
      if(dlt(point->x, xMin))
	xMin = point->x;
      if(dgt(point->y, yMax))
	yMax = point->y;
      if(dlt(point->y, yMin))
	yMin = point->y;
      //printiln("x %i", d2i(point->x));
      //printiln("y %i", d2i(point->y));
    }*/
  }
  //printiln("%i reversed", nReversed);
  /*printiln("%i points", nTotalPoints);
  //DUMPDOUBLE(xMax);
  DUMPDOUBLE(xMin);
  //DUMPDOUBLE(yMax);
  DUMPDOUBLE(yMin);

  //calc scale (same for both x and y) (assumes square pixels)
  double height = dsub(yMax,yMin), width = dsub(xMax,xMin);
  DUMPDOUBLE(width);
  DUMPDOUBLE(height);
  double xScale = ddiv(i2d(SCREEN_WIDTH), width);
  double yScale = ddiv(i2d(SCREEN_HEIGHT), height);
  double scale = DMIN(xScale, yScale);
  DUMPDOUBLE(xScale);
  DUMPDOUBLE(yScale);
  //DUMPDOUBLE(scale);

  //calc offset
  int offsetX, offsetY;
  offsetX = -d2i(dmul(xMin, scale));
  offsetY = -d2i(dmul(yMin, scale));

  printiln("oX %i", offsetX);
  printiln("oY %i", offsetY);

  int x = d2i(dmul(e.edges[0].points[0].x, scale)) + offsetX;
  int y = d2i(dmul(e.edges[0].points[0].y, scale)) + offsetY;
  DUMPINT(x);
  DUMPINT(y);

  println("PAK to start");
  PAKAndRelease();

  //draw stuff
  ClearScreen();
  //SetColor(COLOR_WHITE);
  for(int i=2; i<e.nEdges; i++) {
    static const int NCOLORS = 256, NCYCLES = 4, MAXCOLOR = NCOLORS-1;
    int time = ((i-1) * (NCOLORS * NCYCLES)) / (e.nEdges - 2);
    int cycle = (time / NCOLORS) % NCYCLES, subtime = time % NCOLORS;
    switch(cycle) {
      case 0:	SetColor(RGB(0, MAXCOLOR, subtime));		break;
      case 1:	SetColor(RGB(0, MAXCOLOR-subtime, MAXCOLOR));	break;
      case 2:	SetColor(RGB(subtime, 0, MAXCOLOR));		break;
      case 3:	SetColor(RGB(MAXCOLOR, 0, MAXCOLOR-subtime));	break;
    }
    GD_EDGE* edge = &e.edges[i];
    for(int j=0; j<edge->nPoints-1; j++) {
      GD_POINT* point = &edge->points[j];
      Line(d2i(dmul(point->x, scale)) + offsetX, d2i(dmul(point->y, scale)) + offsetY,
	d2i(dmul(point[1].x, scale)) + offsetX, d2i(dmul(point[1].y, scale)) + offsetY);
    }
  }
  if(e.nEdges >= 2) {
    SetColor(RGB(0,255,0)); //green
    GD_EDGE* edge = &e.edges[1];
    for(int j=0; j<edge->nPoints-1; j++) {
      GD_POINT* point = &edge->points[j];
      Line(d2i(dmul(point->x, scale)) + offsetX, d2i(dmul(point->y, scale)) + offsetY,
	d2i(dmul(point[1].x, scale)) + offsetX, d2i(dmul(point[1].y, scale)) + offsetY);
    }
  }
  if(e.nEdges >= 1) {
    SetColor(COLOR_WHITE);
    GD_EDGE* edge = &e.edges[0];
    for(int j=0; j<edge->nPoints-1; j++) {
      GD_POINT* point = &edge->points[j];
      Line(d2i(dmul(point->x, scale)) + offsetX, d2i(dmul(point->y, scale)) + offsetY,
	d2i(dmul(point[1].x, scale)) + offsetX, d2i(dmul(point[1].y, scale)) + offsetY);
    }
  }*/
  /*for(int i=0; i<e.nEdges; i++) {
    GD_EDGE* edge = &e.edges[i];

    for(int j=0; j<edge->nPoints-1; j++) {
      GD_POINT* point = &edge->points[j];
      Line(d2i(dmul(point->x, scale)) + offsetX, d2i(dmul(point->y, scale)) + offsetY,
	d2i(dmul(point[1].x, scale)) + offsetX, d2i(dmul(point[1].y, scale)) + offsetY);
    }
  }*/

  /*UpdateScreen();
  while(1) Wait(WAIT_KEY);*/
  //PAKAndRelease();
  //Exit(0);

  return XR_OK;
}

//Gui-Do XML
XRES GDXGetBool(bool* b) {
  char buffer[6];
  int size;
  //TX(XMLGetTagContent(&ptr));
  TX(XMLReadTagContent(buffer, sizeof(buffer), &size));
  if(StringMatch(buffer, "True")) {
    *b = true;
    return XR_OK;
  } else if(StringMatch(buffer, "False")) {
    *b = false;
    return XR_OK;
  } else {
    return XR_PARSE_ERROR;
  }
}

/*XRES GDXGetPoints(GD_POINT* p, int* nP) {
  const char* ptr;
  TX(XMLGetTagContent(&ptr));
  for(*nP = 0; *nP < MAX_GDPOINTS; (*nP)++) {
    while(iswhite(*ptr)) ptr++;	//skip whitespace
    double d = strtod(ptr, &ptr);
    if(deq(d, 0.0))
      return XR_OK;
    p[*nP].x = d;
    while(iswhite(*ptr)) ptr++;	//skip whitespace
    d = strtod(ptr, &ptr);
    if(deq(d, 0.0)) {
      TXFAIL(XR_PARSE_ERROR);
    }
    p[*nP].y = d;
  }
  while(iswhite(*ptr)) ptr++;	//skip whitespace
  double d = strtod(ptr, &ptr);
  if(!deq(d, 0.0)) {
    printiln("%i points", *nP);
    printfln("%f", d);
    TXFAIL(XR_BUFFER_OVERFLOW);
  } else
    return XR_OK;
}*/

XRES readXML(GD_EDGES* aEdges, Handle hSock) {
  XRES res;
  aEdges->nEdges = 0;
  //println((char*)gXMLFile);
  TX(XMLInit(hSock));
  TX(XMLStartTag("Route"));

  TX(XMLStartTag("Edges"));
  while(!(res = XMLStartTag("Edge"))) {
    GD_EDGE* pEdge;
    if(aEdges->nEdges >= MAX_GDEDGES) {
      TXFAIL(XR_BUFFER_OVERFLOW);
    }
    pEdge = &aEdges->edges[aEdges->nEdges];
    /*{
      TX(XMLStartTag("EdgeID"));
      const char* ptr;
      TX(XMLGetTagContent(&ptr));
      int edgeID = atoi(ptr);
      printiln("id %i", edgeID);
      TX(XMLEndTag());
    }*/
    /*{
      TX(XMLStartTag("IsReversed"));
      TX(GDXGetBool(&pEdge->isReversed));
      //printiln("isr %i", pEdge->isReversed);
      TX(XMLEndTag());
    }*/
    /*{
      TX(XMLStartTag("Points"));
      TX(GDXGetPoints(pEdge->points, &pEdge->nPoints));
      //printiln("%i points", pEdge->nPoints);
      TX(XMLEndTag());
    }*/
    /*{
      TX(XMLStartTag("Length"));
      const char* ptr;
      TX(XMLGetTagContent(&ptr));
      double length = strtod(ptr, &ptr);
      printfln("len %f", length);
      TX(XMLEndTag());
    }*/
    {
      int size;
      TX(XMLStartTag("Name"));
      TX(XMLReadTagContent(pEdge->name, sizeof(pEdge->name), &size));
      printsln("TC %s", pEdge->name);
      TX(XMLEndTag());
    }
    TX(XMLEndTag());
    aEdges->nEdges++;
  }
  if(XISERROR(res)) {
    TX(res);
  } else if(res != XR_TAG_NOT_FOUND) {
    TX(XMLEndTag());
  }

  TX(XMLEndTag());
  TX(XMLFinish());
  return XR_OK;
}

#include "../shared/misc.c"
#include "../shared/print.c"
#include "xmlparser.c"
