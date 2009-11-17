require File.expand_path('../../rules/exe.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = ["src"]
	@EXTRA_INCLUDES = ["include"]
	@EXTRA_CPPFLAGS = " -Wno-shadow -Wno-unreachable-code"
	@LIBRARIES = ["z"]

	@NAME = "makesis-200"
end

work.invoke
