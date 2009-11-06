require File.expand_path('../../rules/native_lib.rb')

idlc = NativeLibWork.new
idlc.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = [".."]
	@NAME = "idl-common"
end

idlc.invoke
