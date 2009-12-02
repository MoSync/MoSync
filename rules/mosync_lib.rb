require "#{File.dirname(__FILE__)}/dll.rb"
require "#{File.dirname(__FILE__)}/pipe.rb"

class MoSyncDllWork < DllWork
	def setup
		setup_native
		super
	end
end

class PipeLibWork < PipeGccWork
	def setup
		@FLAGS = " -L"
		setup_pipe
		super
	end
	def filename; @NAME + ".lib"; end
end

module MoSyncLib end

def MoSyncLib.inin(work, mod)
	work.extend(mod)
	work.invoke
end

def MoSyncLib.invoke(mod)
	target :pipe do
		MoSyncLib.inin(PipeLibWork.new, mod)
	end
	target :native do
		MoSyncLib.inin(MoSyncDllWork.new, mod)
	end
	target :default => [:pipe, :native]
	
	Targets.invoke
end
