/*
 * Copyright 2007 Doxological Ltd.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <faxpp/transcode.h>

const char *decode_to_string(FAXPP_DecodeFunction t)
{
  if(t == FAXPP_utf8_decode)
    return "UTF-8";
  else if(t == FAXPP_iso_8859_1_decode)
    return "ISO-8859-1";
  else if (t == FAXPP_utf16_le_decode)
    return "UTF-16LE";
  else if (t == FAXPP_utf16_be_decode)
    return "UTF-16BE";
  else if (t == FAXPP_utf16_native_decode)
    return "UTF-16 (native)";
  else if (t == FAXPP_ucs4_le_decode)
    return "ISO-10646-UCS-4 (LE)";
  else if (t == FAXPP_ucs4_be_decode)
    return "ISO-10646-UCS-4 (BE)";
  else if (t == FAXPP_ucs4_native_decode)
    return "ISO-10646-UCS-4 (native)";
  return "Unknown";
}

const char *encode_to_string(FAXPP_EncodeFunction t)
{
  if(t == FAXPP_utf8_encode)
    return "UTF-8";
  else if (t == FAXPP_utf16_native_encode)
    return "UTF-16";
  return "Unknown";
}

// 9 is the code for an illegal first byte
uint8_t FAXPP_utf_8_bytes[256] =
{
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6
};

/*
 * Code range    | UTF-8
 * --------------+-------------------------------------
 * 000000-00007F | 0xxxxxxx
 * 000080-0007FF | 110xxxxx 10xxxxxx
 * 000800-00FFFF | 1110xxxx 10xxxxxx 10xxxxxx
 * 010000-10FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 */
unsigned int
FAXPP_utf8_decode(const void *buffer, const void *buffer_end,
                  Char32 *ch)
{
  uint8_t *buf = (uint8_t*)buffer;
  if(*buf < 0x80) {
    /* 0aaa aaaa
       0aaa aaaa */
    *ch = *buf;
    return 1;
  }

  unsigned int len = FAXPP_utf_8_bytes[*buf];

  Char32 c = 0;
  switch(len) {
  case 1:
    // Handled above
    break;
  case 2:
    if(buf + len > (uint8_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;

    /* 110a aaaa 10bb bbbb
       0000 0aaa aabb bbbb */
    c  = (*buf++ & 0x1F) << 6;

    if((*buf & 0xC0) != 0x80)
      return TRANSCODE_BAD_ENCODING;
    c |= (*buf   & 0x3F);
    break;
  case 3:
    if(buf + len > (uint8_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;

    /* 1110 aaaa 10bb bbbb 10cc cccc
       0000 0000 aaaa bbbb bbcc cccc */
    c  = (*buf++ & 0x0F) << 12;

    if((*buf & 0xC0) != 0x80)
      return TRANSCODE_BAD_ENCODING;
    c |= (*buf++ & 0x3F) << 6;

    if((*buf & 0xC0) != 0x80)
      return TRANSCODE_BAD_ENCODING;
    c |= (*buf   & 0x3F);
    break;
  case 4:
    if(buf + len > (uint8_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;

    /* 1111 0aaa 10bb bbbb 10cc cccc 10dd dddd
       0000 0000 000a aabb bbbb cccc ccdd dddd */
    c  = (*buf++ & 0x07) << 18;

    if((*buf & 0xC0) != 0x80)
      return TRANSCODE_BAD_ENCODING;
    c |= (*buf++ & 0x3F) << 12;

    if((*buf & 0xC0) != 0x80)
      return TRANSCODE_BAD_ENCODING;
    c |= (*buf++ & 0x3F) << 6;

    if((*buf & 0xC0) != 0x80)
      return TRANSCODE_BAD_ENCODING;
    c |= (*buf   & 0x3F);
    break;
  case 9: // This is our code for a bad first byte
  default:
    return TRANSCODE_BAD_ENCODING;
  }

  *ch = c;
  return len;
}

// ISO-8859-1 is the first 256 codepoints from Unicode
unsigned int
FAXPP_iso_8859_1_decode(const void *buffer, const void *buffer_end,
                        Char32 *ch)
{
  *ch = *(uint8_t*)buffer;
  return 1;
}

/*
 * Code range    | UTF-16
 * --------------+-------------------------------------
 * 000000-00FFFF | xxxxxxxx xxxxxxxx
 * 010000-10FFFF | 110110xx xxxxxxxx 110111xx xxxxxxxx
 */
unsigned int
FAXPP_utf16_be_decode(const void *buffer, const void *buffer_end,
                      Char32 *ch)
{
  uint8_t *buf = (uint8_t*)buffer;
  unsigned int len;

  if(*buf >= 0xD8 && *buf <= 0xDF) len = 4;
  else len = 2;

  if(buf + len > (uint8_t*)buffer_end)
    return TRANSCODE_PREMATURE_END_OF_BUFFER;

  Char32 c = 0;
  switch(len) {
  case 2:
    /* aaaa aaaa bbbb bbbb
       aaaa aaaa bbbb bbbb */
    c = *buf++ << 8;
    c |= *buf;
    break;
  case 4:
    /* 1101 10aa bbbb bbbb 1101 11cc dddd dddd
       0000 0000 0001 aabb bbbb bbcc dddd dddd */
    c = 1 << 20;
    if((*buf & 0xFC) != 0xD8) return TRANSCODE_BAD_ENCODING;
    c |= (*buf++ & 0x03) << 18;
    c |= *buf++ << 16;

    if((*buf & 0xFC) != 0xDC) return TRANSCODE_BAD_ENCODING;
    c |= (*buf++ & 0x03) << 8;
    c |= *buf;
    break;
  }

  *ch = c;
  return len;
}

unsigned int
FAXPP_utf16_le_decode(const void *buffer, const void *buffer_end,
                      Char32 *ch)
{
  uint8_t *buf = (uint8_t*)buffer;
  unsigned int len;

  if(*(buf + 1) >= 0xD8 && *(buf + 1) <= 0xDF) len = 4;
  else len = 2;

  if(buf + len > (uint8_t*)buffer_end)
    return TRANSCODE_PREMATURE_END_OF_BUFFER;

  Char32 c = 0;
  switch(len) {
  case 2:
    /* bbbb bbbb aaaa aaaa
       aaaa aaaa bbbb bbbb */
    c = *buf++;
    c |= *buf << 8;
    break;
  case 4:
    /* bbbb bbbb 1101 10aa dddd dddd 1101 11cc
       0000 0000 0001 aabb bbbb bbcc dddd dddd */
    c = 1 << 20;
    c |= *buf++ << 16;
    if((*buf & 0xFC) != 0xD8) return TRANSCODE_BAD_ENCODING;
    c |= (*buf++ & 0x03) << 18;

    c |= *buf++;
    if((*buf & 0xFC) != 0xDC) return TRANSCODE_BAD_ENCODING;
    c |= (*buf & 0x03) << 8;
    break;
  }

  *ch = c;
  return len;
}

unsigned int
FAXPP_utf16_native_decode(const void *buffer, const void *buffer_end,
                          Char32 *ch)
{
  uint16_t *buf = (uint16_t*)buffer;
  unsigned int len;

  if(*buf >= 0xD800 && *buf <= 0xDF00) len = 2;
  else len = 1;

  if(buf + len > (uint16_t*)buffer_end)
    return TRANSCODE_PREMATURE_END_OF_BUFFER;

  Char32 c = 0;
  switch(len) {
  case 1:
    /* aaaa aaaa bbbb bbbb
       aaaa aaaa bbbb bbbb */
    c = *buf;
    break;
  case 2:
    /* 1101 10aa bbbb bbbb 1101 11cc dddd dddd
       0000 0000 0001 aabb bbbb bbcc dddd dddd */
    c = 1 << 20;
    if((*buf & 0xFC00) != 0xD800) return TRANSCODE_BAD_ENCODING;
    c |= (*buf++ & 0x03FF) << 10;

    if((*buf & 0xFC00) != 0xDC00) return TRANSCODE_BAD_ENCODING;
    c |= (*buf & 0x03FF);
    break;
  }

  *ch = c;
  return len * sizeof(uint16_t);
}

unsigned int
FAXPP_ucs4_be_decode(const void *buffer, const void *buffer_end,
                     Char32 *ch)
{
  uint8_t *buf = (uint8_t*)buffer;

  if(buf + 4 > (uint8_t*)buffer_end)
    return TRANSCODE_PREMATURE_END_OF_BUFFER;

  Char32 c;

  /* aaaa aaaa bbbb bbbb cccc cccc dddd dddd
     aaaa aaaa bbbb bbbb cccc cccc dddd dddd */
  c  = *buf++ << 24;
  c |= *buf++ << 16;
  c |= *buf++ << 8;
  c |= *buf;

  *ch = c;
  return 4;
}

unsigned int
FAXPP_ucs4_le_decode(const void *buffer, const void *buffer_end,
                     Char32 *ch)
{
  uint8_t *buf = (uint8_t*)buffer;

  if(buf + 4 > (uint8_t*)buffer_end)
    return TRANSCODE_PREMATURE_END_OF_BUFFER;

  Char32 c;

  /* dddd dddd cccc cccc bbbb bbbb aaaa aaaa
     aaaa aaaa bbbb bbbb cccc cccc dddd dddd */
  c  = *buf++;
  c |= *buf++ << 8;
  c |= *buf++ << 16;
  c |= *buf   << 24;

  *ch = c;
  return 4;
}

unsigned int
FAXPP_ucs4_native_decode(const void *buffer, const void *buffer_end,
                         Char32 *ch)
{
  uint32_t *buf = (uint32_t*)buffer;

  if(buf + 1 > (uint32_t*)buffer_end)
    return TRANSCODE_PREMATURE_END_OF_BUFFER;

  /* aaaa aaaa bbbb bbbb cccc cccc dddd dddd
     aaaa aaaa bbbb bbbb cccc cccc dddd dddd */

  *ch = *buf;
  return 1 * sizeof(uint32_t);
}

unsigned int
FAXPP_utf8_encode(void *buffer, void *buffer_end, Char32 ch)
{
  uint8_t *buf = (uint8_t*)buffer;

	if(ch < 0x80) {
    if(buf + 1 > (uint8_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;

    *buf = ch;
		return 1;
  }
	else if(ch < 0x800) {
    if(buf + 2 > (uint8_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;

    /* 110a aaaa 10bb bbbb
       0000 0aaa aabb bbbb */
    *buf++ = (ch >> 6) | 0xC0;
    *buf = (ch & 0x3F) | 0x80;
		return 2;
  }
	else if(ch < 0x10000) {
    if(buf + 3 > (uint8_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;

    /* 1110 aaaa 10bb bbbb 10cc cccc
       0000 0000 aaaa bbbb bbcc cccc */
    *buf++ = (ch >> 12) | 0xE0;
    *buf++ = (ch >> 6) | 0x80;
    *buf = (ch & 0x3F) | 0x80;
		return 3;
  }

  if(buf + 4 > (uint8_t*)buffer_end)
    return TRANSCODE_PREMATURE_END_OF_BUFFER;

  /* 1111 0aaa 10bb bbbb 10cc cccc 10dd dddd
     0000 0000 000a aabb bbbb cccc ccdd dddd */
  *buf++ = (ch >> 18) | 0xF0;
  *buf++ = (ch >> 12) | 0x80;
  *buf++ = (ch >> 6) | 0x80;
  *buf = (ch & 0x3F) | 0x80;
  return 4;
}

unsigned int
FAXPP_utf16_native_encode(void *buffer, void *buffer_end, Char32 ch)
{
  uint16_t *buf = (uint16_t*)buffer;

  if((ch & 0xFFFF0000) == 0) {
    if(buf + sizeof(uint16_t) >= (uint16_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;

    *buf = ch;
    return sizeof(uint16_t);
  }
 else {
    if(buf + 2 * sizeof(uint16_t) >= (uint16_t*)buffer_end)
      return TRANSCODE_PREMATURE_END_OF_BUFFER;
    
    ch -= 0x10000;
    *buf++ = ((ch >> 10) + 0xD800);
    *buf = ((ch & 0x3FF) + 0xDC00);
    return 2 * sizeof(uint16_t);
  }
}

