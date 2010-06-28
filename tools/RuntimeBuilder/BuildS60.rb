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

class RuntimeBuilder
	
	def build_symbian(runtime_dir, mode, version)
		debug = (mode == "debug") ? "D" : "";
	
		config_file_dest = $SETTINGS[:symbian_source] + "inc/config_platform.h"
		config_file_src = "#{runtime_dir}config#{debug}.h"
		
		puts "Using config file: #{config_file_src}"
	
		backup_file(config_file_dest)
		FileUtils.copy_file(config_file_src, config_file_dest)
		
		if version == "s60v2"
			group_dir = "#{$SETTINGS[:symbian_source]}group"
			default = "@S60_2nd_FP3:com.nokia.series60"
			symbian_system = "armi"
		elsif version == "s60v3"
			group_dir = "#{$SETTINGS[:symbian_source]}group-ed3"
			default = "@S60_3rd_FP1:com.nokia.s60"
			symbian_system = "gcce"
			# symbian_system = "armv5"
		elsif version == "s60v5"
			group_dir = "#{$SETTINGS[:symbian_source]}group-ed3"
			symbian_system = "gcce"
			# symbian_system = "armv5"
		else
			puts "FATAL ERROR! - Unknown version (#{version})."
			return 1
		end
		
		# store the current dir and change to the group dir
		cwd = Dir.pwd
		Dir.chdir group_dir
		
		# call the build functions
		if version == "s60v5"
			cmd = "#{cwd}/build_s60v5.bat #{$SETTINGS[:s60v5_compiler]}"
			puts cmd
			system(cmd)
		else
			system("devices -setdefault #{default}")
			system("bldmake bldfiles");
			system("call abld clean #{symbian_system} urel");
			system("call abld build #{symbian_system} urel");
		end
		
		# revert initial config file
		revert_backupped_file(config_file_dest)
		
		# go back to the initial directory
		Dir.chdir cwd
		
		if version == "s60v2"
			epoc_dir = "/Symbian/8.1a/S60_2nd_FP3/epoc32/"
			sis_dir = "#{$SETTINGS[:symbian_source]}sis/"
			app_file = "#{epoc_dir}release/#{symbian_system}/urel/MoSync.app"
			
			if(!File.exist? app_file) 
				puts "FATAL ERROR! - S60 2nd build failed."
				return 1
			end
			
			# Copy all the generated files to it's runtime folder
			FileUtils.copy_file(app_file, "#{runtime_dir}MoSync#{debug}.app")
			FileUtils.copy_file("#{epoc_dir}data/z/system/apps/MoSync/MoSync.rsc", "#{runtime_dir}/MoSync.rsc")
			FileUtils.copy_file("#{epoc_dir}data/z/system/apps/MoSync/MoSync_caption.rsc", "#{runtime_dir}MoSync_caption.rsc")
			FileUtils.copy_file("#{sis_dir}MoSync-template.pkg", "#{runtime_dir}MoSync-template.pkg")
			
			puts "\nFINISHED! - #{runtime_dir}MoSync#{debug}.app, and other runtime files was succesfully generated!\n\n"
			return 0
		elsif version == "s60v3"
			epoc_dir = "/Symbian/9.2/S60_3rd_FP1/Epoc32/"
			sis_dir = "#{$SETTINGS[:symbian_source]}sis-ed3/"
			exe_file = "#{epoc_dir}release/#{symbian_system}/urel/MoSync2.exe"
			
			if(!File.exist? exe_file) 
				puts "FATAL ERROR! - S60 3rd build failed."
				return 1
			end
			
			# Copy all the generated files to it's runtime folder
			FileUtils.copy_file(exe_file, "#{runtime_dir}MoSync#{debug}.exe")
			FileUtils.copy_file("#{epoc_dir}data/z/resource/apps/MoSync_3rd.RSC", "#{runtime_dir}MoSync.RSC")
			FileUtils.copy_file("#{epoc_dir}data/z/private/10003a3f/import/apps/MoSync_reg.RSC", "#{runtime_dir}MoSync_reg.RSC")
			FileUtils.copy_file("#{sis_dir}MoSync-template.pkg", "#{runtime_dir}MoSync-template.pkg")
			FileUtils.copy_file("#{$SETTINGS[:symbian_source]}/server/MoSyncServer_Express_Signed.sis", "#{runtime_dir}MoSyncServer.sis")
			
			puts "\nFINISHED! - #{runtime_dir}MoSync#{debug}.exe, and other runtime files was succesfully generated!\n\n"
			return 0
		elsif version == "s60v5"
			epoc_dir = "/Symbian/s60v5/S60_5th_Edition_SDK_v1.0/Epoc32/"
			sis_dir = "#{$SETTINGS[:symbian_source]}sis-ed5/"
			exe_file = "#{epoc_dir}release/#{symbian_system}/urel/MoSync2.exe"
			
			if(!File.exist? exe_file) 
				puts "FATAL ERROR! - S60 5th build failed."
				return 1
			end
			
			# Copy all the generated files to it's runtime folder
			FileUtils.copy_file(exe_file, "#{runtime_dir}MoSync#{debug}.exe")
			FileUtils.copy_file("#{epoc_dir}data/z/resource/apps/MoSync_3rd.RSC", "#{runtime_dir}MoSync.RSC")
			FileUtils.copy_file("#{epoc_dir}data/z/private/10003a3f/import/apps/MoSync_reg.RSC", "#{runtime_dir}MoSync_reg.RSC")
			FileUtils.copy_file("#{sis_dir}MoSync-template.pkg", "#{runtime_dir}MoSync-template.pkg")
			
			puts "\nFINISHED! - #{runtime_dir}MoSync#{debug}.exe, and other runtime files was succesfully generated!\n\n"
			return 0
		else
			puts "FATAL ERROR! - Unknown version (#{version})."
			return 1
		end
	end
	
	def s60v2(runtime_dir, mode)
		return build_symbian(runtime_dir, mode, "s60v2")
	end
	
	def s60v3(runtime_dir, mode)
		return build_symbian(runtime_dir, mode, "s60v3")
	end
	
	def s60v5(runtime_dir, mode)
		return build_symbian(runtime_dir, mode, "s60v5")
	end
end
