#!/usr/bin/ruby

require File.expand_path('../../rules/host.rb')
require File.expand_path('../../rules/mosync_util.rb')
require File.expand_path('../../rules/dll.rb')

class MxInvokerTask < FileTask
	def initialize(work, sourceIdl)
		super(work, 'output/invoke-extension.cpp')
		@source = sourceIdl
		@mxInvoker = mosyncdir + '/bin/mx-invoker'
		@prerequisites << DirTask.new(work, 'output')
		@prerequisites << FileTask.new(work, @source)
		@prerequisites << FileTask.new(work, @mxInvoker + EXE_FILE_ENDING)
	end
	def execute
		sh "#{@mxInvoker} -i #{@source} -o output"
	end
end

work = DllWork.new
work.instance_eval do
	@invokerTask = MxInvokerTask.new(self, 'test.idl')
	def setup
		@prerequisites << @invokerTask
		super
	end
	@EXTRA_SOURCETASKS = [@invokerTask]
	@SOURCES = ['.']
	@EXTRA_INCLUDES = [mosyncdir + '/ext-include', 'output']
	@EXTRA_CPPFLAGS = ''
	@NAME = "ext_Test"
end

work.invoke
