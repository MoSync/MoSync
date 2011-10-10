#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_exe.rb')

work = PipeExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@LIBRARIES = ["mautil"]
	@NAME = "GLMoblet_OpenGLES1"
end

work.invoke
