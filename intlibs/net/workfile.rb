require File.expand_path('../../rules/native_mosync.rb')

work = NativeMoSyncLib.new
work.instance_eval do 
	@SOURCES = ["."]
	@NAME = "net"
end

work.invoke
