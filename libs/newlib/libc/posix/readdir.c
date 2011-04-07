#ifndef HAVE_OPENDIR

/*
* Copyright (c) 1983 Regents of the University of California.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. All advertising materials mentioning features or use of this software
*    must display the following acknowledgement:
*	This product includes software developed by the University of
*	California, Berkeley and its contributors.
* 4. Neither the name of the University nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)readdir.c	5.7 (Berkeley) 6/1/90";
#endif /* LIBC_SCCS and not lint */

#include <dirent.h>
#include <stddef.h>
#include <string.h>

/*
* get next entry in a directory.
*/
dirent* readdir(DIR* dirp) {
	register struct dirent *dp;
	int res;

	if (dirp->dd_fd == -1)
		return NULL;

	res = getdents (dirp->dd_fd, (struct dirent *)dirp->dd_buf, dirp->dd_len);

	if (res <= 0) {
		return NULL;
	}
	dp = (struct dirent *)(dirp->dd_buf);
	return (dp);
}

int readdir_r(DIR* dirp, dirent* entry, dirent ** result) {
	dirent* dp = readdir(dirp);
	if(!dp)
		return -1;
	memcpy(entry, dp, sizeof(dirent));
	*result = entry;
	return 0;
}

#endif /* ! HAVE_OPENDIR */
