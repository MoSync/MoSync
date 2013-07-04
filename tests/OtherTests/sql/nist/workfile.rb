#!/usr/bin/ruby

require File.expand_path('../../../rules/exe.rb')

work = ExeWork.new
work.instance_eval do
	@SOURCES = ['.']
	@EXTRA_INCLUDES = ['../../../intlibs']
	@EXTRA_CFLAGS = ' -include sl3.h'
	@SPECIFIC_CFLAGS = {
		'basetab.c' => ' -ansi -Wno-strict-prototypes -Wno-old-style-definition -Wno-return-type -Wno-implicit-int',
	}
	@LOCAL_LIBS = ['sqlite']
	@NAME = 'basetab'
end

work.invoke
