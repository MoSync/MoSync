#!/usr/bin/ruby

require File.expand_path('../../rules/native_lib.rb')

idlc = NativeLibWork.new
idlc.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = [".."]
	if(!@GCC_IS_V4 && CONFIG == "")
		# gcc's C++ optimizer has a few bugs...
		@EXTRA_CPPFLAGS = " -Wno-uninitialized"
	end
	@NAME = "idl-common"
end

idlc.invoke
