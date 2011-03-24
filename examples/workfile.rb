#!/usr/bin/ruby

require File.expand_path('../rules/task.rb')
require File.expand_path("./parse_example_list.rb")
SUBDIRS = parseExampleList


target :default do
	Work.invoke_subdirs(SUBDIRS)
end

target :clean do
	Work.invoke_subdirs(SUBDIRS, 'clean')
end

Targets.invoke
