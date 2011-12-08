#!/usr/bin/ruby

require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
        @SOURCES = ["."]
        @EXTRA_INCLUDES = ["../../intlibs"]
        @LOCAL_LIBS = ["filelist", "profiledb"]

        @NAME = "profiledb"
        if(HOST==:linux || HOST==:darwin)
                @LIBRARIES = ["expat"]
        else    # win32
                @CUSTOM_LIBS = ["libexpat.lib"]
        end

        @INSTALLDIR = mosyncdir + '/bin'
end

work.invoke
