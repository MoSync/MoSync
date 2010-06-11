#include <wchar.h>
#include <wctype.h>
#include "wcstub.h"


extern int wcscasecmp(const wchar_t *s1, const wchar_t *s2);
extern int wcsncasecmp(const wchar_t *s1, const wchar_t *s2, size_t n);


int wcsicmp(const wchar_t *s1, const wchar_t *s2)
{
//#ifdef __APPLE__
// enable this workaround on all platforms, instead of
// specifying which platforms don't have wcscasecmp
#if 1
     int i;

     for (i = 0; 
	  s1[i] != L'\0' && s2[i] != L'\0'; 
	  i++) {
	  wint_t x = towlower(s1[i]);
	  wint_t y = towlower(s2[i]);	  
	  if (x != y)
	       return x - y;
     }
     return towlower(s1[i]) - towlower(s2[i]);	  
#else
	return(wcscasecmp(s1, s2));
#endif
}


int wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
//#ifdef __APPLE__
#if 1
     int i;

     for (i = 0; 
	  i < n && s1[i] != L'\0' && s2[i] != L'\0'; 
	  i++) {
	  wint_t x = towlower(s1[i]);
	  wint_t y = towlower(s2[i]);	  
	  if (x != y)
	       return x - y;
     }
     
     return  (i >= n) ? 0 : towlower(s1[i]) - towlower(s2[i]);	  
#else
     return(wcsncasecmp(s1, s2, n));
#endif
}
