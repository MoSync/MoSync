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

class RuntimeBuilder 

	def build_wince(runtime_dir, debug, version) 
		puts (debug=="D")? "Building debug.":"Building release."
		
		config_file_dest = $SETTINGS[:wince_source] + "config_platform.h"	
		config_file_src = runtime_dir + "config" + debug + ".h"
		if !File.exist? config_file_src
			puts config_file_src + " missing."
			return
		end
	
		backup_file(config_file_dest)
	
		File.copy(config_file_src, config_file_dest)
		
		if (version == "sp2003")
			configuration = "Smartphone 2003 (ARMV4)"
		else
			configuration = "Windows Mobile 5.0 Smartphone SDK (ARMV4I)"
		end
		
		system("build_wince.bat \"" + $SETTINGS[:wince_source] + "\" \"Release|" + configuration + "\"")
		
		exe_file_src = $SETTINGS[:wince_source] + configuration + "/Release/MoRE-winmobile.exe"
		exe_file_dest = runtime_dir + "MoRE-winmobile" + debug + ".exe" 
		
		if File.exist? exe_file_src
			File.copy(exe_file_src, exe_file_dest)
		else
			puts "Build failed."
		end
		
		# Restore config_platform.h
		revert_backupped_file(config_file_dest)
	end

	def sp2003(runtime_dir)
		build_wince(runtime_dir, "", "sp2003")
		build_wince(runtime_dir, "D", "sp2003")	
	end

	def wm5(runtime_dir)
		build_wince(runtime_dir, "", "wm5")
		build_wince(runtime_dir, "D", "wm5")	
	end
end
