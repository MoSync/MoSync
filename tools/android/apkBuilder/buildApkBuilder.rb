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

require 'fileutils'

class_dir = "temp/"
output_dir = ".."
arg = ARGV[0]
if arg.class != NilClass
	output_dir = "#{arg}/"
end

if File.exist? class_dir
	FileUtils.rm_rf class_dir
end
Dir.mkdir class_dir

# build apkBuilder
puts("Compiling androidprefs")
system("javac -source 1.6 -target 1.6 -d temp/  androidprefs/src/com/android/prefs/AndroidLocation.java");
puts("Compiling jarutils")
system("javac -source 1.6 -target 1.6 -d temp/ -cp temp jarutils/src/com/android/jarutils/*.java");
puts("Compiling apkbuilder")
system("javac -source 1.6 -target 1.6 -d temp/ -cp temp apkbuilder/src/com/android/apkbuilder/ApkBuilder.java");

FileUtils.cd class_dir
puts("building Apk Builder\n");
system("jar cfm #{output_dir}/apkbuilder.jar ../apkbuilder/etc/manifest.txt .")
FileUtils.cd ".."

FileUtils.rm_rf class_dir

