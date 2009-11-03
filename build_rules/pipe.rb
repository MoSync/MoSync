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

BUILDDIR_BASE = "build/pipe/"

require "#{BD}/build_rules/util.rb"

default(:PIPE_ONLY, true)

require "#{BD}/build_rules/defaults.rb"
require "#{BD}/build_rules/host.rb"
require "#{BD}/build_rules/mosync_common.rb"

MACHDEP = " -DMAPIP"

INCLUDE_DIRS = [".", "#{BD}/libs/MAStd", "#{BD}/libs"] + EXTRA_INCLUDES

if(CONFIG == "debug") then
	INTRA_CFLAGS = " -DMOSYNCDEBUG -S" + EXTRA_CFLAGS
elsif(CONFIG == "")
	INTRA_CFLAGS = " -g -S" + EXTRA_CFLAGS
else
	error "wrong configuration: " + CONFIG
end

OBJECT_ENDING = ".s"
GCC = "#{MOSYNCBIN}xgcc"
GPP = GCC

LESSER_CONLY2 = " -Wmissing-declarations"
INTRA_CXXFLAGS = EXTRA_CXXFLAGS

def libpath(libname)
	return "#{MOSYNCDIR}/lib/pipe/#{libname}#{CONFIG_POSTFIX}.lib"
end

if(defined?(LIBNAME) != nil) then
	LIBTARGET = libpath(LIBNAME)
end

if(defined?(LIBTARGET) != nil) then
	INTRA_LIBS = []
else
	INTRA_LIBS = DEFAULT_LIBS + LOCAL_DLLS.collect {|lib| "#{lib}"}
end
INTRA_OBJECTS = INTRA_LIBS.collect {|lib| libpath(lib) }

def build_lib(t)
	sh "#{MOSYNCBIN}pipe-tool -L #{t.name} #{t.prerequisites}"
end

require "#{BD}/build_rules/common.rb"

def build_program(t)
	sh "cd #{BUILDDIR} && #{PIPETOOL}#{PIPEFLAGS} -sld -collect-stabs -B tempProgram #{expand_paths(t.prerequisites)}" do |ok, res|
		if(ok) then
			sh "mv #{BUILDDIR}tempProgram #{t.name}"
		else
			error("pipe-tool failure")
		end
	end
end
