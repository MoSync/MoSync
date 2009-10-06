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

require "#{BD}/build_rules/mosync_common.rb"
require "#{BD}/build_rules/util.rb"

default(:LSTFILES, FileList["*.lst"])

task :clean do
	sh "rm -rf build"
	sh "rm -rf MAHeaders.h"
end

def dir_res()
	bdt = task BUILDDIR
	bdt.invoke
	if(defined?(RESTARGET) != nil) then
		rt = task RESTARGET
		rt.invoke
	end
end

def build_res(t)
	sh "#{PIPETOOL} -R #{t.name} #{t.prerequisites}"
end

if(LSTFILES != [] && defined?(RESTARGET) == nil) then
	RESTARGET = "build/resources"
end

if(defined?(RESTARGET) != nil) then
	file RESTARGET => LSTFILES do |t|
		build_res(t)
	end
	file "MAHeaders.h" => RESTARGET
end

task :pipe do
	#puts "top_level_tasks: #{Rake.application.top_level_tasks}"
	require "#{BD}/build_rules/pipe.rb"
	P = "#{BUILDDIR}program"
	dir_res()
	task = file P => OBJECTS do |t|
		build_program(t)
	end
	task.invoke
end

task :run => :pipe do
	R = (defined?(RESTARGET) != nil) ? " -resource #{File.expand_path(RESTARGET)}" : ""
	sh "cd #{BUILDDIR} && #{MOSYNCBIN}moemu -program program#{R}"
end

task :native do
	require "#{BD}/build_rules/defaults.rb"
	EXETARGET = "#{BUILDDIR}#{EXENAME}#{EXE_FILE_ENDING}"
	require "#{BD}/build_rules/native_mosync.rb"
	dir_res()
	(task :default).invoke
end

task :runn => :native do
	sh "cd #{BUILDDIR} && ./#{EXENAME}"
end
