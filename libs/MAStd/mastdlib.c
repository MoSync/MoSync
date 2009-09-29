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

#include "ma.h"
#include "conprint.h"
#include "mastdlib.h"


//****************************************
// ASCII to int
//****************************************

#ifdef MAPIP

//now with (incomplete) overflow detection

int atoi(const char* string)
{
	int neg = 0;
	int v = 0;
	int i = 0;

	if(*string == '-')
	{
		string++;
		neg = 1;
	}

	while(isdigit(string[i]))
	{
		v = (v * 10) + (string[i++] - '0');

		if(i > 10)
			maPanic(-6, "atoi overflow!");
	}

	if(neg)
		return -v;

	return v;
}

#endif	//MAPIP

int axtoi(const char *hexStg) {
	int n = 0;         // position in string
	int m = 0;         // position in digit[] to shift
	int count;         // loop index
	int intValue = 0;  // integer value of hex string
	int digit[8];

	if(hexStg[0] == '0' && hexStg[1] == 'x') hexStg+=2;

	// hold values to convert
	while (n < 8) {
		if (hexStg[n]=='\0')
			break;
		if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
			digit[n] = hexStg[n] & 0x0f;            //convert to int
		else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
			digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
		else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
			digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
		else break;
		n++;
	}
	count = n;
	m = n - 1;
	n = 0;
	while(n < count) {
		// digit[n] is value of hex digit at position n
		// (m << 2) is the number of positions to shift
		// OR the bits into return value
		intValue = intValue | (digit[n] << (m << 2));
		m--;   // adjust the position to set
		n++;   // next digit to process
	}
	return (intValue);
}

char ConvertUtf8ToLatin1(const char* utf8, int* pnBytes) {
	char b = utf8[0];
	if(b & 0x80) {
		int nBytes = 0, unicode, i;
		do {
			nBytes++;
			b <<= 1;
		} while(b & 0x80);
		if(nBytes > 4) {
			printf("CU8b: %i", nBytes);
			maExit(nBytes);
		}
		unicode = (b >> nBytes) << ((nBytes-1) * 6);
		for(i=0; i<nBytes-1; i++) {
			unicode |= (utf8[i+1] & 0x3F) << ((nBytes-2-i) * 6);
		}
		if(unicode > 0xFF) {
			printf("CU8u: %i", unicode);
			maExit(unicode);
		}
		*pnBytes = nBytes;
		return (char)unicode;
	} else {
		*pnBytes = 1;
		return b;
	}
}

BOOL StringEqual_utf8_latin1(const char* utf8, const char* latin1) {
	while(*latin1) {
		int nBytes;
		if(ConvertUtf8ToLatin1(utf8, &nBytes) != *latin1++)
			return FALSE;
		utf8 += nBytes;
	}
	return TRUE;
}

#if 0
int find(const char* a, char b) {
	int pos = 0;
	while(a[pos]) {
		if(a[pos] == b)
			return pos;
		pos++;
	}
	return -1;
}
#endif

static const unsigned int randInitialY=842502087u, randInitialZ=3579807591u, randInitialW=273326509u;
static unsigned int randStateX=1, randStateY=842502087u, randStateZ=3579807591u, randStateW=273326509u;

void srand(unsigned int seed) {
	randStateX = seed;
	randStateY = randInitialY;
	randStateZ = randInitialZ;
	randStateW = randInitialW;
}

int urand() {
	unsigned int t = (randStateX ^ (randStateX << 11));
	randStateX = randStateY; randStateY = randStateZ; randStateZ = randStateW;
	return randStateW = (randStateW ^ (randStateW >> 19)) ^ (t ^ (t >> 8));
}

int rand() {
	return (int)(RAND_MAX & urand());
}

#ifdef MAPIP
int abs(int x) {
	return x >= 0 ? x : -x;
}

div_t div(int numer, int denom) {
	div_t r;
	r.quot = numer / denom;
	r.rem = numer % denom;
	return r;
}
#endif
