#!/usr/bin/ruby

require File.expand_path('../../rules/mosync_lib.rb')

work = PipeLibWork.new
work.instance_eval do
	def copyFilesSubDir(name)
		@INSTALL_INCDIR = 'Facebook/' + name
		@HEADER_DIRS = [ name ]
		copyHeaders
	end

	def setup_pipe
		@SOURCES = ['.', 'GraphAPI', 'GraphAPI/GetConnections',
			'GraphAPI/GetFacebookObjects', 'GraphAPI/GetFacebookObjects/FacebookObjects',
			'GraphAPI/Publish', 'HTTP', 'JSON_lib']
		@EXTRA_INCLUDES = ['.']

		copyFilesSubDir('GraphAPI/GetFacebookObjects/FacebookObjects')
		copyFilesSubDir('GraphAPI/GetFacebookObjects')
		copyFilesSubDir('GraphAPI/GetConnections')
		copyFilesSubDir('GraphAPI/Publish')
		copyFilesSubDir('GraphAPI')
		copyFilesSubDir('JSON_lib')
		copyFilesSubDir('HTTP')

		@HEADER_DIRS = ['.']
		@INSTALL_INCDIR = 'Facebook'

		@NAME = 'Facebook'
	end
end

work.invoke
