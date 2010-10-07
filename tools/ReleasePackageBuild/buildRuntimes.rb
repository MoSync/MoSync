# Copyright (C) 2010 MoSync AB
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


require 'fileutils'

def returnToBuild(path, ret)
	FileUtils.cd path
	return ret
end

# The the current path
cpath = FileUtils.pwd

# Make the output folder
FileUtils.mkdir_p "#{ENV['MOSYNC_PROFILES_PATH']}"

internalBuild = ENV['MOSYNC_INTERNAL_BUILD']

# If this is not an internal MoSync build, generate the runtimes which is possible to create from Windows
# Symbian, JavaME, Windows Mobile and Android
# Othervise we shall use the distributed build system

if(internalBuild == nil)

	puts "Building runtimes"

	FileUtils.cd "#{ENV['MOSYNC_TRUNK']}/tools/profileConverter"
	
	returnToBuild cpath, system("ruby conv.rb -dst #{ENV['MOSYNC_PROFILES_PATH']}")
else
	
	puts "Build MoSync Internal runtimes"
	
	FileUtils.cd "#{ENV['MOSYNC_RELEASE_BUILD_PATH']}"
	ret = system "ruby private-runtimebuilder/lib/buildmaster.rb profiles.zip mosync-trunk"
	if true == ret
		system "unzip profiles.zip -d #{ENV['MOSYNC_PATH']}"
	end
	
	returnToBuild cpath, ret
end
