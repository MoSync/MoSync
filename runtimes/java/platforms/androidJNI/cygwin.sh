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

#!/bin/bash

export ANDROID_PATH=`cygpath -u $1`
export MOSYNC_SRC=`cygpath -u $3` 
export MOSYNC_CPP=`cygpath -u $3"/runtimes/cpp"`
export MOSYNC_CPP_SRC=`cygpath -u $3"/runtimes/cpp/platforms/android"`
export MOSYNC_JAVA_SRC=`cygpath -u $3"/runtimes/java/platforms/androidJNI"`

echo "Build library!"

cd $MOSYNC_JAVA_SRC/AndroidProject

$ANDROID_PATH/ndk-build -B

echo "Finished!"
