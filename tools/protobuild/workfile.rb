require File.expand_path('../../rules/exe.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = ["."]
	@EXTRA_CFLAGS = " -Wno-strict-prototypes -Wno-missing-prototypes -Wno-old-style-definition -Wno-missing-noreturn"
	@NAME = "protobuild"
end

work.invoke
