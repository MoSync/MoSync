#include <wchar.h>
#include "wcstub.h"


extern int wcscasecmp(const wchar_t *s1, const wchar_t *s2);
extern int wcsncasecmp(const wchar_t *s1, const wchar_t *s2, size_t n);


int wcsicmp(const wchar_t *s1, const wchar_t *s2)
{
	return(wcscasecmp(s1, s2));
}


int wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
	return(wcsncasecmp(s1, s2, n));
}
