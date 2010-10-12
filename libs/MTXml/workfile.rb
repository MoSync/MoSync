#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	def setup_native
		@LOCAL_DLLS = ["mosync", "mastd"]
		setup_base
	end
	
	def setup_pipe
		setup_base
	end
	
	def setup_base
		entities = FileTask.new(self, 'entities.c')
		entities.instance_eval do
			@prerequisites << FileTask.new(@work, 'entities.txt')
			def execute
				tfn = 'build/_temp.c'
				sh "gperf -tCE --language=ANSI-C --lookup-function-name=entity_lookup entities.txt " +
					"| #{sed('s/#line/\\/\\/line/')} > #{tfn}"
				if(File.size(tfn) == 0)
					error "GPERF failed!"
				end
				FileUtils.mv(tfn, @NAME)
			end
		end
		
		@SOURCES = ["."]
		@IGNORED_FILES = ['entities.c']
		@IGNORED_HEADERS = ['entities.h']
		@EXTRA_SOURCETASKS = [entities]
		@SPECIFIC_CFLAGS = {"MTXml.cpp" => " -Wno-unreachable-code",
			"entities.c" => " -Wno-extra",
		}
		@INSTALL_INCDIR = "MTXml"
		@NAME = "mtxml"
	end
end

MoSyncLib.invoke(mod)
