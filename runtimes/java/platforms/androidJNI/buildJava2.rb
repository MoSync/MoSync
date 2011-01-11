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

# TODO: Replace this check with a check for other essential files?
#if !File.exist? "src/config_platform.h"
#	puts "\nFATAL ERROR! - No config_platform.h file found!"
#	return 1
#end

#
# Copy shared generated Java files to package com.mosync.internal.android.
#

# Set directory paths.
sharedGeneratedJavaSourceDir = "../../Shared/generated/"
outDir = "AndroidProject/src/com/mosync/internal/generated/"

# TODO: Document when this is used. On the build server?
if(ENV['MOSYNC_SRC'] != nil)
	shared_java_source = ENV['MOSYNC_SRC'] + "/runtimes/java/Shared/generated/"
	outDir = ENV['MOSYNC_SRC'] + 
		"/runtimes/java/platforms/AndroidJNI/" +
		"AndroidProject/src/com/mosync/internal/generated/"
end

puts "sharedGeneratedJavaSourceDir: " + sharedGeneratedJavaSourceDir
puts "outDir: " + outDir

# Copy .java files.
Dir.foreach(sharedGeneratedJavaSourceDir) do |fileName| 
	if (/\.java$/.match fileName)
		puts "Copying " + fileName
		FileUtils.cp sharedGeneratedJavaSourceDir + fileName, outDir + fileName
	end
end

exit 0
