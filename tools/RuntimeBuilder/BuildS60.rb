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

	def build_symbian(runtime_dir, debug, version) 
		puts (debug=="D")? "Building debug.":"Building release."
	
		config_file_dest = $SETTINGS[:symbian_source] + "inc/config_platform.h"
		config_file_src = runtime_dir + "config" + debug + ".h"
		if !File.exist? config_file_src
			puts config_file_src + " missing."
			return
		end		
		backup_file(config_file_dest)
		File.copy(config_file_src, config_file_dest)
		
		if version == "s60v2"
			group_dir = $SETTINGS[:symbian_source] + "group"
			default = "@S60_2nd_FP3:com.nokia.series60"
			symbian_system = "armi"
		else
			group_dir = $SETTINGS[:symbian_source] + "group-ed3"
			default = "@S60_3rd_FP1:com.nokia.s60"
			symbian_system = "gcce"			
		end
		
		# store the current dir and change to the group dir
		cwd = Dir.pwd
		Dir.chdir group_dir
		
		system("devices -setdefault " + default)
		system("bldmake bldfiles");
		system("call abld clean " + symbian_system + " urel");
		system("call abld build " + symbian_system + " urel");		
		
		# go back to the initial directory
		Dir.chdir cwd
		
		if version == "s60v2"
			epoc_dir = "/Symbian/8.1a/S60_2nd_FP3/epoc32/";
			sis_dir = $SETTINGS[:symbian_source] + "sis/";
			app_file = epoc_dir + "release/armi/urel/MoSync.app"
			
			if(!File.exist? app_file) 
				puts "Build failed."
				return
			end			
			
			File.copy(app_file, runtime_dir + "MoSync" + debug + ".app")
			File.copy(epoc_dir + "data/z/system/apps/MoSync/MoSync.rsc", runtime_dir + "/MoSync.rsc")
			File.copy(epoc_dir +"data/z/system/apps/MoSync/MoSync_caption.rsc", runtime_dir + "MoSync_caption.rsc")
			File.copy(sis_dir + "MoSync-template.pkg", runtime_dir + "MoSync-template.pkg")			
		else
			epoc_dir = "/Symbian/9.2/S60_3rd_FP1/Epoc32/"
			sis_dir = $SETTINGS[:symbian_source] + "sis-ed3/"		
			exe_file = epoc_dir + "release/gcce/urel/MoSync2.exe"
			
			if(!File.exist? exe_file) 
				puts "Build failed."
				return
			end
		
			File.copy(exe_file, runtime_dir + "MoSync" + debug + ".exe")
			File.copy(epoc_dir + "data/z/resource/apps/MoSync_3rd.RSC", runtime_dir + "MoSync.RSC")
			File.copy(epoc_dir +"data/z/private/10003a3f/import/apps/MoSync_reg.RSC", runtime_dir + "MoSync_reg.RSC")
			File.copy(sis_dir + "MoSync-template.pkg", runtime_dir + "MoSync-template.pkg")
		end
		
		revert_backupped_file(config_file_dest)
	end

	def s60v2(runtime_dir)	
		build_symbian(runtime_dir, "", "s60v2")
		build_symbian(runtime_dir, "D", "s60v2")	
	end
	
	def s60v3(runtime_dir)
		build_symbian(runtime_dir, "", "s60v3")
		build_symbian(runtime_dir, "D", "s60v3")
	end
end
