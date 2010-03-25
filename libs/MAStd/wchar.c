#include "wchar.h"

// TODO: shorten these function by using loops.
int mbtowc(wchar_t* dst, const char* src, size_t count) {
	if(count < 1)
		return -1;
	if((src[0] & 0x80) == 0) {	// 1 byte
		*dst = *src;
		return *dst == 0 ? 0 : 1;
	} else if((src[0] & 0xE0) == 0xC0) {	// 2 bytes
		if(count < 2)
			return -1;
		if((src[1] & 0xC0) != 0x80)
			return -1;
		*dst = ((((wchar_t)src[0]) & 0x1F) << 6) | (((wchar_t)src[1]) & 0x3F);
		return *dst == 0 ? 0 : 2;
	} else if((src[0] & 0xF0) == 0xE0) {	// 3 bytes
		if(count < 3)
			return -1;
		if((src[1] & 0xC0) != 0x80)
			return -1;
		if((src[2] & 0xC0) != 0x80)
			return -1;
		*dst = ((((wchar_t)src[0]) & 0x0F) << 12) | ((((wchar_t)src[1]) & 0x1F) << 6) |
			(((wchar_t)src[2]) & 0x3F);
		return *dst == 0 ? 0 : 3;

		// if we use UCS-2, we can't deal with Unicode characters outside the Basic Multilingual Plane.
#if MB_CUR_MAX > 3
	} else if((src[0] & 0xF8) == 0xF0) {	// 4 bytes
		if(count < 4)
			return -1;
		if((src[1] & 0xC0) != 0x80)
			return -1;
		if((src[2] & 0xC0) != 0x80)
			return -1;
		if((src[3] & 0xC0) != 0x80)
			return -1;
		*dst = ((((wchar_t)src[0]) & 0x03) << 18) | ((((wchar_t)src[1]) & 0x0F) << 12) |
			((((wchar_t)src[2]) & 0x1F) << 6) | (((wchar_t)src[3]) & 0x3F);
		return *dst == 0 ? 0 : 4;
#endif	//MB_CUR_MAX
	}
	return -1;
}

int wctomb(char* dst, wchar_t src) {
	if(src <= 0x7F) {
		*dst = (char)src;
		return src == 0 ? 0 : 1;
	} else if(src <= 0x7FF) {
		dst[1] = 0x80 | (src & 0x3F);
		dst[0] = 0xC0 | ((src & 0x7E0) >> 6);
		return 2;
	} else {
#if MB_CUR_MAX > 3
		if(src <= 0xFFFF)
#endif
		{
			dst[2] = 0x80 | (src & 0x3F);
			dst[1] = 0x80 | ((src & 0x0FE0) >> 6);
			dst[0] = 0xE0 | ((src & 0xF000) >> 12);
			return 3;
		}
#if MB_CUR_MAX > 3
		else if(src <= 0x10FFFF) {
			dst[3] = 0x80 | (src & 0x3F);
			dst[2] = 0x80 | ((src & 0x0FE0) >> 6);
			dst[1] = 0x80 | ((src & 0x03F000) >> 12);
			dst[0] = 0xF0 | ((src & 0x3C0000) >> 18);
			return 4;
		}
#endif
	}
#if MB_CUR_MAX > 3
	return -1;
#endif
}

size_t mbstowcs(wchar_t* dst, const char* src, size_t count) {
	size_t wpos = 0;
	const char* srcp = src;
	if(dst == NULL) {
		wchar_t wc;
		int res;
		do {
			res = mbtowc(&wc, srcp, 4);
			if(res < 0)
				return res;
			wpos++;
			srcp += res;
		} while(res > 0);
	} else while(wpos < count) {
		int res = mbtowc(dst + wpos, srcp, 4);
		if(res < 0)
			return res;
		if(res == 0) {
			break;
		}
		wpos++;
		srcp += res;
	}
	return wpos;
}

size_t wcstombs(char* dst, const wchar_t* src, size_t count) {
	size_t dpos = 0;
	const wchar_t* srcp = src;
	if(dst == NULL) {
		char mb[MB_CUR_MAX];
		int res;
		do {
			res = wctomb(mb, *srcp);
			if(res < 0)
				return res;
			dpos += res;
			srcp++;
		} while(res > 0);
		dpos++;	// terminating NUL.
	} else while(dpos + MB_CUR_MAX < count) {
		int res = wctomb(dst + dpos, *srcp);
		if(res < 0)
			return res;
		if(res == 0)
			break;
		srcp++;
		dpos += res;
	}
	return dpos;
}
