/*
 * Copyright (C) 1993 DJ Delorie
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms is permitted
 * provided that the above copyright notice and following paragraph are
 * duplicated in all such forms.
 *
 * This file is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

//
// access
//


#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

static int access(const char *fn, int flags)
{
  struct stat s;
  if (stat(fn, &s))
    return -1;

  if (s.st_mode & S_IFDIR)
    return 0;

  if (flags & W_OK)
  {
    if (s.st_mode & S_IWRITE)
      return 0;

    return -1;
  }

  return 0;
}

#include <ma.h>
#include <conprint.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <maassert.h>

const char *dbname = "/test.db";
int MAMain(void) GCCATTRIB(noreturn);
int MAMain(void) {
	InitConsole ();
	puts("before access");
	if (access(dbname,0) == 0)
	{
		puts("Files exists");
		FREEZE;
	} else {
		puts("File does not exist");
		FREEZE;
	}
}
