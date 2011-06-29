#!/usr/bin/ruby

require File.expand_path('../../../rules/mosync_exe.rb')

class MxConfigTask < MultiFileTask
	def dllName(e)
		"#{@extDir}/ext_#{e[1]}.dll"
	end
	
	def initialize(work, extDir, extensions)
		@extensions = extensions
		@extDir = extDir
		mxNames = extensions.collect do |e|
			"build/mx_#{e[1]}.h"
		end
		super(work, 'build/mxConfig.txt', mxNames)
		@mxConfig = mosyncdir + '/bin/mx-config'
		@prerequisites << DirTask.new(work, 'build')
		@prerequisites << FileTask.new(work, @mxConfig + EXE_FILE_ENDING)
		@extensions.each do |e|
			@prerequisites << FileTask.new(work, e[0])
			@prerequisites << FileTask.new(work, dllName(e))
		end
	end
	def execute
		params = ''
		@extensions.each do |e|
			params += " #{e[0]} #{dllName(e)}"
		end
		sh "#{@mxConfig} -o build#{params}"
	end
end

work = PipeExeWork.new
work.instance_eval do
	def setup
		set_defaults
		@prerequisites << MxConfigTask.new(self, "#{@COMMON_BASEDIR}/build/#{CONFIG}", [['../test.idl', 'Test']])
		super
	end
	@SOURCES = ['.']
	@EXTRA_INCLUDES = ['build']
	@EXTRA_CPPFLAGS = ''
	@NAME = "ext2_user"
end

work.invoke
