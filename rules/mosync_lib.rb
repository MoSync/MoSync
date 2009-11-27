require "#{File.dirname(__FILE__)}/dll.rb"
require "#{File.dirname(__FILE__)}/pipe.rb"

class MoSyncDllWork < DllWork
	def setup
		#puts methods.inspect
		setup_native
		#@EXTRA_CFLAGS = @EXTRA_CFLAGS.to_s + " -DMOSYNC_DLL_IMPORT"
		#@EXTRA_CPPFLAGS = @EXTRA_CPPFLAGS.to_s + " -DMOSYNC_DLL_IMPORT"
		super
	end
end

class PipeLibWork
end

module MoSyncLib end

def MoSyncLib.inin(work, mod)
	#puts mod.inspect
	#puts mod.singleton_methods.inspect
	#puts mod.instance_methods.inspect
	#puts work.methods.inspect
	work.extend(mod)
	#puts work.methods.inspect
	work.invoke
end

def MoSyncLib.invoke(mod)
	target :pipe do
		#MoSyncLib.inin(PipeLibWork.new, mod)
	end
	target :native do
		MoSyncLib.inin(MoSyncDllWork.new, mod)
	end
	target :default => [:pipe, :native]
	
	Targets.invoke
end
