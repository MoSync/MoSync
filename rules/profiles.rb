# Copyright (C) 2011 MoSync AB
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.

require "#{File.dirname(__FILE__)}/util.rb"
require "#{File.dirname(__FILE__)}/mosync_util.rb"

class Profiles
	# Returns a hash of profile info, parsed from maprofile.h
	def self.parse(profileName)
		hash = {}
		open("#{mosyncdir}/profiles/vendors/#{profileName}/maprofile.h", 'r').each do |line|
			s = '#define '
			if(line.beginsWith('#define MA_PROF_'))
				spaceIndex = line.index(' ', s.length)
				if(spaceIndex == nil)
					name = 'MA_PROF_'+line.slice(s.length, line.length - (s.length + 1))
					hash[name] = nil
				else
					name = line.slice(s.length, spaceIndex - s.length)
					value = line.slice(spaceIndex + 1, line.length - (spaceIndex + 2))
					hash[name] = value
				end
			end
		end
		#p hash
		return hash
	end
end
