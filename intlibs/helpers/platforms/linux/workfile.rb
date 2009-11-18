require File.expand_path('../../../../rules/native_lib.rb')

work = NativeLibWork.new
work.instance_eval do 
	@SOURCES = ["."]
	@EXTRA_INCLUDES = ["../../.."]
	@NAME = "mosync_log_file"
end

work.invoke
