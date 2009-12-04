# This file defines helper classes used for compiling MoSync libraries for both
# native and pipe platforms.
# To use, call MoSyncLib.invoke with a module
# which has the setup_native and setup_pipe methods defined.

require "#{File.dirname(__FILE__)}/dll.rb"
require "#{File.dirname(__FILE__)}/pipe.rb"

module MoSyncMod
	def modSetup
		@EXTRA_INCLUDES = @EXTRA_INCLUDES.to_a + ["#{ENV['MOSYNCDIR']}/include"]
	end
end

class MoSyncDllWork < DllWork
	include MoSyncMod
	def setup
		setup_native
		modSetup
		@EXTRA_LINKFLAGS = @EXTRA_LINKFLAGS.to_s + " -Wl,--enable-auto-import"
		super
	end
end

class PipeLibWork < PipeGccWork
	include MoSyncMod
	def setup
		@FLAGS = " -L"
		setup_pipe
		modSetup
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
