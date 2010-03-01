#!/usr/bin/ruby

require File.expand_path('../rules/task.rb')

target :pipe do
	Work.invoke_subdirs(SUBDIRS, 'pipe')
	if(!USE_NEWLIB)
		#temp until we fix the directory thing
		sh "rake pipe CONFIG=\"#{CONFIG}\""
	end
end

target :native do
	Work.invoke_subdirs(SUBDIRS, 'native')
end

target :default => :pipe

target :clean do
	Work.invoke_subdirs(SUBDIRS, 'clean')
end

Targets.setup

if(USE_NEWLIB)
	stdlibs = ["newlib"]
else
	stdlibs = ["MAStd", "MAFS"]
end
SUBDIRS = stdlibs + ["MAUtil", "MTXml", "MAUI", "MATest", "MAP"] #, "MinUI"

Targets.invoke
