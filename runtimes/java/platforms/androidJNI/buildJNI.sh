#!/bin/bash

MOSYNC_SRC="/cygdrive/d/code/work/MoSync/trunk"
ANDROID_PATH="/cygdrive/d/android-ndk-1.5_r1/sources/mosync"

echo "Removing old files"

cd $ANDROID_PATH/..
rm -rf mosync
mkdir mosync

cd ../apps
rm -rf mosync
mkdir mosync
cp $MOSYNC_SRC"/runtimes/cpp/platforms/android/Application.mk" mosync/Application.mk

mkdir $ANDROID_PATH"/base"

echo "Copying files"

cd $MOSYNC_SRC"/runtimes/cpp/core"
cp -R * $ANDROID_PATH


cd $MOSYNC_SRC"/runtimes/cpp/base"
cp -R * $ANDROID_PATH"/base"
cp -R * $ANDROID_PATH


cd $MOSYNC_SRC"/runtimes/cpp/platforms/android"
cp -R * $ANDROID_PATH"/base"
cp -R * $ANDROID_PATH


cd $MOSYNC_SRC"/intlibs"
cp -R * $ANDROID_PATH


cd $MOSYNC_SRC"/runtimes/java/platforms/androidJNI/src"
cp config_platform.h $ANDROID_PATH
cp config_platform.h $ANDROID_PATH"/base"


cd $ANDROID_PATH
cd ../..

echo "Build library"

make APP="mosync"
