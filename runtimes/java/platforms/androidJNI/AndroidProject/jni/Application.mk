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

APP_BUILD_SCRIPT	:= $(MOSYNC_CPP_SRC)/Android.mk
APP_PROJECT_PATH	:= $(MOSYNC_JAVA_SRC)/AndroidProject
APP_MODULES			:= mosync

APP_CPPFLAGS		:= -D_android -I$(MOSYNC_CPP)/core -I$(MOSYNC_CPP)/base -I$(MOSYNC_CPP) -I$(MOSYNC_CPP_SRC) -I$(MOSYNC_SRC)/intlibs -I$(MOSYNC_JAVA_SRC)/src
