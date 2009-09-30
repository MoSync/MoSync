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

	def build_wince(runtime_dir, mode, version) 
		if (mode=="debug") 
			debug = "D"
		else
			debug = ""
		end
		
		# Copy the old config_platform.h file and copy the one from the runtime_dir to the source location
		configFile = $settings[:wince_source] + "config_platform.h"	
		configFileExist = File.exist? configFile
	
		if configFileExist == true
			configBakFile = $settings[:wince_source] + "config_platform.h.bak"
			if File.exist? configBakFile
				File.delete configBakFile
			end
			File.copy(configFile, configBakFile)
		end
		
		File.copy(runtime_dir + "config" + debug + ".h", configFile)
		
		if (version == "sp2003")
			configuration = "Smartphone 2003 (ARMV4)"
		else
			configuration = "Windows Mobile 5.0 Smartphone SDK (ARMV4I)"
		end
		
		system("build_wince.bat \"" + $settings[:wince_source] + "\" \"Release|" + configuration + "\"")
		
		File.copy($settings[:wince_source] + configuration + "/Release/MoRE-winmobile.exe",
		runtime_dir + "MoRE-winmobile" + debug + ".exe")
		
		# Restore config_platform.h
		if configFileExist == true
			File.copy(configBakFile, configFile)
			File.delete configBakFile
		end
	end

	def sp2003(runtime_dir)
		build_wince(runtime_dir, "release", "sp2003")
		build_wince(runtime_dir, "debug", "sp2003")	
	end

	def wm5(runtime_dir)
		build_wince(runtime_dir, "release", "wm5")
		build_wince(runtime_dir, "debug", "wm5")	
	end
end
