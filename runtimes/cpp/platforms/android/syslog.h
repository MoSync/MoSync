/* Copyright (C) 2013 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef SYSLOG_H_
#define SYSLOG_H_

#ifdef __ANDROID_TRACE__
#include <jni.h>
#include <android/log.h>
#define SYSLOG(...) __android_log_print(ANDROID_LOG_INFO, "MoSync Syscall", __VA_ARGS__)
#else
#define SYSLOG(...)
#endif

#endif /* SYSLOG_H_ */
