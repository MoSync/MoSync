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

include $(CLEAR_VARS)

LOCAL_PATH:= $(MOSYNC_CPP)

LOCAL_MODULE    := mosync
LOCAL_ARM_MODE	:= arm
LOCAL_SRC_FILES := core/Core.cpp platforms/android/FileImpl.cpp base/FileStream.cpp base/MemStream.cpp base/Stream.cpp base/Image.cpp ../../intlibs/hashmap/hashmap.cpp platforms/android/AndroidHelpers.cpp base/Syscall.cpp platforms/android/SyscallImpl.cpp platforms/android/IOCtl.cpp core/disassembler.cpp core/Recompiler/ArmAssembler.cpp core/Recompiler/ArmRecompiler.cpp platforms/android/MoSyncBridge.cpp ../../intlibs/helpers/platforms/linux/log.cpp
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

LOCAL_CFLAGS := -DFPM_ARM 

include $(BUILD_SHARED_LIBRARY)
