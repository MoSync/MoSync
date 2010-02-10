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

require 'fileutils'
require 'sqlite3'
require 'set'
#require 'win32/process'

# Lower priority of this process, so we don't hog the entire computer.
#Process.setpriority(Process::PRIO_PROCESS, 0, Process.getpriority(Process::PRIO_PROCESS, 0) - 1)

RELEVANT_CAPS = [
	'ScreenSize_x',
	'ScreenSize_y',
	'IconSize_x',
	'IconSize_y',
	'BitsPerPixel',
# disabled these... 
#	'StorageSize',
#	'HeapSize',
#	'MaxJarSize',
	'JavaPackage',
	'OS',
	'Packager',
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
	:s60v5 => [],
	:sp2003 => [],
	:wm5 => [],
	:wm6 => [],
	:wm6pro => [],	
	:moblin => [],
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
		'MaxJarSize',
		'Packager',
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


filename = "profiles.db"
sql_filename = "profiles.sql"

output_root = "profiles/"
build_root = "../ProfileConverter/"
dest_dir = ARGV[0]
if dest_dir.class != NilClass
	output_root = "#{dest_dir}/"
	build_root = ""
end

if(ARGV[0])
	REBUILD_DATABASE = (ARGV[0] != 'n')
else
	REBUILD_DATABASE = true
end

puts "rootdir :  #{output_root}"

bitmap = {}

runtimes = {
	:wm5    => [],
	:wm6    => [],
	:wm6pro => [],	
	:sp2003 => [],
	:s60v2  => [],
	:s60v3  => [],
	:s60v5  => [],
	:JavaME => [],
	:moblin => [],
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
		return gsub(/[^a-zA-Z0-9_]/, "_").upcase();
	end
end

if !File.exist? sql_filename
	puts "FATAL ERROR!! File '#{sql_filename} missing. Unable to build runtimes."
	return
end

if REBUILD_DATABASE
# We start from scratch
if File.exist? filename
	File.delete filename
end

# Create database file
db = SQLite3::Database.new( filename )

# Create database from SQL file
puts "Create database from SQL file"
File.open(sql_filename, "r") do |infile|
	while (line = infile.gets)
		if(line.length != 1)
			if !line.empty?
				db.execute line
			end
		end	
	end
end
else
	puts "Reading database: #{filename}"
	db = SQLite3::Database.new(filename)
end

puts "Generating all capability permutations!"

device_query = db.prepare("SELECT capvalue.value from capvalue INNER JOIN" <<
			   " devicecapvalue ON devicecapvalue.capvalue=capvalue.id INNER JOIN" <<
			   " cap ON devicecapvalue.cap=cap.id AND cap.name=? AND" <<
			   " devicecapvalue.device=?");
VENDOR_DIR = "#{output_root}vendors"
RUNTIME_DIR = "#{output_root}runtimes"
FileUtils.mkdir_p VENDOR_DIR
FileUtils.mkdir_p RUNTIME_DIR
definitions = {}

db.execute( "select name from vendor" ) do |vendor|
	# fix to exclude a lot of entries which is not real devices
	vendor = vendor[0]
	if (vendor.eql?("Generic") || vendor.eql?("Native") || vendor.eql?("BlackBerry"))
		next
	end
	
	puts vendor
	FileUtils.mkdir_p "#{VENDOR_DIR}/#{vendor}"
	
	icon_path = "icons/#{vendor.downcase}Icon.png"
	if File.exist? icon_path
		FileUtils.copy_file( icon_path, "#{VENDOR_DIR}/#{vendor}/icon.png")
	end
	
	db.execute("SELECT device.id, device.name, platform.name FROM device, vendor " <<
		"INNER JOIN platformversion ON device.platformversion = platformversion.id " <<
		"INNER JOIN platform ON platformversion.platform = platform.id " <<
		"WHERE vendor.id=device.vendor AND vendor.name=\"#{vendor}\"") do |device|
		
		# if the runtime support isn't implemented we just go to the next device.
		if(!runtimes[device[2].to_sym]) 
			next
		end

		rt_obj = Runtime.new(device[2].to_s)
		seen_defines = []
		device_path = "#{VENDOR_DIR}/#{vendor}/#{device[1]}"
		FileUtils.mkdir_p device_path
		File.open("#{device_path}/maprofile.h", 'w') do |profile|  
			profile.puts "#ifndef _MSAB_PROF_H_\n#define _MSAB_PROF_H_"
			profile.puts
			profile.puts "#define MA_PROF_STRING_VENDOR \"#{vendor}\""
			profile.puts "#define MA_PROF_STRING_DEVICE \"#{device[1]}\""
			profile.puts "#define MA_PROF_STRING_PLATFORM \"#{device[2]}\""
			profile.puts "#define MA_PROF_VENDOR_#{vendor.format}"
			profile.puts "#define MA_PROF_DEVICE_#{device[1].format}"
			profile.puts
			RELEVANT_CAPS.each do |cap|
				device_query.bind_params(cap.to_s, device[0].to_s);
				device_query.execute(
				) do |value|
					if(CAP_TYPES[:constant].include? cap)
						value.each do |v|
							v = v[0]
							def_name = "MA_PROF_CONST_#{cap.format}"
							rt_obj.caps[def_name] = "#{v.upcase}"
							def_str = "#define #{def_name} #{v.format}"
						
							if(def_name == "MA_PROF_CONST_STORAGESIZE")
								definitions[def_name] = "MA_PROF_CONST_STORAGESIZE,StorageSize,bytes"
							elsif(def_name == "MA_PROF_CONST_SCREENSIZE_Y")
								definitions[def_name] = "MA_PROF_CONST_SCREENSIZE_Y,ScreenSize/Y,pixels"
							elsif(def_name == "MA_PROF_CONST_SCREENSIZE_X")
								definitions[def_name] = "MA_PROF_CONST_SCREENSIZE_X,ScreenSize/X,pixels"
							elsif(def_name == "MA_PROF_CONST_MAXJARSIZE")
								definitions[def_name] = "MA_PROF_CONST_MAXJARSIZE,MaxJarSize,bytes"
							elsif(def_name == "MA_PROF_CONST_ICONSIZE_Y")
								definitions[def_name] = "MA_PROF_CONST_ICONSIZE_Y,IconSize/Y,pixels"
							elsif(def_name == "MA_PROF_CONST_ICONSIZE_X")
								definitions[def_name] = "MA_PROF_CONST_ICONSIZE_X,IconSize/X,pixels"
							elsif(def_name == "MA_PROF_CONST_HEAPSIZE")
								definitions[def_name] = "MA_PROF_CONST_HEAPSIZE,HeapSize,bytes"
							elsif(def_name == "MA_PROF_CONST_BITSPERPIXEL")
								definitions[def_name] = "MA_PROF_CONST_BITSPERPIXEL,BitsPerPixel,bits"
							else
								definitions["#{def_name}_#{v.format}"] = "#{def_name},#{cap}/#{v}"
							end
							
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
							v = v[0]
							def_name = "MA_PROF_SUPPORT_#{cap.format}"
							if(!(seen_defines.include? def_name)) then
								if(def_name !=  "MA_PROF_SUPPORT_JAVAPACKAGE") then
									seen_defines << def_name
								end
								rt_obj.caps["#{def_name}_#{v.format}"] = "TRUE";
								def_str = "#define #{def_name}_#{v.format}"
								definitions["#{def_name}_#{v.format}"] = "#{def_name}_#{v.format},#{cap}/#{v}"
								profile.puts def_str
							end
						end
					elsif(cap == "Bugs")
						value.each do |v|
							v = v[0]
							def_name = "MA_PROF_BUG_#{v.format}"
							definitions[def_name] = "#{def_name},#{cap}/#{v}"
							rt_obj.caps[def_name] = "TRUE";
							profile.puts "#define #{def_name}"
						end
					elsif(cap == "Features")
						value.each do |v|
							v = v[0]
							#puts v.to_s
							if v == "hasPointerEvents"
								definitions["MA_PROF_SUPPORT_STYLUS"] = "MA_PROF_SUPPORT_STYLUS,Support/Stylus"
								rt_obj.caps["MA_PROF_SUPPORT_STYLUS"] = "TRUE";
								def_str = "#define MA_PROF_SUPPORT_STYLUS"
								profile.puts def_str
							elsif v == "isVirtual"
								## should not generate this profile......
							end
						end
					elsif(cap == "JavaConfiguration")
						value.each do |v|
							v = v[0]
							if (v == "CLDC/1.0" ||
								v == "CLDC/1.0.4")
								rt_obj.caps["MA_PROF_SUPPORT_CLDC_10"] = "TRUE";
								definitions["MA_PROF_SUPPORT_CLDC_10"] = "MA_PROF_SUPPORT_CLDC_10,Support/Cldc1.0"
									
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

File.open("#{VENDOR_DIR}/definitions.txt", 'w') do |def_file|
	definitions.each do |line|
		def_file.puts line[1]
	end
end	

runtimes.each do |platform, devs|
	puts "#{platform} has #{devs.size} devices!"
end

runtimes.each do |platform_name, platform|
	id = 1
	platform.each do |runtime|
		runtime_dir = "#{platform_name}/#{id}/"
		FileUtils.mkdir_p "#{RUNTIME_DIR}/#{runtime_dir}"
		File.open("#{RUNTIME_DIR}/#{runtime_dir}/devices.txt", 'w') do |devices|  
			runtime.devices.each do |device|
				File.open("#{VENDOR_DIR}/#{device.vendor}/#{device.name}/runtime.txt", 'w') do |runtime_txt|
					runtime_txt.puts "profiles\\runtimes\\#{platform_name}\\#{id}"
				end
			
				devices.puts "#{device.vendor}\\#{device.name}"
			end
		end
		
		release_defines = ['PHONE_RELEASE', 'MOSYNC_COMMERCIAL']
		if(platform_name == :sp2003 || platform_name == :wm5) # || platform_name == :s60v2 || platform_name == :s60v3)
			release_defines << "USE_ARM_RECOMPILER"
		end
		if(platform_name == :s60v3)
			release_defines << "SUPPORT_MOSYNC_SERVER"
		end
		write_config_h(runtime, "#{RUNTIME_DIR}/#{runtime_dir}/config.h", RELEVANT_DEFINES[platform_name.to_sym], release_defines)
		
		debug_defines = release_defines + ['PUBLIC_DEBUG', 'LOGGING_ENABLED']
		write_config_h(runtime, "#{RUNTIME_DIR}/#{runtime_dir}/configD.h", RELEVANT_DEFINES[platform_name.to_sym], debug_defines)
		
		cwd = Dir.pwd
		Dir.chdir "../RuntimeBuilder/"
		
		puts "platform dir : #{build_root}#{RUNTIME_DIR}/#{runtime_dir}"
		
		if(platform_name == :JavaME && (runtime.caps.has_key? "MA_PROF_SUPPORT_CLDC_10"))
			cmd = "ruby RuntimeBuilder.rb Settings.rb JavaMEcldc10 #{build_root}#{RUNTIME_DIR}/#{runtime_dir}"
		else
			cmd = "ruby RuntimeBuilder.rb Settings.rb #{platform_name} #{build_root}#{RUNTIME_DIR}/#{runtime_dir}"
		end

		puts(cmd)
		success = system(cmd)
		
		Dir.chdir cwd

		if(!success)
			puts "Building one of the runtimes failed."
			exit(1)
		end
		
		id = id + 1
	end
end
exit(0)

#	runtimes[:JavaME].each do |device|
#		puts "\ndevice:"
#		device.caps.each do |define, value|
#			puts "#{define} = #{value}" 
#		end
#	end
