#!/bin/sh
#TODO we must build the database libs for all platforms as well!!!
#The iOS native BenchDB.h files should be accessed by its relative paths in the .xcodeproj:s, so every bench
#doesn't need its own in the repo and locally
#build and deploy all benchmark apps on iOS
echo "== starting bench =="
date
cd ../linpack/ios/
xcodebuild OBJROOT=Build/Objs SYMROOT=Build/Products
cd ../../benchmark_suites/
./fruitstrap -b ../linpack/ios/Build/Products/Release-iphoneos/NativeLinpack3.app

cd ../membench/ios/
xcodebuild OBJROOT=Build/Objs SYMROOT=Build/Products
cd ../../benchmark_suites/
./fruitstrap -b ../membench/ios/Build/Products/Release-iphoneos/MemBenchNativeIOS.app

cd ../opengl/ios/
xcodebuild OBJROOT=Build/Objs SYMROOT=Build/Products
cd ../../benchmark_suites/
./fruitstrap -b ../opengl/ios/Build/Products/Release-iphoneos/GLBenchIOSNative.app

cd ios/BenchmarkSuite/
xcodebuild OBJROOT=Build/Objs SYMROOT=Build/Products
cd ../..
./fruitstrap -d -b ios/BenchmarkSuite/Build/Products/Release-iphoneos/BenchmarkSuite.app

#build and run all apps on android
adb uninstall com.mosync.opengl
adb uninstall com.mosync.membench
adb uninstall com.java.linpack
adb uninstall com.mosync.NDKLinpack

cd ../opengl/android-dalvik/
ant release
ant installr
#adb shell am start -n com.mosync.opengl/.GLBenchActivity
cd ../..
#sleep 80
#adb shell am force-stop com.mosync.opengl

cd membench/android-ndk/
ant release
ant installr
#adb shell am start -n com.mosync.membench/.MemBenchNativeAndroidActivity
cd ../..
#sleep 45
#adb shell am force-stop com.mosync.membench

cd linpack/android-dalvik/
ant release
ant installr
#adb shell am start -n com.java.linpack/.JavaLinpackActivity
cd ../..
#sleep 35
#adb shell am force-stop com.java.linpack

cd linpack/android-ndk/
ant release
ant installr
#adb shell am start -n com.mosync.NDKLinpack/.NDKLinpackActivity
cd ../..
#sleep 20
#adb shell am force-stop com.mosync.NDKLinpack
cd opengl/android-dalvik/
adb shell am start -n com.mosync.opengl/.GLBenchActivity
cd ../..

#compile and run mosync benchmarks, we need to wait for the previous benchmarks to finish
cd benchmark_suites/
./build_run_mosync.sh
echo "bench ended"
date
#after ALL benchmarks have finished, use egrep PATTERN to print the output from all the apps to a file, make sure to do adb logcat -c
#at the top of this file to clear the logs on the device. Then check the file to see the status of all the executions
#the same can be done with the iOS apps since they run in gdb