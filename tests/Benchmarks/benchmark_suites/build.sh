adb uninstall com.mosync.opengl
adb uninstall com.mosync.membench
adb uninstall com.java.linpack
adb uninstall com.mosync.NDKLinpack

#build and run all apps on android
cd ../opengl/android-dalvik/
ant release
ant installr
#adb shell am start -n com.mosync/.GLBenchActivity
cd ../..
#sleep 80

cd membench/android-ndk/
ant release
ant installr
#adb shell am start -n com.mosync/.MemBenchNativeAndroidActivity
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