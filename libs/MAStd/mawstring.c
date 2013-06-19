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

#ifndef __IOS__
#include "ma.h"
#include "mawstring.h"
#include "madmath.h"
#else
#include <ma.h>
#include <mawstring.h>
#include <madmath.h>
#endif

#if defined(MAPIP) || (defined(MOSYNC_NATIVE) && defined(__ANDROID__))

#ifndef NO_BUILTINS

wchar *wcsncpy(wchar *dest, const wchar *source, size_t count)
{
	wchar *start = dest;

	while (count && (*dest++ = *source++)) count--;
	if (count) while (--count) *dest++ = '\0';
	return start;
}
#endif


//#ifndef NO_BUILTINS

int wcsncmp(const wchar *s1, const wchar *s2, size_t count)
{
	if (!count) return 0;

	while (--count && *s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	return *(wchar *) s1 - *(wchar *) s2;
}

//#endif

#ifndef NO_BUILTINS

wchar *wcsncat(wchar *s1, const wchar *s2, size_t count)
{
	wchar *start = s1;

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

int wcsicmp(const wchar *s1, const wchar *s2)
{
	wchar f, l;

	do 
	{
		f = ((*s1 <= 'Z') && (*s1 >= 'A')) ? *s1 + 'a' - 'A' : *s1;
		l = ((*s2 <= 'Z') && (*s2 >= 'A')) ? *s2 + 'a' - 'A' : *s2;
		s1++;
		s2++;
	} while ((f) && (f == l));

	return (int) (f - l);
}

int wcsnicmp(const wchar *s1, const wchar *s2, size_t count)
{
	int f, l;

	do 
	{
		if (((f = (wchar)(*(s1++))) >= 'A') && (f <= 'Z')) f -= 'A' - 'a';
		if (((l = (wchar)(*(s2++))) >= 'A') && (l <= 'Z')) l -= 'A' - 'a';
	} while (--count && f && (f == l));

	return f - l;
}

#if defined(MOSYNC_NATIVE) && defined(__ANDROID__)
wchar *wcschr(const wchar *s, wchar ch)
#else
wchar *wcschr(const wchar *s, int ch)
#endif
{
	while (*s && *s != (wchar) ch) s++;
	if (*s == (wchar) ch) return (wchar *) s;
	return NULL;
}

#if defined(MOSYNC_NATIVE) && defined(__ANDROID__)
wchar *wcsrchr(const wchar *s, wchar ch)
#else
wchar *wcsrchr(const wchar *s, int ch)
#endif
{
	wchar *start = (wchar *) s;

	while (*s++);
	while (--s != start && *s != (wchar) ch);
	if (*s == (wchar) ch) return (wchar *) s;

	return NULL;
}

wchar *wcsstr(const wchar *str1, const wchar *str2)
{
	wchar *cp = (wchar *) str1;
	wchar *s1, *s2;

	if (!*str2) return (wchar *) str1;

	while (*cp)
	{
		s1 = cp;
		s2 = (wchar *) str2;

		while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
		if (!*s2) return cp;
		cp++;
	}

	return NULL;
}

size_t wcsspn(const wchar *string, const wchar *control)
{
	const wchar *str = (const wchar *)string;
	const wchar *ctrl = (const wchar *)control;

	wchar map[32];
	int count;

	// Clear out bit map
	for (count = 0; count < 32; count++) map[count] = 0;

	// Set bits in control map
	while (*ctrl)
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
		ctrl++;
	}

	// 1st wchar NOT in control map stops search
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

size_t wcscspn(const wchar *string, const wchar *control)
{
	const wchar *str = (const wchar *)string;
	const wchar *ctrl = (const wchar *)control;

	wchar map[32];
	int count;

	// Clear out bit map
	for (count = 0; count < 32; count++) map[count] = 0;

	// Set bits in control map
	while (*ctrl)
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
		ctrl++;
	}

	// 1st wchar in control map stops search
	count = 0;
	map[0] |= 1;
	while (!(map[*str >> 3] & (1 << (*str & 7))))
	{
		count++;
		str++;
	}
	return count;
}

wchar *wcspbrk(const wchar *string, const wchar *control)
{
	const wchar *str = (const wchar *)string;
	const wchar *ctrl = (const wchar *)control;

	wchar map[32];
	int count;

	// Clear out bit map
	for (count = 0; count < 32; count++) map[count] = 0;

	// Set bits in control map
	while (*ctrl)
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
		ctrl++;
	}

	// 1st wchar in control map stops search
	while (*str)
	{
		if (map[*str >> 3] & (1 << (*str & 7))) return (wchar *) str;
		str++;
	}

	return NULL;
}

#if defined(MOSYNC_NATIVE) && defined(__ANDROID__)
void *wmemchr(const void *buf, wchar ch, size_t count)
#else
void *wmemchr(const void *buf, int ch, size_t count)
#endif
{
	while (count && (*(wchar *) buf != (wchar) ch)) 
	{
		buf = (wchar *) buf + 1;
		count--;
	}

	return (count ? (void *) buf : NULL);
}

void *wmemrchr(const void *buf, int ch, size_t count)
{
	wchar* ptr = (wchar*)buf + (count - 1);
	while(*ptr != ch && count != 0) {
		ptr--;
		count--;
	}
	return (count == 0 ? ptr : NULL);
}

wchar *wcslwr(wchar *s)
{
	wchar *p = s;

	while (*p)
	{
		*p = (wchar) tolower(*p);
		p++;
	}

	return s;
}

wchar *wcsupr(wchar *s)
{
	wchar *p = s;

	while (*p)
	{
		*p = (wchar) toupper(*p);
		p++;
	}

	return s;
}


wchar *wcsnset(wchar *s, int c, size_t count)
{
	wchar *start = s;
	while (count-- && *s) *s++ = (wchar) c;
	return start;
}

wchar *wcsrev(wchar *s)
{
	wchar *start = s;
	wchar *left = s;
	wchar ch;

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

size_t wcslen(const wchar *s)
{
	const wchar *eos = s;
	while (*eos++);
	return (int) (eos - s - 1);
}

int wcscmp(const wchar *s1, const wchar *s2)
{
	int ret = 0;
	while (!(ret = *(wchar *) s1 - *(wchar *) s2) && *s2) ++s1, ++s2;

	if (ret < 0)
		ret = -1;
	else if (ret > 0)
		ret = 1 ;

	return ret;
}

wchar *wcscat(wchar *dst, const wchar *src)
{
	wchar *cp = dst;
	while (*cp) cp++;
	while ((*cp++ = *src++));
	return dst;
}

wchar *wcsset(wchar *s, int c)
{
	wchar *start = s;
	while (*s) *s++ = (wchar) c;
	return start;
}

size_t wcsnlen(const wchar *s, size_t count)
{
	const wchar *sc;
	for (sc = s; *sc != '\0' && count--; ++sc);
	return sc - s;
}

#endif	/* MAPIP */
