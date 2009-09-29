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

Handle gSock;

//#define MAX_UNGET 4
#define BUFFER_SIZE 1024
static char gBuffer[BUFFER_SIZE]; //assumed aligned 4
static int gReadPos=0, gWritePos=0;

static XRES ReadChar(char* c) {
  if(gReadPos == gWritePos) {
    int res;

    //move at least 4 bytes to beginning, set g*Pos accordingly
    if(gWritePos > 8) {
      int m4a3 = ((gWritePos-4) & ~3);
      int a3 = (gWritePos & ~3);
      *(int*)gBuffer = *(int*)(gBuffer+m4a3);
      if(m4a3 != a3) {
	*(int*)(gBuffer+4) = *(int*)(gBuffer+a3);
      }
      gReadPos = gWritePos = 4 + (gWritePos & 3);
    }

    SocketRead(gSock, gBuffer+gWritePos, BUFFER_SIZE - gWritePos);
    Wait(WAIT_SOCKET);
    res = SocketState();
    if(res <= 0) {
      printiln("RCerr %i", res);
      return XR_COMM_ERROR;
    }
    gWritePos += res;
  }
  *c = gBuffer[gReadPos++];
  return XR_OK;
}
static XRES UnGetChar() {
  gReadPos--;
  if(gReadPos < 0) {
    TXFAIL(XR_INTERNAL_ERROR);
  }
  return XR_OK;
}

//typedef const char* TAG;
typedef char TAG[MAXTAGSIZE];
TAG gTagStack[MAXLEVEL];
int gStackPointer;

//on failure, state of parser is undefined and reinitialization is required.
static XRES TokenMatchNoSkip(const char* token) {
  const char* oT = token;
  while(*token) {
    char c;
    TX(ReadChar(&c));
    if(*token++ != c) {
      printsln("TM %s", oT);
      printcln("TMc %c", c);
      TX(XR_PARSE_ERROR);
    }
  }
  return XR_OK;
}
static XRES TokenMatch(const char* token) {
  char c;
  do {
    TX(ReadChar(&c));
  } while(iswhite(c));
  TX(UnGetChar());
  /*//since we read the first non-white char, we have to do the checking of it
  if(*token++ != c) {
    TX(XR_PARSE_ERROR);
  }*/
  return TokenMatchNoSkip(token);
}

static XRES XML_ScanToEndTag(const char* tag, bool allow_other_endtags) {
  //printsln("STE %s", tag);
  if(!*tag) {
    TXFAIL(XR_INTERNAL_ERROR);
  }
  //const char* origPtr = gPtr;
  while(true) {
    char c;
    TX(ReadChar(&c));
    if(c == '<') {
      TX(ReadChar(&c));
      if(c == '/') {
	const char* tt = tag;
	bool match = true;
	TX(ReadChar(&c));
	do {
	  if(c != *tt++) {
	    match = false;
	    //printcln("STEm %c", c);
	    //printcln("STEn %c", *(tt-1));
	    if(allow_other_endtags)
	      break;
	    else {
	      //printcln("%c", *(gPtr-1));
	      //printcln("%c", *(tt-1));
	      TX(XR_PARSE_ERROR);
	    }
	  }
	  TX(ReadChar(&c));
	} while(c != '>' && *tt != '>');
	if(match) {
	  if(c != '>')
	    TX(ReadChar(&c));
	  //printsln("STEe %s", tag);
	  return XR_OK;
	}
      }
    }
    //TX(ReadChar(&c));
  }
}

XRES XMLInit(Handle httpSocket) {
  gSock = httpSocket;
  gStackPointer = MAXLEVEL;

  TX(TokenMatch("<?xml"));
  TX(TokenMatch("version=\"1.0\""));
  TX(TokenMatch("encoding=\"utf-8\"?>"));
  return XR_OK;
}

XRES XMLStartTag(const char* expectedTag) {
  int pos = 0;
  const char* origET = expectedTag;
  char* tag;
  --gStackPointer;
  //printsln("ST %s", origET);
  //printiln("sp %i", gStackPointer);
  if(gStackPointer == 0) {
    TXFAIL(XR_PARSE_ERROR);
  }
  tag = gTagStack[gStackPointer];
  while(true) {
    TX(TokenMatch("<"));
#define READ_TO_TAG if(pos >= MAXTAGSIZE) TXFAIL(XR_PARSE_ERROR); TX(ReadChar(&tag[pos++]))
    READ_TO_TAG;
    //TX(UnGetChar());
    if(tag[pos-1] == '/') {	//we've got an end tag
      //compare it with the top of the stack
      gStackPointer++;
      TX(UnGetChar());
      TX(UnGetChar());
      TX(XMLEndTag());
      return XR_TAG_NOT_FOUND;
    }
    expectedTag = origET;
    while(*expectedTag) {
      /*if(*gPtr != '>' && !iswhite(*gPtr)) {
	PRINTLINE;
	printcln("%c", *gPtr);
	goto skip;
      }*/
      if(*expectedTag++ != tag[pos-1]) {
	/*PRINTLINE;
	println(origET);
	println(tag);
	char buf[3] = {expectedTag[-1], gPtr[-1], 0};
	println(buf);*/
	//we need to read the rest of the tag first
	while(tag[pos-1] != '>') {
	  READ_TO_TAG;
	}
	{ //debug
	  tag[pos] = 0;
	  //printsln("skip %s", tag);
	}
	goto skip;
      }
      READ_TO_TAG;
    }
    if(iswhite(tag[pos-1])) {
      while(tag[pos-1] != '>') {
	READ_TO_TAG;
      }
    } else if(tag[pos-1] != '>') {
      //PRINTLINE;
      //goto skip;
      TXFAIL(XR_PARSE_ERROR);
    } else { //tag[pos-1] == '>'
      //READ_TO_TAG;
    }
    break;
skip:
    TX(XML_ScanToEndTag(tag, true));
    pos = 0;
  }
  //printsln("STe %s", origET);
  //printiln("sp %i", gStackPointer);
  return XR_OK;
}

XRES XMLEndTag() {
  //printiln("ET %i", gStackPointer);
  if(gStackPointer >= MAXLEVEL) {
    TX(XR_PARSE_ERROR);
  }
  TX(XML_ScanToEndTag(gTagStack[gStackPointer++], true));
  //printiln("ETe %i", gStackPointer);
  return XR_OK;
}

/*XRES XMLGetTagContent(const char** pPtr) {
  if(gStackPointer >= MAXLEVEL) {
    TX(XR_PARSE_ERROR);
  }
  *pPtr = gPtr;
  return XR_OK;
}
int XMLGetTagContentEnd(const char** pPtr, const char** pEnd) {
  TX(XMLGetTagContent(pPtr));
  while(*gPtr++ != '<');
  *pEnd = gPtr;
  gPtr--;
  return XR_OK;
}*/
XRES XMLReadTagContent(char* buffer, int bufsize, int* size) {
  int pos = 0;
  //printsln("RTC %s", gTagStack[gStackPointer]);
  while(pos < bufsize) {
    TX(ReadChar(&buffer[pos]));
    if(buffer[pos] == '<') {
      TX(UnGetChar());
      buffer[pos] = 0;
      *size = pos;
      return XR_OK;
    }
    pos++;
  }
  TXFAIL(XR_BUFFER_OVERFLOW);
}

XRES XMLFinish() {
  if(gStackPointer != MAXLEVEL) {
    TX(XR_PARSE_ERROR);
  }
  return XR_OK;
}
