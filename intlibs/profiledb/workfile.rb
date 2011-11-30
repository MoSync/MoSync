#!/usr/bin/ruby

require File.expand_path('../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do
	@SOURCES = ["."]
        if ( HOST == :darwin )
                # Objective-C++ compiler
                @EXTRA_CPPFLAGS = " -Wno-shadow -Wno-missing-prototypes"
        end
	@EXTRA_INCLUDES = [".."]
	@NAME = "profiledb"
end

work.invoke
