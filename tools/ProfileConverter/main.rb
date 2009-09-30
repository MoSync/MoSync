require 'ftools'
require 'sqlite3'

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
	'OS'
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
#	:bug => [
#		'BACKLIGHTFLASHES'
#	]
}

JAVAPACKAGE_SUPPORT = [
		'BTAPI',
#		'MA_PROF_BUG_BACKLIGHTFLASHES',
#		'MA_PROF_BUG_MICROEMU',
#		'MA_PROF_SUPPORT_FRAMEBUFFER_32BIT',
#		'MA_PROF_SUPPORT_STYLUS',
#		'MA_PROF_SUPPORT_CLDC_10',
		'LOCATIONAPI',
		'WMAPI'
]

filename = ARGV[0]
#destdir = ARGV[1]
bitmap = {}

puts "Reading database: #{filename}"
db = SQLite3::Database.new( filename )
File.makedirs "output"
#db.execute( "select name from vendor" ) do |vendor|
	db.execute( "select name from vendor where name='Nokia'" ) do |vendor|
	puts vendor
	File.makedirs "output/#{vendor}"
	db.execute("SELECT device.id, device.name FROM device, vendor WHERE" <<
		       " vendor.id=device.vendor AND vendor.name=\"#{vendor}\"") do |device|
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
				db.execute(
					"SELECT capvalue.value from capvalue INNER JOIN" <<
					" devicecapvalue ON devicecapvalue.capvalue=capvalue.id INNER JOIN" <<
					" cap ON devicecapvalue.cap=cap.id AND cap.name=\"#{cap}\" AND" <<
					" devicecapvalue.device=#{device[0]}" 
				) do |value|
					cap_type = "ERROR"
					if(CAP_TYPES[:constant].include? cap)
						def_name = "MA_PROF_CONST_#{cap.upcase}"
						def_str = "#define #{def_name} #{value.to_s.upcase}"
						#puts "current def_name: #{def_name}"
						#puts "seen_defines: #{seen_defines.inspect}"
						if(!(seen_defines.include? def_name)) then
							seen_defines << def_name
							profile.puts def_str
						end
					elsif(CAP_TYPES[:supports].include? cap)
						value.each do |v|
							v.upcase!
							def_name = "MA_PROF_SUPPORT_#{cap.upcase}"
							def_str = "#define #{def_name}_#{v}"
							#puts "current def_name: #{def_name}"
							#puts "seen_defines: #{seen_defines.inspect}"
							if(!(seen_defines.include? def_name)) then
								if(def_name !=  "MA_PROF_SUPPORT_JAVAPACKAGE") then
									seen_defines << def_name
								end
								profile.puts def_str
							end
						end
					end
				end
			end
			profile.puts "\n#endif /* _MSAB_PROF_H_ */"
		end
	end
end

