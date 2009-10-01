require 'ftools'
require 'sqlite3'
require 'set'

RELEVANT_CAPS = [
	'ScreenSize_x',
	'ScreenSize_y',
	'IconSize_x',
	'IconSize_y',
	'BitsPerPixel',
	'StorageSize',
	'HeapSize',
	'MaxJarSize',
	'JavaPackage',
	'OS',
]

JAVA_RELEVANT_THINGS = [
		'MA_PROF_SUPPORT_JAVAPACKAGE_BTAPI',
		'MA_PROF_BUG_BACKLIGHTFLASHES',
		'MA_PROF_BUG_MICROEMU',
		'MA_PROF_SUPPORT_STYLUS',
		'MA_PROF_SUPPORT_CLDC_10',
		'MA_PROF_SUPPORT_JAVAPACKAGE_LOCATIONAPI',
		'MA_PROF_SUPPORT_JAVAPACKAGE_WMAPI'
]

CAP_TYPES = {
	:constant => [
		'ScreenSize_x',
		'ScreenSize_y',
		'IconSize_x',
		'IconSize_y',
		'BitsPerPixel',
		'StorageSize',
		'HeapSize',
		'MaxJarSize'
	],
	:supports => [
		'JavaPackage',
		'OS'
	]
}



# This class is a representation of a device, used
# to construct a number of sets of devices
#
#
#

$bpp = Set.new

class Device
	#include Comparable
	attr_accessor :platform, :caps
	def initialize(platform)
		@platform = platform
		@caps = {}
	end

	def cmp other
		#puts "in cmp!!!"
		if(@platform != other.platform) then
			raise "HAVOK!"
		end
		if(@platform == "JavaME") then
			[
				'MA_PROF_SUPPORT_JAVAPACKAGE_BTAPI',
				'MA_PROF_BUG_BACKLIGHTFLASHES',
				'MA_PROF_BUG_MICROEMU',
				'MA_PROF_SUPPORT_STYLUS',
				'MA_PROF_SUPPORT_CLDC_10',
				'MA_PROF_SUPPORT_JAVAPACKAGE_LOCATIONAPI',
				'MA_PROF_SUPPORT_JAVAPACKAGE_WMAPI'
			].each do |cap|
				if(@caps.has_key?(cap) != other.caps.has_key?(cap)) then
					return false
				end
			end
			return true
		elsif(@platform == "wm5" || @platform == "sp2003" || @platform == "s60v3")
			true
		elsif(@platform == "s60v2")
			#return ((@caps["BitsPerPixel"].to_i > 16) != (other.caps["BitsPerPixel"].to_i > 16))
			if(@caps["MA_PROF_CONST_BITSPERPIXEL"] == nil || other.caps["MA_PROF_CONST_BITSPERPIXEL"] == nil) then
				if(@caps["MA_PROF_CONST_BITSPERPIXEL"] == nil && other.caps["MA_PROF_CONST_BITSPERPIXEL"] == nil) then
					return true;
				else
					return false
				end
			end
			if((@caps["MA_PROF_CONST_BITSPERPIXEL"].to_i > 16) && (other.caps["MA_PROF_CONST_BITSPERPIXEL"].to_i > 16)) then
				return true
			elsif((@caps["MA_PROF_CONST_BITSPERPIXEL"].to_i <= 16) && (other.caps["MA_PROF_CONST_BITSPERPIXEL"].to_i <= 16)) then
				return true
			else
				return false
			end
		end
	end
end

filename = ARGV[0]
#destdir = ARGV[1]
bitmap = {}

devices = {
	:wm5    => [],
	:sp2003 => [],
	:s60v2  => [],
	:s60v3  => [],
	:JavaME => []
}

class Array
	def add_device(d)
#		JAVA_RELEVANT_THINGS.each do |t|
			#puts "d: #{t} : '#{d.caps[t]}'"
#		end
		self.each do |e|
			if(e.cmp d) then
				return
			end
		end
		self << d
	end
end

puts "Reading database: #{filename}"
db = SQLite3::Database.new( filename )

device_query = db.prepare("SELECT capvalue.value from capvalue INNER JOIN" <<
			   " devicecapvalue ON devicecapvalue.capvalue=capvalue.id INNER JOIN" <<
			   " cap ON devicecapvalue.cap=cap.id AND cap.name=? AND" <<
			   " devicecapvalue.device=?");
			   
File.makedirs "output"
db.execute( "select name from vendor" ) do |vendor|
#db.execute( "select name from vendor where name='Nokia'" ) do |vendor|
	puts vendor
	File.makedirs "output/#{vendor}"
	db.execute("SELECT device.id, device.name, platform.name FROM device, vendor " <<
		"INNER JOIN platformversion ON device.platformversion = platformversion.id " <<
		"INNER JOIN platform ON platformversion.platform = platform.id " <<
		"WHERE vendor.id=device.vendor AND vendor.name=\"#{vendor}\"") do |device|
		dev_obj = Device.new(device[2].to_s)
		seen_defines = []
		device_path = "output/#{vendor}/#{device[1]}"
		File.makedirs device_path
		File.open("#{device_path}/maprofile.h", 'w') do |profile|  
			profile.puts "#ifndef _MSAB_PROF_H_\n#define _MSAB_PROF_H_"
			profile.puts
			profile.puts "MA_PROF_STRING_VENDOR \"#{vendor}\""
			profile.puts "MA_PROF_STRING_DEVICE \"#{device[1]}\""
			profile.puts "MA_PROF_VENDOR_#{vendor[0].upcase}"
			profile.puts "MA_PROF_DEVICE_#{device[1].upcase}"
			profile.puts
			RELEVANT_CAPS.each do |cap|
				device_query.bind_params(cap.to_s, device[0].to_s);
				device_query.execute(
				) do |value|
					if(CAP_TYPES[:constant].include? cap)
						value.each do |v|
							def_name = "MA_PROF_CONST_#{cap.upcase}"
							dev_obj.caps[def_name] = "#{v.to_s.upcase}"
							def_str = "#define #{def_name} #{v.to_s.upcase}"
							if(!(seen_defines.include? def_name)) then
								seen_defines << def_name
								profile.puts def_str
							end
						end
					elsif(CAP_TYPES[:supports].include? cap)
						value.each do |v|
							v.to_s.upcase!
							def_name = "MA_PROF_SUPPORT_#{cap.upcase}"
							if(!(seen_defines.include? def_name)) then
								if(def_name !=  "MA_PROF_SUPPORT_JAVAPACKAGE") then
									seen_defines << def_name
								end
								dev_obj.caps["#{def_name}_#{v.to_s.upcase}"] = "TRUE";
								def_str = "#define #{def_name}_#{v.to_s.upcase}"
								profile.puts def_str
							end
						end
					end
				end
			end
			profile.puts "\n#endif /* _MSAB_PROF_H_ */"
		end
		$bpp.add dev_obj.caps["BitsPerPixel"].to_i	
		devices[device[2].to_sym].add_device dev_obj

	end
	puts $bpp.to_s	
end
	devices.each do |platform, devs|
		puts "#{platform} has #{devs.size} devices!"
	end
	devices[:JavaME].each do |device|
		puts "\ndevice:"
		device.caps.each do |define, value|
			puts "#{define} = #{value}" 
		end
	end
