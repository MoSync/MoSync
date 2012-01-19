#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

work = PipeLibWork.new
work.instance_eval do
	def setup_pipe
		@SOURCES = [
			".",
			"Libs",
			"Libs/JSNativeUI",
			"Libs/Notification",
			"Libs/PhoneGap",
			"Libs/W3C"]

		@EXTRA_INCLUDES = ['.']

		copyFilesSubDir("Libs")
		copyFilesSubDir("Libs/JSNativeUI")
		copyFilesSubDir("Libs/Notification")
		copyFilesSubDir("Libs/PhoneGap")
		copyFilesSubDir("Libs/W3C")

		@HEADER_DIRS = ['.']
		@INSTALL_INCDIR = "Wormhole"

		@NAME = "Wormhole"
	end

	def copyFilesSubDir(name)
		@INSTALL_INCDIR = "Wormhole/" + name
		@HEADER_DIRS = [name]
		copyHeaders
	end
end

work.invoke
