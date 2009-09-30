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

# usage: ruby RuntimeBuilder.rb <Settings.rb file> <platform> <runtime directory (containing config.h)>

class RuntimeBuilder 	
	def method_missing(method, *args) 
		puts "RuntimeBuilder failed: invalid platform " + method.to_s
	end

	def build(platform, runtime_dir)		
		puts "building " + platform
		send(platform, runtime_dir)
	end
end

require ARGV[0]
require "BuildJavaME.rb"
require "BuildS60.rb"
require "BuildWinCE.rb"

runtime_builder = RuntimeBuilder.new
runtime_builder.build(ARGV[1], ARGV[2])
