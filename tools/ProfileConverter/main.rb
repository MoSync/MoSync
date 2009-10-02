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
	
	'Features',
	'Bugs',
	'JavaConfiguration'
]

RELEVANT_DEFINES = {

	:JavaME => [
		'MA_PROF_SUPPORT_JAVAPACKAGE_BLUETOOTH',
		'MA_PROF_BUG_BACKLIGHTFLASHES',
		'MA_PROF_BUG_MICROEMU',
		'MA_PROF_SUPPORT_STYLUS',
		'MA_PROF_SUPPORT_CLDC_10',
		'MA_PROF_SUPPORT_JAVAPACKAGE_LOCATIONAPI',
		'MA_PROF_SUPPORT_JAVAPACKAGE_WMAPI'
	],
	
	:s60v2 => ['MA_PROF_SUPPORT_FRAMEBUFFER_32BIT'],
	:s60v3 => [],
	:sp2003 => [],
	:wm5 => []
}

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

 class Device
	attr_accessor :name, :vendor
	def initialize(name, vendor)
		@name, @vendor = name, vendor
	end
 end

class Runtime
	attr_accessor :platform, :caps, :devices
	def initialize(platform)
		@platform = platform
		@caps = {}
		@devices = []
	end
	
	def add_missing_defines
		caps.each do |cap, value|
			if(platform=="s60v2" && cap == "MA_PROF_CONST_BITS_PER_PIXEL")
				if(value.to_i > 16)
					@caps["MA_PROF_SUPPORT_FRAMEBUFFER_32BIT"] = "TRUE"
				end
			end
		end
	end
	
	def cmp other
		if(@platform != other.platform) then
			raise "HAVOK!"
		end
		
		RELEVANT_DEFINES[platform.to_sym].each do |cap|
			if(@caps.has_key?(cap) != other.caps.has_key?(cap)) then
				return false
			end
		end		
	end
end

def write_config_h(runtime, file, relevant_defines, extra_defines)
	File.open(file, 'w') do |config_h|
		config_h.puts "#ifndef CONFIG_H"
		config_h.puts "#define CONFIG_H"
				
		relevant_defines.each do |cap|
			if(runtime.caps.has_key?(cap))
				config_h.puts "#define #{cap}"
			end
		end
		
		extra_defines.each do |define|
			config_h.puts "#define #{define}"		
		end
		
		config_h.puts "#endif /* CONFIG_H */"		
	end
end


filename = ARGV[0]
#destdir = ARGV[1]
bitmap = {}

runtimes = {
	:wm5    => [],
	:sp2003 => [],
	:s60v2  => [],
	:s60v3  => [],
	:JavaME => []
}

class Array
	def add_runtime(d)
#		JAVA_RELEVANT_THINGS.each do |t|
			#puts "d: #{t} : '#{d.caps[t]}'"
#		end
		self.each do |e|
			if(e.cmp d) then
				return e
			end
		end
		self << d
		return d
	end
end

class String
	def format
		return gsub(/[\-\.\s]/, "_").upcase();
	end
end

puts "Reading database: #{filename}"
db = SQLite3::Database.new( filename )

device_query = db.prepare("SELECT capvalue.value from capvalue INNER JOIN" <<
			   " devicecapvalue ON devicecapvalue.capvalue=capvalue.id INNER JOIN" <<
			   " cap ON devicecapvalue.cap=cap.id AND cap.name=? AND" <<
			   " devicecapvalue.device=?");
VENDOR_DIR = "profiles/vendors"
RUNTIME_DIR = "profiles/runtimes"
File.makedirs VENDOR_DIR
File.makedirs RUNTIME_DIR
db.execute( "select name from vendor" ) do |vendor|
#db.execute( "select name from vendor where name='Nokia'" ) do |vendor|
	puts vendor
	File.makedirs "#{VENDOR_DIR}/#{vendor}"
	db.execute("SELECT device.id, device.name, platform.name FROM device, vendor " <<
		"INNER JOIN platformversion ON device.platformversion = platformversion.id " <<
		"INNER JOIN platform ON platformversion.platform = platform.id " <<
		"WHERE vendor.id=device.vendor AND vendor.name=\"#{vendor}\"") do |device|
		rt_obj = Runtime.new(device[2].to_s)
		seen_defines = []
		device_path = "#{VENDOR_DIR}/#{vendor}/#{device[1]}"
		File.makedirs device_path
		File.open("#{device_path}/maprofile.h", 'w') do |profile|  
			profile.puts "#ifndef _MSAB_PROF_H_\n#define _MSAB_PROF_H_"
			profile.puts
			profile.puts "MA_PROF_STRING_VENDOR \"#{vendor}\""
			profile.puts "MA_PROF_STRING_DEVICE \"#{device[1]}\""
			profile.puts "MA_PROF_VENDOR_#{vendor[0].format}"
			profile.puts "MA_PROF_DEVICE_#{device[1].format}"
			profile.puts
			RELEVANT_CAPS.each do |cap|
				device_query.bind_params(cap.to_s, device[0].to_s);
				device_query.execute(
				) do |value|
					if(CAP_TYPES[:constant].include? cap)
						value.each do |v|
							def_name = "MA_PROF_CONST_#{cap.format}"
							rt_obj.caps[def_name] = "#{v.to_s.upcase}"
							def_str = "#define #{def_name} #{v.to_s.format}"
							if(rt_obj.platform=="s60v2" && def_name == "MA_PROF_CONST_BITSPERPIXEL")
								if(rt_obj.caps[def_name].to_i > 16)
									rt_obj.caps["MA_PROF_SUPPORT_FRAMEBUFFER_32BIT"] = "TRUE"
									profile.puts "#define MA_PROF_SUPPORT_FRAMEBUFFER_32BIT"
								end
							end
							
							if(!(seen_defines.include? def_name)) then
								seen_defines << def_name
								profile.puts def_str
							end
						end
					elsif(CAP_TYPES[:supports].include? cap)
						value.each do |v|							
							def_name = "MA_PROF_SUPPORT_#{cap.format}"
							if(!(seen_defines.include? def_name)) then
								if(def_name !=  "MA_PROF_SUPPORT_JAVAPACKAGE") then
									seen_defines << def_name
								end
								rt_obj.caps["#{def_name}_#{v.to_s.format}"] = "TRUE";
								def_str = "#define #{def_name}_#{v.to_s.format}"
								profile.puts def_str
							end
						end
					elsif(cap == "Bugs")
						value.each do |v|
							def_name = "MA_PROF_BUG_#{v.to_s.format}"
							rt_obj.caps[def_name] = "TRUE";
							profile.puts "#define #{def_name}"
						end
					elsif(cap == "Features")
						value.each do |v|
							#puts v.to_s
							if v.to_s == "hasPointerEvents"
								rt_obj.caps["MA_PROF_SUPPORT_STYLUS"] = "TRUE";
								def_str = "#define MA_PROF_SUPPORT_STYLUS"
								profile.puts def_str							
							elsif v.to_s == "isVirtual"
								## should not generate this profile......
							end
						end
					elsif(cap == "JavaConfiguration")
						value.each do |v|
							if (v.to_s == "CLDC/1.0" ||
								v.to_s == "CLDC/1.0.4")
								rt_obj.caps["MA_PROF_SUPPORT_CLDC_10"] = "TRUE";
								def_str = "#define MA_PROF_SUPPORT_CLDC_10"
								profile.puts def_str
							end
						end
					end
				end
			end
			profile.puts "\n#endif /* _MSAB_PROF_H_ */"
		end
		runtime = runtimes[device[2].to_sym].add_runtime rt_obj
		runtime.devices << Device.new(device[1], vendor)

	end
end

runtimes.each do |platform, devs|
	puts "#{platform} has #{devs.size} devices!"
end

runtimes.each do |platform_name, platform|
	id = 1
	platform.each do |runtime|
		runtime_dir = "#{platform_name}/#{id}/"
		File.makedirs "#{RUNTIME_DIR}/#{runtime_dir}"
		File.open("#{RUNTIME_DIR}/#{runtime_dir}/devices.txt", 'w') do |devices|  
			runtime.devices.each do |device|
				File.open("#{VENDOR_DIR}/#{device.vendor}/#{device.name}/runtime.txt", 'w') do |runtime_txt|
					runtime_txt.puts "profiles\\runtimes\\#{platform_name}\\#{id}"
				end
			
				devices.puts "#{device.vendor}\\#{device.name}"
			end
		end
		
		write_config_h(runtime, "#{RUNTIME_DIR}/#{runtime_dir}/config.h", RELEVANT_DEFINES[platform_name.to_sym], ['PHONE_RELEASE'])
		write_config_h(runtime, "#{RUNTIME_DIR}/#{runtime_dir}/configD.h", RELEVANT_DEFINES[platform_name.to_sym], ['PUBLIC_DEBUG'])
		
		cwd = Dir.pwd
		Dir.chdir "../RuntimeBuilder/"
		system ("ruby RuntimeBuilder.rb Settings.rb #{platform_name} ../ProfileConverter/profiles/runtimes/#{runtime_dir}")
		Dir.chdir cwd
				
		id = id + 1
	end
end

#	runtimes[:JavaME].each do |device|
#		puts "\ndevice:"
#		device.caps.each do |define, value|
#			puts "#{define} = #{value}" 
#		end
#	end
