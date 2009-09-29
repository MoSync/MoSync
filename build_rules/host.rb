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

require "#{BD}/build_rules/error.rb"

MOSYNCDIR = ENV['MOSYNCDIR']

UNAME = open("|uname").readline().strip()

if(UNAME == "Linux")
	HOST = "linux"

	if ( File.exist?( "/etc/moblin-release" ) )
		HOST_PLATFORM = "moblin"
	elsif ( File.exist?( "/etc/lsb-release" ) )
		HOST_PLATFORM = "ubuntu"
	elsif ( File.exist?( "/etc/SUSE-release" ) )
		HOST_PLATFORM = "suse"
	elsif ( File.exist?( "/etc/redhat-release" ) )
		HOST_PLATFORM = "redhat"
	elsif ( File.exist?( "/etc/redhat_version" ) )
		HOST_PLATFORM = "redhat"
	elsif ( File.exist?( "/etc/fedora-release" ) )
		HOST_PLATFORM = "fedora"
	elsif ( File.exist?( "/etc/slackware-release" ) )
		HOST_PLATFORM = "slackware"
	elsif ( File.exist?( "/etc/slackware_version" ) )
		HOST_PLATFORM = "slackware"
	elsif ( File.exist?( "/etc/debian-release" ) )
		HOST_PLATFORM = "debian"
	elsif ( File.exist?( "/etc/debian_version" ) )
		HOST_PLATFORM = "debian"
	elsif ( File.exist?( "/etc/mandrake-release" ) )
		HOST_PLATFORM = "mandrake"
	elsif ( File.exist?( "/etc/gentoo-release" ) )
		HOST_PLATFORM = "gentoo"
	end

	SDL_SOUND = File.exist?( "/usr/include/SDL/SDL_sound.h" )
	BLUETOOTH = File.exist?( "/usr/include/bluetooth/bluetooth.h" )

elsif(UNAME == "MINGW32_NT-5.1")
	HOST = "win32"

elsif(UNAME == "CYGWIN_NT-5.1")
	HOST = "linux"

else
	error("Unknown platform: #{UNAME}")
end

GCC_VERSION = (open("|gcc --version").readline().split(/ /))[2]
GCC_IS_V4 = (GCC_VERSION[0] >= "4"[0])
GCC_IS_V43 = (GCC_IS_V4 && (GCC_VERSION[2] >= "3"[0]))

# debugging
#warning("Platform: #{HOST}")
#warning("GCC version: #{GCC_VERSION}")
#warning("GCC_IS_V4: #{GCC_IS_V4}")
#warning("GCC_IS_V43: #{GCC_IS_V43}")
