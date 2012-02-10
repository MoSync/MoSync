# Copyright (C) 2010 MoSync AB
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

LOCAL_PATH		:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := mosync
LOCAL_ARM_MODE  := arm
LOCAL_SRC_FILES := FileImpl.cpp \
	../../../../intlibs/helpers/platforms/linux/log.cpp \
	../sdl/FileImpl.cpp \
	MoSyncBridge.cpp \
	AndroidHelpers.cpp \
	SyscallImpl.cpp \
	IOCtl.cpp \
	../../base/FileStream.cpp \
	../../base/MemStream.cpp \
	../../base/Stream.cpp \
	../../base/Image.cpp \
	../../base/Syscall.cpp \
	../../core/Core.cpp \
	../../core/disassembler.cpp \
	../../core/Recompiler/ArmAssembler.cpp \
	../../core/Recompiler/ArmRecompiler.cpp \
	../../../../intlibs/hashmap/hashmap.cpp \
	../../core/Recompiler/ashmem.cpp \


LOCAL_LDLIBS	:= -L$(SYSROOT)/usr/lib -llog -lGLESv1_CM -lGLESv2

LOCAL_CFLAGS	:= -DFPM_ARM

include $(BUILD_SHARED_LIBRARY)
