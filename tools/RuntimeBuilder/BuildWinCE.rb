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
		configFileDest = $settings[:wince_source] + "config_platform.h"	
		configFileDestExist = File.exist? configFileDest
		configFileSrc = runtime_dir + "config" + debug + ".h"
		if (File.exist? configFileSrc) == false
			puts configFileSrc + " missing."
			return
		end
	
		if configFileDestExist == true
			configBakFile = $settings[:wince_source] + "config_platform.h.bak"
			if File.exist? configBakFile
				File.delete configBakFile
			end
			File.copy(configFileDest, configBakFile)
		end
		
		File.copy(configFileSrc, configFileDest)
		
		if (version == "sp2003")
			configuration = "Smartphone 2003 (ARMV4)"
		else
			configuration = "Windows Mobile 5.0 Smartphone SDK (ARMV4I)"
		end
		
		system("build_wince.bat \"" + $settings[:wince_source] + "\" \"Release|" + configuration + "\"")
		
		exeFileSrc = $settings[:wince_source] + configuration + "/Release/MoRE-winmobile.exe"
		exeFileDest = runtime_dir + "MoRE-winmobile" + debug + ".exe" 
		
		if File.exist? exeFileSrc
			File.copy(exeFileSrc, exeFileDest)
		else
			puts "Build failed."
		end
		
		
		
		
		# Restore config_platform.h
		if configFileDestExist == true
			File.copy(configBakFile, configFileDest)
			File.delete configBakFile
		else
			File.delete configFileDest
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
