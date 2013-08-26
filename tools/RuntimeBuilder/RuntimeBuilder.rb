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

# usage: ruby RuntimeBuilder.rb <Settings.rb file> <platform> <runtime directory (containing config.h)>

require 'fileutils'

def error(msg)
	puts "Error: #{msg}"
	raise msg
end

def sh(cmd)
	#TODO: optimize by removing the extra shell
	#the Process class should be useful.
	$stderr.puts cmd
	if(!system(cmd)) then
		error "Command failed: '#{$?}'"
	end
end

def backup_file(file)
	bak = file + ".bak"
	if File.exist? bak
		FileUtils.rm bak
	end
	
	if !File.exist? file
		return
	end
	
	FileUtils.copy_file(file, bak)
end

def revert_backupped_file(file)
	bak = file + ".bak"

	if File.exist? bak
		FileUtils.copy_file(bak, file)
		FileUtils.rm bak
	end
end

class RuntimeBuilder
	def log(str) 
		if ($SETTINGS[:verbose])
			puts str
		end
	end

	def system(str)
		log(str)
		Kernel.system(str)
	end
		
	def method_missing(method, *args) 
		puts "\nFATAL ERROR: invalid platform #{method.to_s}"
	end

	def build(platform, runtime_dir)
		modes = [
			["release", "#{runtime_dir}config.h"],
			["debug", "#{runtime_dir}configD.h"],
		]
		
		modes.each do |mode|
			if File.exist? mode[1]
				puts "Building #{mode[0]} runtime for #{platform}"
				error = send(platform, runtime_dir, mode[0])
				if(error != 0)
					exit(error)
				end
			else
				puts "WARNING: Skipping build. No config file for #{platform} #{mode[0]} runtime."
			end
		end
	end
end

if !File.exist? "Settings.rb"
	puts "Creates the Settings.rb file"
	FileUtils.copy_file("Settings.rb.example", "Settings.rb")
end

# usage example: runtimebuilder.rb Settings.rb s60v3 /mosync/profiles/runtimes/s60v3/1/

require ARGV[0]
require "./BuildJavaME.rb"
require "./BuildS60.rb"
require "./BuildWinCE.rb"
require "./BuildAndroid.rb"
require "./BuildMoblin.rb"
require "./BuildIphone.rb"

runtime_builder = RuntimeBuilder.new
runtime_builder.build(ARGV[1].downcase, ARGV[2])
