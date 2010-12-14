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

require 'rubygems'	# needed for sqlite3 on some(unknown) versions of Ruby
require 'fileutils'
require 'sqlite3'
require 'set'

# config

DB_FILENAME = "profiles.db"
SQL_FILENAME = "profiles.sql"

# command-line arguments
output_root = "profiles/"
build_root = "../ProfileConverter/"
gBuildRuntimes = true
gDoSanityCheck = false

i=0
while(i<ARGV.length)
	arg = ARGV[i]
	if(arg == '-nobuild')
		gBuildRuntimes = false
	elsif(arg == '-dst')
		i += 1
		output_root = "#{ARGV[i]}/"
		build_root = ''
	elsif(arg == '-sc')
		gDoSanityCheck = true
	else
		puts "Unknown argument: '#{arg}'"
		exit(1)
	end
	i += 1
end

OUTPUT_ROOT = output_root
BUILD_ROOT = build_root
VENDOR_DIR = "#{OUTPUT_ROOT}vendors"
RUNTIME_DIR = "#{OUTPUT_ROOT}runtimes"


# data types
class PlatformReferrer
	def initialize(name, platform_index)
		if(name == "")
			@name = ""
		else
			@name = name.to_sym
		end
		@platform = PLATFORM[platform_index]
	end
	attr_reader(:name, :platform)
end

class Device
	def initialize(name, vendor_index, platformversion_index)
		if(name == "")
			@name = ""
		else
			@name = name.to_sym
		end
		@vendor = vendor_index.to_i
		@platformversion = PLATFORMVERSION[platformversion_index.to_i]
		@caps = Hash.new
	end
	attr_reader(:name, :vendor, :platformversion)
	attr_accessor(:caps)	#Hash(Symbol, Array(String))
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


# const data
RELEVANT_CAPS = [
	:ScreenSize_x,
	:ScreenSize_y,
	:IconSize_x,
	:IconSize_y,
	:BitsPerPixel,
# disabled these... 
#	:StorageSize,
#	:HeapSize,
#	:MaxJarSize,
	:JavaPackage,
	:OS,
	:Packager,
	:Features,
	:Bugs,
	:JavaConfiguration,
	:CameraResolution_x,
	:CameraResolution_y,
	:properties,
]

RELEVANT_DEFINES = {
	:JavaME => [
		'MA_PROF_SUPPORT_JAVAPACKAGE_BLUETOOTH',
		'MA_PROF_BUG_BACKLIGHTFLASHES',
		'MA_PROF_BUG_MICROEMU',
		'MA_PROF_SUPPORT_STYLUS',
		'MA_PROF_SUPPORT_CLDC_10',
		'MA_PROF_SUPPORT_JAVAPACKAGE_LOCATIONAPI',
		'MA_PROF_SUPPORT_JAVAPACKAGE_WMAPI',
		'MA_PROF_SUPPORT_CAMERA',
		'MA_PROF_SUPPORT_JAVAPACKAGE_FILECONNECTION',
		'MA_PROF_SUPPORT_JAVAPACKAGE_PIMAPI',
		'MA_PROF_SUPPORT_JAVAPACKAGE_MMAPI',
	],
	
	:s60v2 => ['MA_PROF_SUPPORT_FRAMEBUFFER_32BIT'],
	:s60v3 => [],
	:s60v5 => [],
	:sp2003 => [],
	:wm5 => [],
	:wm6 => [],
	:wm6pro => [],
	:moblin => [],
	:android_3 => [],
	:android_4 => [],
	:android_7 => [],
	:iphoneos => [],
}

CAP_TYPES = {
	:constant => [
		:ScreenSize_x,
		:ScreenSize_y,
		:IconSize_x,
		:IconSize_y,
		:BitsPerPixel,
		:StorageSize,
		:HeapSize,
		:MaxJarSize,
		:Packager,
		:CameraResolution_x,
		:CameraResolution_y,
	],
	:supports => [
		:JavaPackage,
		:OS,
	]
}


# static data
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
	:android_3 => [],
	:android_4 => [],
	:android_7 => [],
	:iphoneos => [],
}


# helpers
def assert(truth)
	if(!truth)
		throw(42)
	end
end

def assert_eq(a, b)
	if(a != b)
		puts "Assertion failed: " + a.to_s + " = " + b.to_s
		throw(42)
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

# Returns an array, with [0] == nil.
# The block takes the row as a parameter and shall return an array element.
def read_table(db, name, &block)
	puts "Reading #{name}"
	res = db.execute("select * from #{name}")
	puts res.length
	arr = Array.new(res.length + 1)
	res.each_with_index do |row, i|
		assert_eq(row[0].to_i, i+1)
		arr[i+1] = block.call(row)
	end
	return arr
end

def read_simple_table(db, name)
	read_table(db, name) do |row| row[1] end
end

class Array
	def add_runtime(d)
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


# main code
if(File.exist?(DB_FILENAME))
	#puts(DB_FILENAME + " exists.")
	#puts DB_FILENAME + ": " + File.mtime(DB_FILENAME).to_s
	#puts SQL_FILENAME + ": " + File.mtime(SQL_FILENAME).to_s
	REBUILD_DATABASE = File.mtime(DB_FILENAME) < File.mtime(SQL_FILENAME)
	if(REBUILD_DATABASE)
		puts "Database is out of date."
	else
		puts "Database is up to date."
	end
else
	REBUILD_DATABASE = true
end

if REBUILD_DATABASE
	# We start from scratch
	if File.exist? DB_FILENAME
		File.delete DB_FILENAME
	else
		puts "Database didn't exist."
	end

	# Create database file
	db = SQLite3::Database.new( DB_FILENAME )

	# Create database from SQL file
	puts "Create database from SQL file..."
	File.open(SQL_FILENAME, "r") do |infile|
		while (line = infile.gets)
			if(line.length != 1)
				if !line.empty?
					db.execute line
				end
			end
		end
	end
else
	puts "Reading database: #{DB_FILENAME}"
	db = SQLite3::Database.new(DB_FILENAME)
end

VENDOR = read_simple_table(db, "vendor")
CAPVALUE = read_simple_table(db, "capvalue")
PLATFORM = read_table(db, "platform") do |row| row[1].to_sym end
#MOSYNCCAP = read_string_table(db, "mosynccap")
#PLATFORM.each do |name|
#	puts name.inspect
#end
PLATFORMVERSION = read_table(db, "platformversion") do |row| PlatformReferrer.new(row[1], row[2].to_i) end
DEVICE = read_table(db, "device") do |row| Device.new(row[1], row[2], row[3]) end
CAP = read_table(db, "cap") do |row| PlatformReferrer.new(row[1], row[2].to_i) end

name = "devicecapvalue"
puts "Reading #{name}"
db.execute("select * from #{name}") do |row|
	devId = row[1].to_i
	cap = CAP[row[2].to_i].name
	value = CAPVALUE[row[3].to_i]
	if(!DEVICE[devId].caps.has_key?(cap))
		DEVICE[devId].caps[cap] = [value]
	else
		DEVICE[devId].caps[cap] << value
	end
end





puts "Generating all capability permutations!"

FileUtils.mkdir_p VENDOR_DIR
FileUtils.mkdir_p RUNTIME_DIR
definitions = {}

def skipVendor?(vendor)
	return vendor.eql?("Generic") || vendor.eql?("Native") || vendor.eql?("BlackBerry")
end

puts "Handling vendors..."
VENDOR.each_with_index do |vendor, index|
	next if(index == 0)
	# fix to exclude a lot of entries which is not real devices
	if(skipVendor?(vendor))
		next
	end
	
	#puts vendor
	FileUtils.mkdir_p "#{VENDOR_DIR}/#{vendor}"
	
	icon_path = "icons/#{vendor.downcase}Icon.png"
	if File.exist? icon_path
		FileUtils.copy_file( icon_path, "#{VENDOR_DIR}/#{vendor}/icon.png")
	end
end

puts "Handling devices..."
devicesWithErrors = 0
DEVICE.each_with_index do |device, index|
	next if(index == 0)
	
	# if the runtime support isn't implemented we just go to the next device.
	if(!runtimes[device.platformversion.platform])
		next
	end
	
	vendor = VENDOR[device.vendor]
	if(skipVendor?(vendor))
		next
	end
	#puts "#{vendor}/#{device.name}"
	rt_obj = Runtime.new(device.platformversion.platform)
	seen_defines = []
	device_path = "#{VENDOR_DIR}/#{vendor}/#{device.name}"
	FileUtils.mkdir_p device_path
	hasScreenSizeX = false
	hasScreenSizeY = false
	File.open("#{device_path}/maprofile.h", 'w') do |profile|
		profile.puts "#ifndef _MSAB_PROF_H_\n#define _MSAB_PROF_H_"
		profile.puts
		profile.puts "#define MA_PROF_STRING_VENDOR \"#{vendor}\""
		profile.puts "#define MA_PROF_STRING_DEVICE \"#{device.name}\""
		profile.puts "#define MA_PROF_STRING_PLATFORM \"#{device.platformversion.platform}\""
		profile.puts "#define MA_PROF_STRING_PLATFORM_#{device.platformversion.platform.to_s.upcase}"
		profile.puts "#define MA_PROF_VENDOR_#{vendor.to_s.format}"
		profile.puts "#define MA_PROF_DEVICE_#{device.name.to_s.format}"
		if(device.platformversion.platform == :iphoneos)
			profile.puts "#define MA_PROF_OUTPUT_CPP"
		end
		profile.puts
		RELEVANT_CAPS.each do |cap|
			if(device.caps.has_key?(cap))
				value = device.caps[cap]
				if(CAP_TYPES[:constant].include? cap)
					value.each do |v|
						def_name = "MA_PROF_CONST_#{cap.to_s.format}"
						rt_obj.caps[def_name] = "#{v.upcase}"
						def_str = "#define #{def_name} #{v.format}"
						
						if(def_name == "MA_PROF_CONST_STORAGESIZE")
							definitions[def_name] = "MA_PROF_CONST_STORAGESIZE,StorageSize,bytes"
						elsif(def_name == "MA_PROF_CONST_SCREENSIZE_Y")
							raise "#{vendor}/#{device.name}: broken #{def_name}: #{value}" if(value[0].to_i > 10000)
							hasScreenSizeY = true
							definitions[def_name] = "MA_PROF_CONST_SCREENSIZE_Y,ScreenSize/Y,pixels"
						elsif(def_name == "MA_PROF_CONST_SCREENSIZE_X")
							raise "#{vendor}/#{device.name}: broken #{def_name}: #{value}" if(value[0].to_i > 10000)
							hasScreenSizeX = true
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
						
						#puts "#{rt_obj.platform} #{def_name}"
						
						if(rt_obj.platform==:s60v2 && def_name == "MA_PROF_CONST_BITSPERPIXEL")
							#puts "s60v2 MA_PROF_CONST_BITSPERPIXEL: " + rt_obj.caps[def_name]
							if(rt_obj.caps[def_name].to_i > 16)
								rt_obj.caps["MA_PROF_SUPPORT_FRAMEBUFFER_32BIT"] = "TRUE"
								profile.puts "#define MA_PROF_SUPPORT_FRAMEBUFFER_32BIT"
							end
						end
						
						if(!(seen_defines.include? def_name)) then
							seen_defines << def_name
							profile.puts def_str
						end
					end	#value.each
				elsif(CAP_TYPES[:supports].include? cap)
					value.each do |v|
						def_name = "MA_PROF_SUPPORT_#{cap.to_s.format}"
						if(!(seen_defines.include? def_name)) then
							if(def_name != "MA_PROF_SUPPORT_JAVAPACKAGE") then
								seen_defines << def_name
							end
							rt_obj.caps["#{def_name}_#{v.format}"] = "TRUE";
							def_str = "#define #{def_name}_#{v.format}"
							definitions["#{def_name}_#{v.format}"] = "#{def_name}_#{v.format},#{cap}/#{v}"
							profile.puts def_str
						end
					end	#value.each
				elsif(cap == :Bugs)
					value.each do |v|
						def_name = "MA_PROF_BUG_#{v.format}"
						definitions[def_name] = "#{def_name},#{cap}/#{v}"
						rt_obj.caps[def_name] = "TRUE";
						profile.puts "#define #{def_name}"
					end
				elsif(cap == :Features)
					value.each do |v|
						#puts v.to_s
						if v == "hasPointerEvents"
							definitions["MA_PROF_SUPPORT_STYLUS"] = "MA_PROF_SUPPORT_STYLUS,Support/Stylus"
							rt_obj.caps["MA_PROF_SUPPORT_STYLUS"] = "TRUE";
							def_str = "#define MA_PROF_SUPPORT_STYLUS"
							profile.puts def_str
						elsif v == "hasCamera"
							#code dupe
							def_name = "MA_PROF_SUPPORT_CAMERA"
							if(!(seen_defines.include? def_name))
								definitions[def_name] = "#{def_name},Support/Camera"
								rt_obj.caps[def_name] = "TRUE";
								def_str = "#define #{def_name}"
								profile.puts def_str
							end
						elsif v == "isVirtual"
							## should not generate this profile......
						end
					end	#value.each
				elsif(cap == :properties)
					value.each do |v|
						if(v == "camera.orientations")
							#code dupe
							def_name = "MA_PROF_SUPPORT_CAMERA"
							if(!(seen_defines.include? def_name))
								definitions[def_name] = "#{def_name},Support/Camera"
								rt_obj.caps[def_name] = "TRUE";
								def_str = "#define #{def_name}"
								profile.puts def_str
							end
						end
					end	#value.each
				elsif(cap == :JavaConfiguration)
					value.each do |v|
						if (v == "CLDC/1.0" ||
							v == "CLDC/1.0.4")
							rt_obj.caps["MA_PROF_SUPPORT_CLDC_10"] = "TRUE";
							definitions["MA_PROF_SUPPORT_CLDC_10"] = "MA_PROF_SUPPORT_CLDC_10,Support/Cldc1.0"
							
							def_str = "#define MA_PROF_SUPPORT_CLDC_10"
							profile.puts def_str
						end
					end	#value.each
				end	#(cap == :JavaConfiguration)
			end	#device.caps.has_key?
		end	#RELEVANT_CAPS.each
		profile.puts "\n#endif /* _MSAB_PROF_H_ */"
	end	#File.open
	
	if(gDoSanityCheck && !(hasScreenSizeX && hasScreenSizeY))
		puts "Missing screen size: #{vendor}/#{device.name}"
		devicesWithErrors += 1
	end
	
	runtime = runtimes[device.platformversion.platform].add_runtime rt_obj
	runtime.devices << device
end	#DEVICE.each

if(devicesWithErrors > 0)
	raise "Device profile sanity check failed on #{devicesWithErrors} devices"
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
				vendor = VENDOR[device.vendor]
				File.open("#{VENDOR_DIR}/#{vendor}/#{device.name}/runtime.txt", 'w') do |runtime_txt|
					runtime_txt.puts "profiles\\runtimes\\#{platform_name}\\#{id}"
				end
			
				devices.puts "#{vendor}\\#{device.name}"
			end
		end
		
		release_defines = ['PHONE_RELEASE', 'MOSYNC_COMMERCIAL']
		if(platform_name == :sp2003 || platform_name == :wm5 || platform_name == :wm6 || platform_name == :wm6pro || platform_name == :s60v3 || platform_name == :s60v5)
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
		
		puts "platform dir : #{BUILD_ROOT}#{RUNTIME_DIR}/#{runtime_dir}"
		
		if(platform_name == :JavaME && (runtime.caps.has_key? "MA_PROF_SUPPORT_CLDC_10"))
			cmd = "ruby RuntimeBuilder.rb Settings.rb JavaMEcldc10 #{BUILD_ROOT}#{RUNTIME_DIR}/#{runtime_dir}"
		else
			cmd = "ruby RuntimeBuilder.rb Settings.rb #{platform_name} #{BUILD_ROOT}#{RUNTIME_DIR}/#{runtime_dir}"
		end
		
		puts(cmd)
		if(gBuildRuntimes)
			success = system(cmd)
		else
			success = true
		end
		
		Dir.chdir cwd
		
		if(!success)
			puts "Building one of the runtimes failed."
			exit(1)
		end
		
		id = id + 1
	end
end
exit(0)
