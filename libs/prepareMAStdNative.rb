#!/usr/bin/ruby

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

require "../rules/util.rb"
require "../rules/mosync_util.rb"
require "fileutils"

FileUtils.mkpath(["MAStdNative"])

FileUtils.cd "MAStd"
FileUtils.cp_r ["IX_OPENGL_ES.h", "maapi.h", "madmath.h", "maxtoa.h", "maapi_defs.h", "maarg.h", "IX_WIDGET.h", "maheap.h", "matime.h", "conprint.h", "maassert.h", "mastdlib.h", "mavsprintf.h", "ma.h", "mactype.h", "mastring.h", "mawvsprintf.h", "mawstring.h", "IX_OPENGL_ES_MA.h", "IX_OPENGL_ES.h", "IX_GL1.h", "IX_GL2.h", "IX_PIM.h", "GLES", "GLES2"], "../MAStdNative/", :verbose => true

FileUtils.cd ".."

@target_dir = "MAStdNative/"
Dir.glob('**/*.h*').each do |file|
	dir, filename = File.dirname(file), File.basename(file)
	dest = File.join(@target_dir, dir)
	FileUtils.mkdir_p(dest)
	FileUtils.copy_file(file, File.join(dest,filename))
end

Dir.glob('**/*.inl').each do |file|
	dir, filename = File.dirname(file), File.basename(file)
	dest = File.join(@target_dir, dir)
	FileUtils.mkdir_p(dest)
	FileUtils.copy_file(file, File.join(dest,filename))
end

FileUtils.cp_r Dir.glob('MAStdNative/yajl/src/api/*.h'), "MAStdNative/yajl/"
FileUtils.rm_r 'MAStdNative/yajl/src'

FileUtils.mv "MAStdNative/maapi.h", "MAStdNative/maapi.h.old"

lineBuffer = []
tempfile = File.open("MAStdNative/maapi.h", 'w')
File.open("MAStdNative/maapi.h.old").each do |line|
	if(line['char* strcpy']) then
		lineBuffer = []
	else
		lineBuffer.push(line)
		if(lineBuffer.size() > 3) then
			tempfile.puts(lineBuffer.shift())
		end
	end
end

lineBuffer.each() do |line|
	tempfile.puts(line)
end

tempfile.close()

FileUtils.mkpath([mosyncdir+"/include"])
FileUtils.cp_r "MAStdNative", mosyncdir + "/include/"