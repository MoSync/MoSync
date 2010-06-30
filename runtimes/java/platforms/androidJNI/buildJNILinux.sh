#!/bin/bash

# These should be defined outside of this file!
export MOSYNC_SRC="D:\Code\work\MoSync\trunk"
export ANDROID_PATH="D:\android-ndk-r4"

export MOSYNC_CPP=$MOSYNC_SRC"/runtimes/cpp"
export MOSYNC_CPP_SRC=$MOSYNC_SRC"/runtimes/cpp/platforms/android"
export MOSYNC_JAVA_SRC=$MOSYNC_SRC"/runtimes/java/platforms/androidJNI"

if [ -d $MOSYNC_JAVA_SRC"/AndroidProject/jni" ]; then
	echo "jni folder exists!"
else
	echo "jni folder doesn't exist, I'll add that for you!"
	mkdir $MOSYNC_JAVA_SRC"/AndroidProject/jni"
fi

cp $MOSYNC_CPP_SRC"/Application.mk" $MOSYNC_JAVA_SRC"/AndroidProject/jni"
cp $MOSYNC_CPP_SRC"/Android.mk" $MOSYNC_JAVA_SRC"/AndroidProject/jni"

cd $MOSYNC_JAVA_SRC"/src"
cp config_platform.h.example $MOSYNC_CPP_SRC"/config_platform.h"
cp config_platform.h.example $MOSYNC_CPP_SRC"/config_platform.h"

echo "Build library!"

cd $MOSYNC_JAVA_SRC/AndroidProject

$ANDROID_PATH/ndk-build

echo "Finished!"
