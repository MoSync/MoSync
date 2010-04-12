#!/usr/bin/ruby
require File.expand_path('../../rules/mosync_lib.rb')

class YasperWork < Work
	include MoSyncMod
	def setup
		@INSTALL_INCDIR = 'yasper'
		@SOURCES = ['.']
		copyHeaders
	end
end

YasperWork.new.invoke
