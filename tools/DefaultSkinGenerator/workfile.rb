#!/usr/bin/ruby

require File.expand_path('../../rules/task.rb')

task = FileTask.new(nil, '../../libs/MAUI/DefaultSkin.h')
task.instance_eval do
	@prerequisites = Dir.glob('*.png') +  Dir.glob('*.mof')
	@prerequisites.collect! do |file| FileTask.new(nil, file) end
	def execute
		sh 'ruby Generator.rb'
	end
end

task.invoke
