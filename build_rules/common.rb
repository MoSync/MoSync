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

require "#{BD}/build_rules/loader_md.rb"
require "#{BD}/build_rules/host.rb"

class String
	def endsWith(e)
		return self[self.length - e.length, e.length] == e
	end
end

def genfile(source, ending)
	return BUILDDIR + File.basename(source).ext(ending)
end

def objects(sources)
	return sources.collect do |path| genfile(path, OBJECT_ENDING) end
end


if(CONFIG == "debug") then
	_CFLAGS_BASE2 = " -g#{INTRA_CFLAGS}" #-save-temps
elsif(CONFIG == "")
	_CFLAGS_BASE2 = " -O2 -Wdisabled-optimization#{INTRA_CFLAGS}"
else
	error "wrong configuration: " + CONFIG
end

LESSER_WARNINGS =" -Wpointer-arith -Wundef -Wfloat-equal" +
	" -Winit-self"

UNUSED_WARNINGS = " -Wunreachable-code -Wmissing-noreturn -Wstrict-prototypes -Wold-style-definition" +
	" -Wmissing-prototypes -Wmissing-format-attribute -Winline"

LESSER_CONLY = " -Wnested-externs" +
	" -Wdeclaration-after-statement -Wno-format-zero-length" + LESSER_CONLY2

STANDARD_WARNINGS = " -Wall -Werror -Wextra -Wno-unused-parameter -Wwrite-strings -Wshadow"

_CFLAGS_BASE = "#{STANDARD_WARNINGS}#{LESSER_WARNINGS}#{_CFLAGS_BASE2}"

INCLUDE_FLAGS = INCLUDE_DIRS.collect {|dir| " -I "+dir}

_CXXFLAGS_BASE = " -fno-rtti -Wno-deprecated#{EXTRA_CXXFLAGS}"

CFLAGS = "#{MACHDEP}#{_CFLAGS_BASE}#{INCLUDE_FLAGS.to_s}"
CXXFLAGS = "#{CFLAGS}#{_CXXFLAGS_BASE}"

MAKEDEPEND_ENDING = '.md'

def declare_compile_task(source, build_func)
	#print("'"+source+"'\n")
	out = genfile(source, OBJECT_ENDING)
	myTask = file out => source do |t| build_func.call(out, source) end
	file genfile(source, MAKEDEPEND_ENDING) => source
	if(!myTask.needed?) then
		#puts "import #{genfile(source, MAKEDEPEND_ENDING)}"
		depfile = genfile(source, MAKEDEPEND_ENDING)
		if(File.exist?(depfile)) then
			import depfile
		else
			#force rebuild
			File.delete(out)
		end
		#at this point, if a header file was renamed or removed, rake will fail,
		#until the MF files referring to the missing file are deleted.
		#we should remove the dependencies added by the MF file that doesn't exist.
		#beware, however, that the MF also includes the 'source' file as a dependency.
		#don't remove that one.
		
		#So: for each dependency of myTask at this point,
		#remove the dependency if it does not exist and is not 'source'
		
		#myTask = file out
		#puts "pre2 #{out} => #{myTask.prerequisites}"
		#myTask.prerequisites.reject! do |p|
		#	res = (!File.exists?(p) && p != source)
		#	puts "Test '#{p}': #{res}"
		#	return res
		#end
		
		#Now, if a header file was wrongly removed, Rake won't mind but GCC will fail.
	end
end

def depend_flags(src)
	return " -MMD -MF #{genfile(src, MAKEDEPEND_ENDING)}#{SPECIFIC_CFLAGS.fetch(File.basename(src), "")}"
end

directory BUILDDIR
directory BUILDPATH

task :dirs => [ BUILDDIR, BUILDPATH ]

task :default => :dirs

task :clean do
	sh "rm -rf #{BUILDDIR} #{BUILDPATH}"
end

def collect_files(ending)
	files = (SOURCES.collect {|dir| FileList[dir+"/*"+ending]})
	files.flatten!
	files.reject! {|file| IGNORED_FILES.member?(File.basename(file))}
	files += EXTRA_SOURCEFILES.select do |file| file.endsWith(ending) end
	return files
end

CFILES = collect_files(".c")
CPPFILES = collect_files(".cpp") + collect_files(".cc")
ALL_SOURCEFILES = CFILES + CPPFILES

SOURCE_OBJECTS = objects(ALL_SOURCEFILES)
OBJECTS = SOURCE_OBJECTS + INTRA_OBJECTS + EXTRA_OBJECTS

def build_cpp(out, src)
	src = File.expand_path(src)
	puts src
	sh "#{GPP}#{CXXFLAGS}#{depend_flags(src)} #{src} -o #{out}"
end

def build_c(out, src)
	src = File.expand_path(src)
	puts src
	sh "#{GCC}#{CFLAGS}#{LESSER_CONLY}#{depend_flags(src)} #{src} -o #{out}"
end

CFILES.each do |source| declare_compile_task(source, method(:build_c)) end
CPPFILES.each do |source| declare_compile_task(source, method(:build_cpp)) end

#puts "Compile tasks declared"

if(defined?(LIBTARGET) != nil) then
	myTask = file LIBTARGET => OBJECTS do |t|
		directory File.dirname(LIBTARGET)
		d = task File.dirname(LIBTARGET) => :dirs
		d.invoke
		build_lib(t)
	end
	task :default => myTask
	task :clean do
		sh "rm -f #{LIBTARGET}"
	end
	
	if(defined?(INSTALL_INCDIR) != nil) then
		
		INSTALL_INCPATH = "#{MOSYNCDIR}/include/#{INSTALL_INCDIR}/"

	def declare_copy_task(src, dst)
		file dst => src do |t|
			sh "cp \"#{src}\" \"#{dst}\""
		end
	end

	def collect_hfiles()
		files = (SOURCES.collect {|dir| FileList[dir+"/*.h"]})
		files.flatten!
		files.reject! {|file| IGNORED_HEADERS.member?(File.basename(file))}
		files += EXTRA_HEADERFILES
		return files
	end

	def installed_hfile(hfile)
		INSTALL_INCPATH + File.basename(hfile).ext(".h")
	end

	HFILES = collect_hfiles()
	INSTALLED_HFILES = HFILES.collect do |hfile| installed_hfile(hfile) end

	HFILES.each do |hfile| declare_copy_task(hfile, installed_hfile(hfile)) end

	directory INSTALL_INCPATH
	task :default => INSTALL_INCPATH

	task :default => INSTALLED_HFILES

	task :clean do
		if(INSTALL_INCPATH[-3,2] != "/.") then
			sh "rm -rf #{INSTALL_INCPATH}"
		end
	end
	end
end

#puts "LIBTARGET handled"
