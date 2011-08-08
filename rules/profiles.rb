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
