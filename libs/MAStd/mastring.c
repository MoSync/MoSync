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
#include "mastring.h"
#include "madmath.h"
#include "maheap.h"

BOOL StringMatch(const char* a, const char* b) {
	while(*a && *b) {
		if(*a++ != *b++)
			return FALSE;
	}
	return TRUE;
}
BOOL StringMatch1(const char* a, const char* b) {
	while(*a) {
		if(*a++ != *b++)
			return FALSE;
	}
	return TRUE;
}

BOOL memeq(const void* a, const void* b, int size) {
	int pos = 0;

#if 0	//optimization
	while((((((int)a) + pos) & 3) != 0) && pos < size) {
		if(((char*)a)[pos] != ((char*)b)[pos])
			return FALSE;
		pos++;
	}
	while(pos <= (size - 4)) {
		if(((int*)a)[pos] != ((int*)b)[pos])
			return FALSE;
		pos += 4;
	}
#endif

	//simple part
	while(pos < size) {
		if(((char*)a)[pos] != ((char*)b)[pos])
			return FALSE;
		pos++;
	}
	return TRUE;
}

//Returns the number of bytes written to dst (1 or 2).
int ConvertLatin1ToUtf8(char* dst, int src) {
	if(src & 0x80) {
		*dst++ = 0xC0 | ((src >> 6) & 0x03);
		*dst = 0x80 | (src & ~0xC0);
		return 2;
	} else {
		*dst = src;
		return 1;
	}
}



#ifdef MAPIP

#ifndef NO_BUILTINS

char *strncpy(char *dest, const char *source, size_t count)
{
	char *start = dest;

	while (count && (*dest++ = *source++)) count--;
	if (count) while (--count) *dest++ = '\0';
	return start;
}
#endif


//#ifndef NO_BUILTINS

int strncmp(const char *s1, const char *s2, size_t count)
{
	if (!count) return 0;

	while (--count && *s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	return *(unsigned char *) s1 - *(unsigned char *) s2;
}

//#endif

#ifndef NO_BUILTINS

char *strncat(char *s1, const char *s2, size_t count)
{
	char *start = s1;

	while (*s1++);
	s1--;

	while (count--)
	{
		if (!(*s1++ = *s2++)) return start;
	}

	*s1 = '\0';
	return start;
}

#endif	//NO_BUILTINS

int memcmp(const void *dst, const void *src, size_t n)
{
	if (!n) return 0;

	while (--n && *(char *) dst == *(char *) src)
	{
		dst = (char *) dst + 1;
		src = (char *) src + 1;
	}

	return *((unsigned char *) dst) - *((unsigned char *) src);
}

int stricmp(const char *s1, const char *s2)
{
	char f, l;

	do
	{
		f = ((*s1 <= 'Z') && (*s1 >= 'A')) ? *s1 + 'a' - 'A' : *s1;
		l = ((*s2 <= 'Z') && (*s2 >= 'A')) ? *s2 + 'a' - 'A' : *s2;
		s1++;
		s2++;
	} while ((f) && (f == l));

	return (int) (f - l);
}

int strnicmp(const char *s1, const char *s2, size_t count)
{
	int f, l;

	do
	{
		if (((f = (unsigned char)(*(s1++))) >= 'A') && (f <= 'Z')) f -= 'A' - 'a';
		if (((l = (unsigned char)(*(s2++))) >= 'A') && (l <= 'Z')) l -= 'A' - 'a';
	} while (--count && f && (f == l));

	return f - l;
}

char *strchr(const char *s, int ch)
{
	while (*s && *s != (char) ch) s++;
	if (*s == (char) ch) return (char *) s;
	return NULL;
}

char *strrchr(const char *s, int ch)
{
	char *start = (char *) s;

	while (*s++);
	while (--s != start && *s != (char) ch);
	if (*s == (char) ch) return (char *) s;

	return NULL;
}

char *strstr(const char *str1, const char *str2)
{
	char *cp = (char *) str1;
	char *s1, *s2;

	if (!*str2) return (char *) str1;

	while (*cp)
	{
		s1 = cp;
		s2 = (char *) str2;

		while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
		if (!*s2) return cp;
		cp++;
	}

	return NULL;
}

size_t strspn(const char *string, const char *control)
{
	const unsigned char *str = (const unsigned char *)string;
	const unsigned char *ctrl = (const unsigned char *)control;

	unsigned char map[32];
	int count;

	// Clear out bit map
	for (count = 0; count < 32; count++) map[count] = 0;

	// Set bits in control map
	while (*ctrl)
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
		ctrl++;
	}

	// 1st char NOT in control map stops search
	if (*str)
	{
		count = 0;
		while (map[*str >> 3] & (1 << (*str & 7)))
		{
			count++;
			str++;
		}

		return count;
	}

	return 0;
}

size_t strcspn(const char *string, const char *control)
{
	const unsigned char *str = (const unsigned char *)string;
	const unsigned char *ctrl = (const unsigned char *)control;

	unsigned char map[32];
	int count;

	// Clear out bit map
	for (count = 0; count < 32; count++) map[count] = 0;

	// Set bits in control map
	while (*ctrl)
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
		ctrl++;
	}

	// 1st char in control map stops search
	count = 0;
	map[0] |= 1;
	while (!(map[*str >> 3] & (1 << (*str & 7))))
	{
		count++;
		str++;
	}
	return count;
}

char *strpbrk(const char *string, const char *control)
{
	const unsigned char *str = (const unsigned char *)string;
	const unsigned char *ctrl = (const unsigned char *)control;

	unsigned char map[32];
	int count;

	// Clear out bit map
	for (count = 0; count < 32; count++) map[count] = 0;

	// Set bits in control map
	while (*ctrl)
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
		ctrl++;
	}

	// 1st char in control map stops search
	while (*str)
	{
		if (map[*str >> 3] & (1 << (*str & 7))) return (char *) str;
		str++;
	}

	return NULL;
}

void *memmove(void *dst, const void *src, size_t count)
{
	void * ret = dst;

	if (dst <= src || (char *) dst >= ((char *) src + count))
	{
		//
		// Non-Overlapping Buffers
		// use memcpy
		//
		return memcpy(dst, src, count);
	}
	else
	{
		//
		// Overlapping Buffers
		// copy from higher addresses to lower addresses
		//
		dst = (char *) dst + count - 1;
		src = (char *) src + count - 1;

		while (count--)
		{
			*(char *) dst = *(char *) src;
			dst = (char *) dst - 1;
			src = (char *) src - 1;
		}
	}

	return ret;
}

void *memchr(const void *buf, int ch, size_t count)
{
	while (count && (*(unsigned char *) buf != (unsigned char) ch))
	{
		buf = (unsigned char *) buf + 1;
		count--;
	}

	return (count ? (void *) buf : NULL);
}

void *memrchr(const void *buf, int ch, size_t count)
{
	char* ptr = (char*)buf + (count - 1);
	while(*ptr != ch && count != 0) {
		ptr--;
		count--;
	}
	return (count == 0 ? ptr : NULL);
}


char *strdup(const char *s)
{
	char *t;
	int len;

	if (!s) return NULL;
	len = strlen(s);
	t = (char *) malloc(len + 1);
	memcpy(t, s, len + 1);
	return t;
}


char *strlwr(char *s)
{
	char *p = s;

	while (*p)
	{
		*p = (char) tolower(*p);
		p++;
	}

	return s;
}

char *strupr(char *s)
{
	char *p = s;

	while (*p)
	{
		*p = (char) toupper(*p);
		p++;
	}

	return s;
}


char *strnset(char *s, int c, size_t count)
{
	char *start = s;
	while (count-- && *s) *s++ = (char) c;
	return start;
}

char *strrev(char *s)
{
	char *start = s;
	char *left = s;
	char ch;

	while (*s++);
	s -= 2;

	while (left < s)
	{
		ch = *left;
		*left++ = *s;
		*s-- = ch;
	}

	return start;
}


char *strtok_r(char *string, const char *control, char **lasts)
{
	unsigned char *str;
	const unsigned char *ctrl = control;

	unsigned char map[32];
	int count;

	// Clear control map
	for (count = 0; count < 32; count++) map[count] = 0;

	// Set bits in delimiter table
	do { map[*ctrl >> 3] |= (1 << (*ctrl & 7)); } while (*ctrl++);

	// Initialize str. If string is NULL, set str to the saved
	// pointer (i.e., continue breaking tokens out of the string
	// from the last strtok call)
	if (string)
		str = string;
	else
		str = *lasts;

	// Find beginning of token (skip over leading delimiters). Note that
	// there is no token iff this loop sets str to point to the terminal
	// null (*str == '\0')

	while ((map[*str >> 3] & (1 << (*str & 7))) && *str) str++;

	string = str;

	// Find the end of the token. If it is not the end of the string,
	// put a null there
	for ( ; *str ; str++)
	{
		if (map[*str >> 3] & (1 << (*str & 7)))
		{
			*str++ = '\0';
			break;
		}
	}

	// Update nexttoken
	*lasts = str;

	// Determine if a token has been found.
	if (string == (char*)str)
		return NULL;
	else
		return string;
}


char *strtok(char *string, const char *control)
{
	static char* dummy;
	return strtok_r(string, control, &dummy);
}


#ifdef MAPIP

void *memset(void *p, int c, size_t n)
{
	char *pb = (char *) p;
	char *pbend = pb + n;
	while (pb != pbend) *pb++ = c;
	return p;
}

#endif

/*
void *memcpy(void *dst, const void *src, size_t n)
{
void *ret = dst;

while (n--)
{
*(char *)dst = *(char *)src;
dst = (char *) dst + 1;
src = (char *) src + 1;
}

return ret;
}
*/
void *memccpy(void *dst, const void *src, int c, size_t count)
{
	while (count && (*((char *) (dst = (char *) dst + 1) - 1) =
		*((char *)(src = (char *) src + 1) - 1)) != (char) c)
		count--;

	return count ? dst : NULL;
}

int memicmp(const void *buf1, const void *buf2, size_t count)
{
	int f = 0, l = 0;
	const unsigned char *dst = (const unsigned char *)buf1, *src = (const unsigned char *)buf2;

	while (count-- && f == l)
	{
		f = tolower(*dst++);
		l = tolower(*src++);
	}

	return f - l;
}
/*
char *strcpy(char *dst, const char *src)
{
char *cp = dst;
while (*cp++ = *src++);
return dst;
}
*/
size_t strlen(const char *s)
{
	const char *eos = s;
	while (*eos++);
	return (int) (eos - s - 1);
}

int strcmp(const char *s1, const char *s2)
{
	int ret = 0;
	while (!(ret = *(unsigned char *) s1 - *(unsigned char *) s2) && *s2) ++s1, ++s2;

	if (ret < 0)
		ret = -1;
	else if (ret > 0)
		ret = 1 ;

	return ret;
}

char *strcat(char *dst, const char *src)
{
	char *cp = dst;
	while (*cp) cp++;
	while ((*cp++ = *src++));
	return dst;
}

char *strset(char *s, int c)
{
	char *start = s;
	while (*s) *s++ = (char) c;
	return start;
}

size_t strnlen(const char *s, size_t count)
{
	const char *sc;
	for (sc = s; *sc != '\0' && count--; ++sc);
	return sc - s;
}


#endif		/* _WIN32 */

static const unsigned int gPowersOfTen[] = { 10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };

#if 0//def MAPIP
//****************************************
// ASCII to double
//****************************************
//FIXME: replace with OpenBSD's version, which should be more complete and bug-free.

double strtod(const char* string, char** endptr)
{
	double d = 0;
	int i;
	int neg = 0;
	int nDigits = 0;
	int arr[10];
	int nDecimals = 0;

	if (*string == '-')
	{
		string++;
		neg = 1;
	}

	while (isdigit((unsigned char) *string))
	{
		if (nDigits >= 10)
			ErrorExit("strtod overflow!", -4);

		arr[nDigits++] = *string - '0';
		string++;
	}

	if (nDigits)
	{
		nDigits--;

		for (i=0; i<nDigits; i++)
		{
			int index = nDigits-i-1;
			d += arr[i] * gPowersOfTen[index];
		}

		d += arr[nDigits];
	}

	if (*string == '.')
	{
		string++;

		while( isdigit((unsigned char) *string))
		{
			if (nDecimals >= 10)
				maPanic(-5, "strtod overflow!");

			d += ((double) (*string - '0')) / gPowersOfTen[nDecimals];
			nDecimals++;
			string++;
		}
	}

	if(endptr != NULL)
		*endptr = (char*)string;

	if(neg)
		return -d;

	return d;
}
#endif	//MAPIP

int i2a(int a, char* dst) {
	char* orig = dst;
	unsigned int v, index=0;
	if(a < 0) {
		*(dst++) = '-';
		a = -a;
	}
	v = a;
	while(v >= gPowersOfTen[index] && index < (sizeof(gPowersOfTen) / sizeof(int)))
		index++;
	while(index > 0) {
		index--;
		*(dst++) = (v / gPowersOfTen[index]) + '0';
		v %= gPowersOfTen[index];
	}
	*(dst++) = v + '0';
	*(dst++) = 0;
	return dst - orig - 1;
}

int d2a(double d, char* dst) {
	char* orig = dst;
	double integer;
	double frac = modf(d, &integer);
	int ifrac = (int)(frac * 1000000.0); //frac < 1, so ifrac < 1000000.
	int i;
	dst += i2a((int)integer, dst);
	if(!ifrac)
		goto end;
	*dst++ = '.';
	if(ifrac >= 1000000) {
		static const char errormsg[] = "error";
		strcpy(dst, errormsg);
		goto end;
	}
	//dst += i2a(ifrac, dst);	//error on .0*x (ex: .0023 will be rendered as .23)
	//rounding error on *.*999 (ex: .6319999999~ is rendered as .631999 instead of .632)
	for(i=4; i>=0 && ifrac > 0; i--) {
		const int pot = gPowersOfTen[i];
		if(ifrac > pot) {
			*dst++ = (ifrac / pot) + '0';
			ifrac %= pot;
		} else if(ifrac == pot) {
			*dst++ = '1';
			goto end;
		} else {  //ifrac < pot
			*dst++ = '0';
		}
	}
	if(ifrac) {
		*dst++ = ifrac + '0';
	}
end:
	*(dst++) = 0;
	return dst - orig - 1;
}

//****************************************
//		UTF8 functions
//****************************************
/*
BOOL StringEqual_utf8_latin1(const char* utf8, const char* latin1);
char ConvertUtf8ToLatin1(const char* utf8, int* pnBytes);

BOOL StringEqual_utf8_latin1(const char* utf8, const char* latin1)
{
	int nBytes;

	while(*latin1)
	{
		if(ConvertUtf8ToLatin1(utf8, &nBytes) != *latin1++)
			return FALSE;

		utf8 += nBytes;
	}
	return TRUE;
}

char ConvertUtf8ToLatin1(const char* utf8, int* pnBytes)
{
	int nBytes = 0, unicode, i;
	char b = utf8[0];

	if (b & 0x80)
	{
		do
		{
			nBytes++;
			b <<= 1;
		}
		while(b & 0x80);

		if (nBytes > 4)
			ErrorExit("CU8b: %i", nBytes);

		unicode = (b >> nBytes) << ((nBytes-1) * 6);

		for(i=0; i<nBytes-1; i++)
		{
			unicode |= (utf8[i+1] & 0x3F) << ((nBytes-2-i) * 6);
		}

		if (unicode > 0xFF)
			ErrorExit("CU8u: %i", unicode);

		*pnBytes = nBytes;
		return (char)unicode;
	}

	*pnBytes = 1;
	return b;
}
*/
