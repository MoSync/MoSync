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

require 'ftools'
require 'sqlite3'

RELEVANT_CAPS = [
	'ScreenSize_x',
	'ScreenSize_y',
	'HeapSize',
	'IconSize_x',
	'IconSize_y'
]

filename = ARGV[0]
#destdir = ARGV[1]
db = SQLite3::Database.new( filename )
File.makedirs "output"
db.execute( "select name from vendor" ) do |vendor|
	puts vendor
	File.makedirs "output/#{vendor}"
	db.execute("select device.id, device.name from device, vendor where vendor.id=device.vendor and vendor.name=\"#{vendor}\"") do |device|
		puts "\t#{device[1]}"
		File.makedirs "output/#{vendor}/#{device[1]}"
		RELEVANT_CAPS.each do |cap|
			db.execute( "select capvalue.value from cap, capvalue, devicecapvalue where cap.name=\"#{cap}\" and devicecapvalue.device=#{device[0]} and devicecapvalue.cap=cap.id and devicecapvalue.capvalue=capvalue.id" ) do |value|
				puts "\t\t#{cap} : #{value}"
			end
		end
	end
end
