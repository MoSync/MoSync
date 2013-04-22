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

require "../../rules/util.rb"
require "../../rules/mosync_util.rb"
require "FileUtils"

msbuildpath = "/Windows/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe"

# build everything
ARGV.each do|a|
	sh "#{msbuildpath} /v:q WP8Libs.sln /p:Configuration=#{a}"
	puts "\n\n----------------------------------\n\n"
	puts "Builded with Configuration: #{a}"
	puts "\n----------------------------------\n\n"
end
