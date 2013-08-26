# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
					hash[name] = value.strip
				end
			end
		end
		#p hash
		return hash
	end
end
