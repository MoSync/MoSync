#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')
require File.expand_path('../../rules/targets.rb')
require File.expand_path('../../rules/host.rb')

work = MoSyncExe.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = []
	if(HOST == :win32) then
		@CUSTOM_LIBS = ['libexpat.lib']
	else
		@LIBRARIES = ['expat']
	end
	@NAME = 'wrapperGenerator'
	@TARGETDIR = '.'
end

target :default do
	work.invoke
end


# run the WG on one header file. copy the results to designated areas.
class WGComboTask < Task
	def initialize(work, src)
		super(work)
		@src = FileTask.new(work, src)
	end
	
	class WGTask < FileTask
		def initialize(work, name, src)
			super(work, name)
			@src = src
			@prerequisites = [@work.target, @src]
		end
		def execute
			sh "#{@work.target} #{@src}"
		end
	end
	
	def wg(dst)
		return WGTask.new(@work, dst, @src)
	end
	
	def setup
		cpp = wg(@src.to_s+'.cpp')
		idl = wg(@src.to_s+'.idl')
		@prerequisites = [
			CopyFileTask.new(@work, "../idl2/opengl_generated.idl", idl),
			CopyFileTask.new(@work, "../../runtimes/cpp/generated/#{File.basename(cpp)}", cpp)
			]
	end
	def needed?
		false
	end
end

target :compile => :default do
	wct = WGComboTask.new(work, 'tests/gl.h')
	wct.setup
	wct.invoke
end


Targets.invoke
