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
verbose = "q"

if ARGV.length != 0
	ARGV.each do|a|
		if a == "--help"
			puts "For building a certain configuration pass it as a command line parameter\n"
			puts "e.g.:"
			puts "\n     \"ruby buildLibraries.rb Release\" will build for release;"
			puts "\n     \"ruby buildLibraries.rb Debug\" will build for degub;"
			puts "\n     \"ruby buildLiraries.rb Release Debug\" will build for both release and debug;"
			puts "\nFor setting the verbose option pass one of the following options:"
			puts "\n     q (quite)"
			puts "\n     m (minimal)"
			puts "\n     n (normal)"
			puts "\n     d (detailed)"
			puts "\n     diag (diagnostic)"
			puts "\ne.g.:"
			puts "\n     \"ruby buildLibraries.rb q Debug\" will build for debug verbose \"quite\";"
			puts "\nNote: The default value is q"
		else
			if a == "q" || a == "m" || a == "n" || a == "d" || a == "diag"
				verbose = a
			else
				if a == "release" || a == "debug" || a == "Release" || a == "Debug"
					sh "#{msbuildpath} /v:#{verbose} WP8Libs.sln /p:Configuration=#{a}"
					puts "\n\n----------------------------------\n\n"
					puts "Builded with Configuration: #{a}"
					puts "\n----------------------------------\n\n"
				else
					puts "For help pass --help as an option.\n"
				end
			end
		end
	end
else
	puts "\nNo option given; Building for Release with verbose \"quite\"; Pass --help for more information;\n\n"
	sh "#{msbuildpath} /v:q WP8Libs.sln /p:Configuration=Release"
	puts "\n\n----------------------------------\n\n"
	puts "Builded with Configuration: Release"
	puts "\n----------------------------------\n\n"
end
