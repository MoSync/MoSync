//
// xtoa.c
//
// Convert integers to ASCII strings
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 

#include "maxtoa.h"

void xtoa(unsigned long val, char *buf, unsigned radix, int negative)
{
  char *p;
  char *firstdig;
  char temp;
  unsigned digval;

  p = buf;

  if (negative) 
  {
    // Negative, so output '-' and negate
    *p++ = '-';
    val = (unsigned long)(-(long) val);
  }

  // Save pointer to first digit
  firstdig = p;

  do 
  {
    digval = (unsigned) (val % radix);
    val /= radix;

    // Convert to ascii and store
    if (digval > 9)
      *p++ = (char) (digval - 10 + 'a');
    else
      *p++ = (char) (digval + '0');
  } while (val > 0);

  // We now have the digit of the number in the buffer, but in reverse
  // order.  Thus we reverse them now.

  *p-- = '\0';
  do 
  {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;
    p--;
    firstdig++;
  } while (firstdig < p);
}

#ifndef _MSC_VER
char *itoa(int val, char *buf, int radix)
{
  if (radix == 10 && val < 0)
    xtoa((unsigned long) val, buf, radix, 1);
  else
    xtoa((unsigned long)(unsigned int) val, buf, radix, 0);

  return buf;
}

char *ltoa(long val, char *buf, int radix)
{
  xtoa((unsigned long) val, buf, radix, (radix == 10 && val < 0));
  return buf;
}

char *ultoa(unsigned long val, char *buf, int radix)
{
  xtoa(val, buf, radix, 0);
  return buf;
}
#endif	//_MSC_VER
