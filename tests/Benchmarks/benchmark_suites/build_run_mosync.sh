#compile and run mosync benchmarks
#build the benchmark database lib. Needs to be done for the mosync version so that it can retrieve
#its git_hash and version from disk
cd ../database_libs/mosync/
./getbuildinfo.sh
./workfile.rb
./workfile.rb CONFIG=
cd ../

cd ../opengl/mosync/
rm -R Output/
rm -R ReleasePackages/
rm -R build/
./workfile_android.rb PACK="Android/Android 2.x" CONFIG=
adb -d uninstall com.mosync.app_GLBench
adb -d install build/pipe_release/Android/Android\ 2.x/GLBench.apk
adb shell am start -n com.mosync.app_GLBench/.MoSync
./workfile_ios.rb PACK="Apple/iPhone" CONFIG=
cd build/pipe_release/Apple/iPhone/xcode-proj/
xcodebuild PROVISIONING_PROFILE=CD4311DF-2E7C-43F8-919D-6BE9C7BC7A33 CODE_SIGN_IDENTITY="iPhone Developer: Tomas Uppgard"
cd ../../../../../../../benchmark_suites/
./fruitstrap -d -b ../opengl/mosync/build/pipe_release/Apple/iPhone/xcode-proj/build/Release-iphoneos/GLBench.app -t 80
adb shell am force-stop com.mosync.app_GLBench

cd ../membench/mosync/
rm -R Output/
rm -R ReleasePackages/
rm -R build/
./workfile_android.rb PACK="Android/Android 2.x" CONFIG=
adb -d uninstall com.mosync.app_MemBench
adb -d install build/pipe_release/Android/Android\ 2.x/MemBench.apk
adb shell am start -n com.mosync.app_MemBench/.MoSync
./workfile_ios.rb PACK="Apple/iPhone" CONFIG=
cd build/pipe_release/Apple/iPhone/xcode-proj/
xcodebuild PROVISIONING_PROFILE=CD4311DF-2E7C-43F8-919D-6BE9C7BC7A33 CODE_SIGN_IDENTITY="iPhone Developer: Tomas Uppgard"
cd ../../../../../../../benchmark_suites/
./fruitstrap -d -b ../membench/mosync/build/pipe_release/Apple/iPhone/xcode-proj/build/Release-iphoneos/MemBench.app -t 40
adb shell am force-stop com.mosync.app_MemBench

cd ../linpack/mosync/
rm -R Output/
rm -R ReleasePackages/
rm -R build/
./workfile_android.rb PACK="Android/Android 2.x" CONFIG=
adb -d uninstall com.mosync.app_linpack
adb -d install build/pipe_release/Android/Android\ 2.x/linpack.apk
adb shell am start -n com.mosync.app_linpack/.MoSync
./workfile_ios.rb PACK="Apple/iPhone" CONFIG=
cd build/pipe_release/Apple/iPhone/xcode-proj/
xcodebuild PROVISIONING_PROFILE=CD4311DF-2E7C-43F8-919D-6BE9C7BC7A33 CODE_SIGN_IDENTITY="iPhone Developer: Tomas Uppgard"
cd ../../../../../../../benchmark_suites/
./fruitstrap -d -b ../linpack/mosync/build/pipe_release/Apple/iPhone/xcode-proj/build/Release-iphoneos/linpack.app -t 40
adb shell am force-stop com.mosync.app_linpack