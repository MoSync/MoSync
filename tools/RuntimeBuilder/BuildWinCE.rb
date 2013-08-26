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

require 'fileutils'

class RuntimeBuilder 

	def build_wince(runtime_dir, mode, version) 
		debug = (mode == "debug") ? "D" : "";
	
		src_folder = "#{$SETTINGS[:wince_source]}MoRE-winmobile/"
		puts "src folder : " + src_folder;
	
		config_file_dest = "#{$SETTINGS[:wince_source]}config_platform.h"	
		config_file_src = "#{runtime_dir}config#{debug}.h"
	
		backup_file(config_file_dest)
		FileUtils.copy_file(config_file_src, config_file_dest)
		
		if (version == "sp2003")
			configuration = "Smartphone 2003 (ARMV4)"
		elsif (version == "wm5")
			configuration = "Windows Mobile 5.0 Smartphone SDK (ARMV4I)"
		elsif (version == "wm6")
			configuration = "Windows Mobile 6 Standard SDK (ARMV4I)"
		elsif (version == "wm6pro")
			configuration = "Windows Mobile 6 Professional SDK (ARMV4I)"
		end
		
		sh("build_wince.bat \"#{src_folder}\" \"Release|#{configuration}\"")
		
		exe_file_src = "#{src_folder}#{configuration}/Release/MoRE-winmobile.exe"
		exe_file_dest = "#{runtime_dir}MoRE-winmobile#{debug}.exe" 
		
		# Restore config_platform.h
		revert_backupped_file(config_file_dest)
		
		if File.exist? exe_file_src
			FileUtils.copy_file(exe_file_src, exe_file_dest)
			puts "\nFINISHED! - #{exe_file_dest} was succesfully generated!\n\n"
			return 0
		else
			puts "\nFATAL ERROR! - No exe file built, check previous output for errors!\n\n"
			return 1
		end
	end

	def sp2003(runtime_dir, mode)
		return build_wince(runtime_dir, mode, "sp2003")
	end

	def wm5(runtime_dir, mode)
		return build_wince(runtime_dir, mode, "wm5")
	end

	def wm6(runtime_dir, mode)
		return build_wince(runtime_dir, mode, "wm6")
	end

	def wm6pro(runtime_dir, mode)
		return build_wince(runtime_dir, mode, "wm6pro")
	end
end
