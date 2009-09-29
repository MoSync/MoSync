#!/bin/sh
#
echo Copying config*.h.example files...

cp -f tools/idl/config.h.example tools/idl/config.h
cp -f intlibs/bluetooth/config_bluetooth.h.example intlibs/bluetooth/config_bluetooth.h
cp -f runtimes/cpp/platforms/sdl/config_platform.h.example runtimes/cpp/platforms/sdl/config_platform.h

echo Done.
