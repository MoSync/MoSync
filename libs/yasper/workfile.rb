#!/usr/bin/ruby
require File.expand_path('../../rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	include MoSyncMod
	def run
		copyHeaders
	end
	
end

include MoSyncMod
def run
	copyHeaders
end

run
