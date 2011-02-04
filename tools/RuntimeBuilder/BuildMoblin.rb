# Copyright (C) 2009 Mobile Sorcery AB
# 
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
require 'fileutils'

##
# Check if directory exists
#
# @param path Directory path
#
# @return true/false
##
def dirExist? ( path )
    return File.exist?( path ) && File.directory?( path )
end


##
# Will return a list of Debian package dependencies
# as an array.
#
# @param fileName Absolute path to the file to lookup
#                 dependencies for.
#
# @return Array of dependencies, or empty array if failed
##
def findDEBDepends ( fileName )
    currPath = FileUtils.pwd( );

    begin
        `which dpkg-shlibdeps`
        if ( $?.exitstatus != 0 )
            raise Exception::new( "Could not find 'dpkg-shlibdeps'" )
        end

        FileUtils.cd( "/tmp" )
        if ( dirExist?( "debian" ) == false )
            FileUtils.mkdir( "debian" )
        end
        if ( dirExist?( "debian/control" ) == false )
            FileUtils.mkdir( "debian/control" )
        end

        resultStr = `dpkg-shlibdeps -O #{fileName} 2>/dev/null`.gsub( /\n/, "" )
        if ( $?.exitstatus != 0 )
            raise Exception::new( "Failed to run dpkg-shlibdeps" )
        end

        resultArray = `echo '#{resultStr}' | sed 's/shlibs:Depends=//'`.gsub( /\n/, "" ).split( ", " )
        if ( $?.exitstatus != 0 )
            raise Exception::new( "Failed to filter dpkg-shlibdeps output" )
        end
    rescue Exception => e
        resultArray = []
    end

    FileUtils.cd( currPath )
    return resultArray
end

##
# Will return a list of RPM package requirements
# as an array.
#
# @param fileName Absolute path to the file to lookup
#                 dependencies for.
#
# @return Array of requirement, or empty array if failed
##
def findRPMRequires ( fileName )
    resultArray = []

    if ( File::exists?( "/usr/lib/rpm/find-requires" ) == false )
        return resultArray
    end

    begin
        reqStr = `echo #{fileName} | /usr/lib/rpm/find-requires`
        if ( $?.exitstatus != 0 )
            raise Exception::new( "Failed to run find-requires" )
        end

        reqStr.split( /\n/ ).each do |r|
            resultArray.push( r )
        end
    rescue Exception => e
        resultArray = []
    end

    return resultArray
end

##
# Will generate the meta-data for the packaging by finding the
# debian and rpm dependencies.
#
# @param fileName Absolute path to the file to lookup
#                 dependencies for.
#
# @return String containing the meta data as json.
##
def generateMetaData ( fileName )
    debDepends  = findDEBDepends( fileName )
    rpmRequires = findRPMRequires( fileName )

    if ( debDepends.empty?( ) && rpmRequires.empty?( ) )
        raise Exception::new( "Failed generate dependency data" )
    end

    return "{
        \"prerm\"     : \".meta/prerm\",
        \"postrm\"    : \".meta/postrm\",
        \"preinst\"   : \".meta/preinst\",
        \"postinst\"  : \".meta/postinst\",
        \"programDir\": \"opt/%appname%/bin\",
        \"programFile\": \"opt/%appname%/bin/program\",
        \"resourceFile\": \"opt/%appname%/bin/resources\",
        \"parseList\" : [ \"usr/bin/%appname%\", \"usr/share/applications/%appname%.desktop\" ],
        \"iconList\"  : [ {\"type\": \"png\", \"size\":16, \"path\":\"usr/share/icons/hicolor/16x16/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":32, \"path\":\"usr/share/icons/hicolor/32x32/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":48, \"path\":\"usr/share/icons/hicolor/48x48/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":64, \"path\":\"usr/share/icons/hicolor/64x64/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":128, \"path\":\"usr/share/icons/hicolor/128x128/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":256, \"path\":\"usr/share/icons/hicolor/256x256/apps/%appname%.png\"}, {\"type\": \"svg\", \"path\":\"usr/share/icons/hicolor/scalable/apps/%appname%.svg\"} ],
        \"depends\"   : #{debDepends.to_s( )},
        \"requires\"  : #{rpmRequires.to_s( )}
}"
end

##
# Checks which linux distro this machine is using the
# standard way of checking.
#
# @return Distro name or 'unknown'
##
def findDistroName
    distMap = { "/etc/moblin-release" => "moblin",
                "/etc/meego-release"  => "meego",
                "/etc/lsb-release"    => "ubuntu",
                "/etc/SUSE-release"   => "suse",
                "/etc/redhat-release" => "redhat",
                "/etc/redhat_version" => "redhat",
                "/etc/fedora-release" => "fedora",
                "/etc/debian-release" => "debian",
                "/etc/debian_version" => "debian",
                "/etc/gentoo-release" => "gentoo",
                "/etc/mandrake-release" => "mandrake",
                "/etc/slackware-release" => "slackware",
                "/etc/slackware_version" => "slackware" }
            
    # Check
    distMap.each do |file, name|  
        if ( File.exist?( file ) == true )        
            return name
        end
    end

    return "unknown"
end


##
# Returns kernel architecture.
# Note: Intel will always be 'i386'
#
# @return String
##
def getKernelArch
    return `uname -m | sed 's/i.../i386/'`.gsub( /\n/, "" )
end


##
# Generates a random string.
#
# @param base Base name
# @param len  Number random chars
# 
# @return String
##
def genString ( baseName, len )
    chars = ("a".."z").to_a + ("A".."Z").to_a + ("0".."9").to_a
    randChars = ""
    1.upto( len ) do |i| 
        randChars << chars[rand(chars.size-1)]
    end
    return baseName+randChars
end


##
# Pretty print a message
#
# @param msg Message
##
def printMsg ( msg )
    puts "\n\n---------------------------------"
	puts " #{msg}"
	puts "---------------------------------\n\n"
end


##
# Copies the content of one directory to another, applying
# a filter to select which files to copy or not to copy.
#
# @param srcPath Origin
# @param dstPath Destination
# @param filter  The filter function which will select files
#                to copy.
##
def filterCopy ( srcPath, dstPath, filter )
    Dir.entries( srcPath ).each do |file|
        srcFilePath = "#{srcPath}/#{file}"
        dstFilePath = "#{dstPath}/#{file}"

        if ( file != "." && file != ".." && filter.call( file ) )
            puts srcFilePath
            if ( File::directory?( srcFilePath ) == true )
                if ( File::exists?( dstFilePath ) == false )
                    FileUtils.mkdir( dstFilePath )
                end
                filterCopy( srcFilePath, dstFilePath, filter )
            else                
                FileUtils.cp( srcFilePath, dstFilePath )
            end
        end

    end
end



##
# Actual build class
#
# 
##
class RuntimeBuilder
	def moblin ( runtime_dir, mode )        
        initialPath = FileUtils.pwd( )
        workPath    = "#{initialPath}/#{genString( "temp", 5 )}"
        scriptPath  = File::dirname( File::expand_path( __FILE__ ) )
        runtime_dir = Pathname::new( runtime_dir ).realpath( ).to_s
        srcPath     = Pathname::new( "#{scriptPath}/../../" ).realpath( ).to_s

        # Check platform, we only attempt to build on Linux
        if ( RUBY_PLATFORM.match( "linux" ).nil? == true )
            puts "Warning: Linux runtime must be built in Linux, skipping..."
            return 0
        end

        begin
            # Remove any existing config file and copy
            # new one
            debug = (mode=="debug") ? "D" : ""
            configPath = "#{runtime_dir}/config#{debug}.h"
            if ( File::exists?( "#{srcPath}/runtimes/cpp/platforms/sdl/config_platform.h" ) == true )
                FileUtils.rm( "#{srcPath}/runtimes/cpp/platforms/sdl/config_platform.h" )
            end
            FileUtils.cp( configPath, "#{srcPath}/runtimes/cpp/platforms/sdl/config_platform.h" )

            #
            # Initial setup
            #
            FileUtils.mkdir( workPath )
            FileUtils.mkdir( "#{workPath}/bin" )
            ENV["MOSYNCDIR"] = workPath

            printMsg( "Performing clean" )
            FileUtils.cd( srcPath )
            if ( system( "ruby workfile.rb clean_more CONFIG=\"\"" ) == false )
                raise Exception::new( "Failed to run clean command" )
            end

            #
            # Build MoRE
            #
            printMsg( "Attempting to build MoRE" )
            if ( system( "ruby workfile.rb more CONFIG=\"\" FULLSCREEN=\"true\" NATIVE_RUNTIME=\"true\"" ) == false )
                raise Exception::new( "Failed to build MoRE" )
            end

            if ( File::exists?( "#{workPath}/bin/MoRE" ) == false )
                raise Exception::new( "Could not find 'MoRE'" )
            end

            #
            # Copy and set-up template
            #
            printMsg( "Copying template package" )
            FileUtils.cd( workPath )
            filterCopy( "#{scriptPath}/linux/template", "#{workPath}", lambda { |f| f != ".svn" }  )
            FileUtils.cp( "bin/MoRE", "opt/%appname%/bin/run" )
            puts "OK"
            

            #
            # Generate its meta data
            #
            printMsg "Generating template meta data"
            meta = generateMetaData( File::expand_path( "opt/%appname%/bin/run" ) )
            `echo '#{meta}' > .meta/.meta`
            puts meta

            printMsg "Packing custom template"            
            if ( mode == "debug" )
                packageName = "runtime.dbg.tar.gz"
            else
                packageName = "runtime.rel.tar.gz"
            end
            if ( system( "tar cvvfz #{runtime_dir}/#{packageName} $(find .meta -type f | sort) usr opt" ) == false )
                raise Exception::new( "Failed to package runtime" )
            end
            puts "OK"

        ensure
            printMsg "Performing clean up"
            if ( dirExist?( workPath ) == true )
                sleep( 0.25 )
                FileUtils.rm_rf( workPath )
            end
            FileUtils.cd( initialPath )
            puts "OK"
        end
        
		return 0
	end
end