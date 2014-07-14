# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This file defines a few constants that describe the host environment.
# HOST, Symbol. Either :linux or :win32.
# DLL_FILE_ENDING, String. The file ending of DLL files.
# EXE_FILE_ENDING, String. The file ending of executable files.

# On Linux only:
# SDL_SOUND, boolean. True if SDL_Sound is available.
# BLUETOOTH, boolean. True if Bluez is available.

require "#{File.dirname(__FILE__)}/error.rb"

begin
	UNAME = open("|uname").readline().strip()
rescue SystemCallError
	UNAME = "windows32"
end

if(UNAME == "Linux")
	HOST = :linux
elsif(UNAME == "MINGW32_NT-5.1")
	HOST = :win32
elsif(UNAME == "MINGW32_NT-6.0")
	HOST = :win32
elsif(UNAME == "MINGW32_NT-6.1")
	HOST = :win32
elsif(UNAME == "MINGW32_NT-6.2")
	HOST = :win32
elsif(UNAME == "windows32")
	HOST = :win32
elsif(UNAME == "CYGWIN_NT-5.1")
	HOST = :win32
elsif(UNAME == "CYGWIN_NT-6.1-WOW64")
	HOST = :win32
elsif(UNAME == "CYGWIN_NT-6.2-WOW64")
	HOST = :win32
elsif(UNAME == "Darwin")
	HOST = :darwin
else
	error("Unknown platform: #{UNAME}")
end

if(HOST == :linux)

	if ( File.exist?( "/etc/moblin-release" ) )
		HOST_PLATFORM = :moblin
	elsif ( File.exist?( "/etc/lsb-release" ) )
		HOST_PLATFORM = :ubuntu
	elsif ( File.exist?( "/etc/SUSE-release" ) )
		HOST_PLATFORM = :suse
	elsif ( File.exist?( "/etc/redhat-release" ) )
		HOST_PLATFORM = :redhat
	elsif ( File.exist?( "/etc/redhat_version" ) )
		HOST_PLATFORM = :redhat
	elsif ( File.exist?( "/etc/fedora-release" ) )
		HOST_PLATFORM = :fedora
	elsif ( File.exist?( "/etc/slackware-release" ) )
		HOST_PLATFORM = :slackware
	elsif ( File.exist?( "/etc/slackware_version" ) )
		HOST_PLATFORM = :slackware
	elsif ( File.exist?( "/etc/debian-release" ) )

		HOST_PLATFORM = :debian
	elsif ( File.exist?( "/etc/debian_version" ) )
		HOST_PLATFORM = :debian
	elsif ( File.exist?( "/etc/mandrake-release" ) )
		HOST_PLATFORM = :mandrake
	elsif ( File.exist?( "/etc/gentoo-release" ) )
		HOST_PLATFORM = :gentoo
	elsif ( File.exist?( "/etc/arch-release" ) )
		HOST_PLATFORM = :arch
  else
    HOST_PLATFORM = :unknown
	end

	SDL_SOUND = File.exist?( "/usr/include/SDL/SDL_sound.h" )
	BLUETOOTH = File.exist?( "/usr/include/bluetooth/bluetooth.h" )
end

#warning("Platform: #{HOST}")

if(HOST == :win32) then
	DLL_FILE_ENDING = '.dll'
	EXE_FILE_ENDING = '.exe'
	FOLDER_SEPARATOR = '\\'
elsif(HOST == :darwin)
	DLL_FILE_ENDING = '.dylib'
	EXE_FILE_ENDING = ''
	FOLDER_SEPARATOR = '/'
else
	DLL_FILE_ENDING = '.so'
	EXE_FILE_ENDING = ''
	FOLDER_SEPARATOR = '/'
end
NATIVE_LIB_FILE_ENDING = '.a'

# Compares two filenames, taking host-dependent case sensitivity into account.
def filenamesEqual(a, b)
	if(HOST == :win32)
		return a.casecmp(b) == 0
	else
		return a == b
	end
end
