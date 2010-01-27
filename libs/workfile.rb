#!/usr/bin/ruby

require File.expand_path('../rules/task.rb')

SUBDIRS = ["MAStd", "MAUtil", "MTXml", "MAFS", "MAUI", "MATest", "MAP"] #, "MinUI"

target :pipe do
	Work.invoke_subdirs(SUBDIRS, 'pipe')
end

target :native do
	Work.invoke_subdirs(SUBDIRS, 'native')
end

target :default do
	Work.invoke_subdirs(SUBDIRS, 'pipe')
end

Targets.invoke
