#!/usr/bin/ruby

USE_NEWLIB = true

require File.expand_path(ENV['MOSYNCDIR']+'/rules/mosync_lib.rb')

mod = Module.new
mod.class_eval do
	def setup_pipe
		@SOURCES = ['.', 'qrcode', 'decoder']
		@EXTRA_SOURCEFILES = ['video/mosyncVideo.c', 'window/mosyncWindow.c']
		@EXTRA_INCLUDES = ['.', 'include']
		@IGNORED_FILES = ['jpeg.c', 'svg.c', 'main.c', 'processor.c']

		@EXTRA_CFLAGS = ' -Wno-declaration-after-statement'+
			' -Wno-pointer-arith -Wno-sign-compare'+
			' -D_POSIX_SOURCE'	#avoid silly bsd functions
		@EXTRA_CFLAGS << ' -Wno-missing-field-initializers' if(@GCC_IS_V4)
		@EXTRA_CFLAGS << ' -Wno-unreachable-code' if(!@GCC_IS_V4)	#compiler bug

		@SPECIFIC_CFLAGS = {
			'isaac.c' => ' -Wno-parentheses',
			'bch15_5.c' => ' -Wno-parentheses',
			'binarize.c' => ' -Wno-parentheses',
			'qrdec.c' => ' -Wno-parentheses -Wno-shadow',
			'rs.c' => ' -Wno-parentheses',
			'util.c' => ' -Wno-parentheses',
			'code128.c' => ' -Wno-shadow',
		}
		@SPECIFIC_CFLAGS['convert.c'] = ' -w' if(!@GCC_IS_V4)

		@INSTALL_INCDIR = 'zbar'
		@HEADER_DIRS = []
		@NAME = 'zbar'
	end
end

MoSyncLib.invoke(mod)
