require File.expand_path('../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do 
	@SOURCES = []
	@EXTRA_SOURCEFILES = ["filelist-win32.c"]
	@EXTRA_INCLUDES = [".."]
	@NAME = "filelist"
end

work.invoke
