#!/usr/bin/ruby

require File.expand_path('../rules/task.rb')

target :pipe do
	Work.invoke_subdirs(SUBDIRS, 'pipe')
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
	stdlibs = ["newlib", "stlport"]
else
	stdlibs = ["MAStd"]
end

SUBDIRS = stdlibs + ["MAUtil", "MTXml", "MAUI", "MAUI-revamp", "MATest", "MAP",
	"Testify", "MAFS", "yajl", "Ads", "Facebook", "NativeUI", "Wormhole", "Notification", "ResCompiler"]

Targets.invoke