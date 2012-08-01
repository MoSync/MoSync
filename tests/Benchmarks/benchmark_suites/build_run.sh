#!/bin/sh
#build and deploy all benchmark apps
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
