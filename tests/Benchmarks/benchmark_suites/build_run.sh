#!/bin/sh
#build and deploy all benchmark apps on iOS
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

cd membench/android-ndk/
ant release
ant installr
#adb shell am start -n com.mosync.membench/.MemBenchNativeAndroidActivity
cd ../..
#sleep 40

cd linpack/android-dalvik/
ant release
ant installr
#adb shell am start -n com.java.linpack/.JavaLinpackActivity
cd ../..
#sleep 20

cd linpack/android-ndk/
ant release
ant installr
#adb shell am start -n com.mosync.NDKLinpack/.NDKLinpackActivity
cd ../..

adb shell am start -n com.mosync.opengl/.GLBenchActivity
