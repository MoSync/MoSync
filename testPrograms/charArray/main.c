#include <ma.h>

int
MAMain (void)
{
  const char cnv[] ="%l[abc]";
  const wchar_t str[] = L"abbcXab";

  return (char*)str - cnv;
}
