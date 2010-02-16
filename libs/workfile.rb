#!/usr/bin/ruby

require File.expand_path('../rules/task.rb')

SUBDIRS = ["MAStd", "MAUtil", "MTXml", "MAFS", "MAUI", "MATest", "MAP"] #, "MinUI"

target :pipe do
	Work.invoke_subdirs(SUBDIRS, 'pipe')
	#temp until we fix the directory thing
	sh "rake pipe CONFIG=\"#{CONFIG}\""
end

target :native do
	Work.invoke_subdirs(SUBDIRS, 'native')
end

target :default => :pipe

target :clean do
	Work.invoke_subdirs(SUBDIRS, 'clean')
end

Targets.invoke
