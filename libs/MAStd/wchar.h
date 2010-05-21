/** \file wchar.h
* \brief Wide character conversions
* \author Fredrik Eldh
*/

#ifndef WCHAR_H
#define WHCAR_H

#include <ma.h>

#ifdef __cplusplus
extern "C" {
#endif

/// The maximum number of bytes in a multibyte character.
#define MB_CUR_MAX 3
#define MB_LEN_MAX 3

#ifdef MAPIP
typedef int wint_t;
#endif

/** \brief Multibyte to wide-char.
*
* Converts a UTF-8 character to the equivalent wide character.
* \param dst A pointer to the destination wchar_t.
* \param src The UTF-8 character.
* \param count The maximum number of bytes to read from \a src.
* \returns The length in bytes of the UTF-8 character, or 0 if it was a NUL character,
* or \< 0 on error.
*/
int mbtowc(wchar_t* dst, const char* src, size_t count);

/** \brief Wide-char to multibyte.
*
* Converts a wide character to the equivalent UTF-8 character.
* \param dst The destination buffer. Must be at least #MB_LEN_MAX bytes long,
* as that is the maximum length of a UTF-8 character.
* \param src The wide character.
* \returns The number of bytes written to \a dst, or 0 if it was a NUL character,
* or \< 0 on error.
*/
int wctomb(char* dst, wchar_t src);

/** \brief Multibyte string to wide-char string.
*
* Converts a NUL-terminated UTF-8 string into the equivalent wide-char string.
*
* \a dst may be NULL. In that case, \a count is ignored,
* no output will actually be written, and the return value will be the number of
* wide characters needed to store the converted string, including the terminating NUL.
*
* \param dst A wide-char buffer, into which the converted string will be written.
* Must be at least \a count characters long.
* \param src The source UTF-8 string.
* \param count The maximum number of characters that may be written to \a dst,
* including the terminating NUL.
* \returns The number of characters converted, excluding the terminating NUL, or \< 0 on error.
* If the return value equals \a count, the output was not NUL-terminated.
*/
size_t mbstowcs(wchar_t* dst, const char* src, size_t count);

/** \brief Wide-char string to multibyte string.
*
* Converts a NUL-terminated wide-char string into the equivalent UTF-8 string.
*
* \a dst may be NULL. In that case, \a count is ignored,
* no output will actually be written, and the return value will be the number of
* bytes needed to store the converted string, including the terminating NUL.
*
* \param dst A buffer, into which the converted string will be written.
* Must be at least \a count bytes long.
* \param src The source wide-char string.
* \param count The maximum number of bytes that may be written to \a dst,
* including the terminating NUL.
* \returns The number of bytes written, excluding the terminating NUL, or \< 0 on error.
* If the return value equals \a count, the output was not NUL-terminated.
*/
size_t wcstombs(char* dst, const wchar_t* src, size_t count);

int mblen(const char* string, size_t size);

#ifdef __cplusplus
}
#endif

#endif	//WHCAR_H
