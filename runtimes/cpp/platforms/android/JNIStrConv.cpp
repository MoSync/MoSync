#include "JNIStrConv.h"

#ifdef MOSYNC_NATIVE
#include <jni.h>

static int wctomb_m(char* dst, wchar_t src) {
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

static size_t wcstombs_m(char* dst, const wchar_t* src, size_t count) {
	size_t dpos = 0;
	const wchar_t* srcp = src;
	if(dst == NULL) {
		char mb[MB_CUR_MAX];
		int res;
		do {
			res = wctomb_m(mb, *srcp);
			if(res < 0)
				return res;
			dpos += res;
			srcp++;
		} while(res > 0);
		dpos++;	// terminating NULL.
	} else {
		// write directly to dst while enough space remains.
		while(dpos + MB_CUR_MAX < count) {
			int res = wctomb_m(dst + dpos, *srcp);
			if(res < 0)
				return res;
			if(res == 0)
				return dpos;
			srcp++;
			dpos += res;
		}
		// slower method when very little space remains.
		while(dpos < count) {
			char buf[MB_CUR_MAX];
			int bytes = wctomb_m(buf, *srcp);
			int remain, num_to_copy;
			if(bytes < 0)
				return bytes;
			remain = count - dpos;
			num_to_copy = (remain > bytes ? bytes : remain);
			for(int i=0; i<num_to_copy; ++i)
				dst[dpos++] = buf[i];

			if (*srcp == 0)
				return dpos;
			++srcp;
		}
	}
	return dpos;
}

static size_t wcstoutf_m(char* dst, const wchar_t* src) {
	/*const UTF32** srcPtr;
	srcPtr[0] = src;
	size_t strLen = wcslen(src);
	const UTF32* end = ((UTF32*)src) + strLen;
	UTF8** dstPtr;
	dstPtr[0] = dst;
	ConversionResult result = ConvertUTF32toUTF8(srcPtr, end, dstPtr, dstPtr[strLen * sizeof(wchar_t)], lenientConversion);
	if (result != conversionOK) {
		dst[0] = 0;
		return 0;
	}
	return ((char*)*dstPtr) - dst;*/
	size_t len = wcstombs_m(dst, src, sizeof(wchar_t) * wcslen(src));
	dst[len] = '\0';
}

jstring mawstojs(JNIEnv* env, const wchar_t* src) {
	size_t len = wcslen(src);
	char* dst = (char*) calloc(len + 1, sizeof(wchar_t));
	wcstoutf_m(dst, src);
	// UTF-8 encoding.
	jstring result = env->NewStringUTF(dst);
	free(dst);
	return result;
}
#else
int wcslen_m(const wchar* maStr) {
	wchar *p;
	p = (wchar*) maStr;
	while (*p) p++;
	return p - maStr;
}
#endif
