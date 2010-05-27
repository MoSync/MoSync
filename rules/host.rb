# Copyright (C) 2009 Mobile Sorcery AB
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

# This file defines a few constants that describe the host environment.
# HOST, Symbol. Either :linux or :win32.
# GCC_IS_V4, boolean. True if gcc's major version number is 4.
# GCC_IS_V43, boolean. True if gcc's version number is 4.3 or greater.
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
elsif(UNAME == "windows32")
	HOST = :win32
elsif(UNAME == "CYGWIN_NT-5.1")
	HOST = :linux
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
