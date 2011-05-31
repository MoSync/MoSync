# Copyright (C) 2010 MoSync AB
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


# Author: Ali Mosavian <mosavian at kth dot se>
#
# usage: ruby ConcurrentBuild.rb output_dir src_dir [filter1 filter2 ...]
#
require 'thread'
require 'pathname'
require 'fileutils'



##
# The class represents a runtime type and its
# settings suchs as whether or not tasks within
# the same runtime should be parallelized or not.
#
##
class RuntimeSetting
    ##
    # Constructor
    #
    # @param name    The runtime name.
    # @param pattern The regex to use to match to this runtime.
    # @param parallelize Whether the tasks within the class should
    #                    run concurrently against one another.
    ##
    def initialize ( name, pattern, parallelize  )
        @name = name
        @pattern = pattern
        @shouldParallelize = parallelize
    end

    def getName 
        return @name
    end
    
    def getPattern
        return @pattern
    end
    
    def shouldParallelize?
        return @shouldParallelize
    end
end



##
# Represents a runtime build task. A command line
# to RuntimeBuilder.rb is supplied to it, from which
# different peices of information is inferred. 
#
##
class RuntimeBuildTask
    ##
    # Constructor, fetches the runtime name using the
    # runtime path.
    #
    # @param cmdLine The command line with which this runtime
    #                can be built (using RuntimeBuilder.rb)
    ##
    def initialize ( cmdLine )
        @cmdLine     = cmdLine
        @cmdLineList = cmdLine.split( )

        # Check format
        if ( @cmdLineList.length != 5 )
            raise Exception::new( "Unexpected runtime build command (#{cmdLine})" )
        end

        # Fetch runtime path
        runtimePath     = @cmdLineList[-1]
        runtimePathList = runtimePath.split( File::SEPARATOR )
        if ( runtimePathList.length < 2 )
            raise Exception::new( "Unexpected runtime path format (#{runtimePath})" )
        end

        # Get runtime name
        @runtimeName = runtimePathList[-2].downcase( )
    end

    def getRuntimeName
        return @runtimeName
    end

    def getCommandLine
        return @cmdLine
    end
end

##
# A work package, contains multiple build task which can
# be run sequantially within the same thread. Every pair
# of work packages can always be build in parallel.
#
##
class WorkPackage
    ##
    # Constructor
    #
    ##
    def initialize (  )
        @logList  = []
        @taskList = []
    end

    ##
    # Adds a task to the work package.
    #
    # @param task Task to add.
    ##
    def addTask ( task )
        @taskList.push( task )
    end

    ##
    # Will attempt to run a task, logging its output to the
    # internal log of this class.
    #
    # @param index The index of the task to run.
    # @throws Exception if running the task failed.
    ##
    def runTask ( index )
        task = @taskList[index]
        cmdLine = "#{task.getCommandLine( ).gsub( /\n/, "" )} 2>&1\n"

        @logList.push( "\n----------------------------------\n" )
        @logList.push( "Task: #{cmdLine}\n" )

        begin
            IO::popen( cmdLine ) do |io|
                while !io.eof?
                    line = io.gets( )
                    @logList.push( line )
                end

                # Return code
                exitCode = Process::waitpid2( io.pid )[1].exitstatus
                if ( exitCode != 0 )
                    raise Exception::new( "#{cmdLine}" )
                end
            end
        ensure
            @logList.push( "----------------------------------\n" )
        end
    end

    def getTaskCount
        return @taskList.length( )
    end

    def getLog 
        return @logList
    end
end


#
# Settings - name, pattern, parrallelize?
#
SETTINGS = Array::new( [
                            RuntimeSetting::new( "javame", "javame", true ),
                            RuntimeSetting::new( "android", "android", false ),
                            RuntimeSetting::new( "iphoneos", "iphoneos", false ),
                            RuntimeSetting::new( "s60", "s60", false ),
                            RuntimeSetting::new( "wm", "(wm|sp)", false ),
                            RuntimeSetting::new( "moblin", "moblin", false )
                       ] )

##
# Matches the supplied parameter to a RuntimeSetting
# object.
#
# @param name Name to try to match against one of
#             the runtime settings
# @return RuntimeSetting object
#
# @throws Exception if name can not be matched
##
def getRuntimeTypeFromName ( name )
    SETTINGS.each do |item|
        if ( !name.match( item.getPattern( ) ).nil? )
            return item
        end
    end

    raise Exception::new( "Unknown runtime type '#{name}'" )
end


##
# Returns the os type.
#
# @return "win", "linux" or "mac"
#
# @throws Exception if os/platform is unknown.
##
def getOS
    platform = RUBY_PLATFORM.downcase( )
    if ( platform.include?( "mswin" ) ||
         platform.include?( "mingw" ) )
        return "win"
    elsif ( platform.include?( "linux" ) )
        return "linux"
    elsif ( platform.include?( "darwin" ) )
        return "mac"
    end

    raise Exception::new( "Unknown platform" )
end


##
# Attempts to find out the number cores a the machine has.
# Supported OSes are NT and NT based oses, linux and OS X.
#
# @return Number of cores, always one or greater.
##
def getCoreCount
    os = getOS( )
    if ( os == "win" )
        count = ENV["NUMBER_OF_PROCESSORS"].to_i( )
    elsif ( os == "linux" )
        count = `cat /proc/cpuinfo | grep processor | wc -l`.gsub( /\n/, "" ).to_i( )
    elsif ( os == "mac" )
        count = `sysctl hw.ncpu | awk '{print $2}'`.gsub( /\n/, "" ).to_i( )
    end

    count = count < 1 ? 1 : count
    return count
end


##
# Will run concurrently run the supplied work package
# list, with the specified maximum number of threads.
# Note: When something fails, all the other treads are
#       notified to not run anything else, but will not
#       cancel the currently running build task.
#
# @param srcPath    The absolute path to the MoSync source tree.
# @param workList   An array containing WorkPackage's.
# @param maxThreads The maximum number of threads to use [default is 4].
#
# @return True if build was succesful, false if not.
##
def runBuildTasks ( srcPath, workList, maxThreads = 4 )
    # Setup
    consoleMutex = Mutex::new( )
    buildFailed = false
    initialPath = FileUtils.pwd( )
    threadCount = maxThreads < workList.length ? maxThreads : workList.length

    FileUtils.cd( "#{srcPath}/tools/RuntimeBuilder" )

    puts "-------------------"
    puts " Starting build with #{threadCount} thread(s)"
    puts "-------------------\n"
    puts "Please be patient, the output of tasks are viewed once its work package has completed.\n\n"

    # Start threads
    threadList = (0..threadCount-1).map do |i|
        Thread::new( i ) do |threadID|
            threadID.step( workList.length-1, threadCount ) do |j|
                begin
                    # Run one work package
                    workPack = workList[j]
                    0.upto( workPack.getTaskCount( )-1 ) do |k|
                        if ( buildFailed == false )
                            workPack.runTask( k )
                        end
                    end

                rescue Exception => e
                    buildFailed = true
                    puts "Task failed: " + e.to_s( )
                end

                # Only log to console once a work package has
                # finished and only allow one thread to do it
                # at a time.
                consoleMutex.synchronize do |e|
                    puts "#{workList[j].getLog( ).join( )}"
                end
            end
        end
    end

    # Wait for threads
    threadList.each do |t| 
        t.join( )
    end

    FileUtils.cd( initialPath )
    return buildFailed == false
end


##
# Runs the profile converter and collects its output
# as build tasks.
#
# @param outPath The absolute path where the profiles
#                are to be generated
# @param srcPath The absolute path of the MoSync source tree.
# @return Array of RuntimeBuildTask's
#
# @throws Exception if the profile converter could not be
#         run succesfully.
##
def runConvAndCollectOutput ( outPath, srcPath )
    # Setup
    resultList = Array::new( )

    # Change to correct path
    currPath = FileUtils.pwd( )
    FileUtils.cd( "#{srcPath}/tools/ProfileConverter" )

    # Run the profile converter and get output
    begin
        IO::popen( "ruby conv.rb -nobuild -dst #{outPath}/profiles" ) do |io|
            while !io.eof?
                line = io.gets
                cmdList = line.split( )

                if ( cmdList.length > 2 &&
                     cmdList[0].downcase( ) == "ruby" &&
                     cmdList[1].downcase( ) == "runtimebuilder.rb" )
                    resultList.push( RuntimeBuildTask::new( line ) )
                end
            end

            # Return code
            exitCode = Process::waitpid2( io.pid )[1].exitstatus
            if ( exitCode != 0 )
                raise Exception::new( "ProfileConverter (conv.rb) returned #{exitCode}" )
            end
        end
    ensure
        FileUtils.cd( currPath )
    end
  
    return resultList
end


##
# Groups all the runtimes together in a map where
# the grouping will be based on the runtime type.
#
# @param runtimeList An array with MoSyncRuntime objects
# @return A map of type Hash<String, Array<RuntimeBuildTask>>
##
def groupRuntimeList ( runtimeList )
    resultMap = Hash.new( )

    runtimeList.each do |item|
        name = item.getRuntimeName( )
        type = getRuntimeTypeFromName( name )

        if ( resultMap[type].nil? == true )
            resultMap[type] = Array::new( )
        end
    
        resultMap[type].push( item )
    end

    return resultMap
end


##
# Divides the build tasks to work packages. The size of
# each work package depends on whether or not the runtime
# to be built can parallelize or note. If so, the tasks
# will be divided into four tasks per package, if not
# all of the runtimes of the same type will be put into the
# same work package.
# 
# @param runtimeMap A map containing the build tasks grouped
#                   by runtime type.
#
# @return An array of WorkPackage's
##
def createWorkList ( runtimeMap )
    workList = Array::new( )

    runtimeMap.each do |type, list|
        workPackCount = 1
        elementsPerWorkPack = list.length

        if ( type.shouldParallelize?( ) == true )
            elementsPerWorkPack = 4
            extraRunTimes = ((list.length % elementsPerWorkPack) != 0) ? 1:0;
            workPackCount = list.length / elementsPerWorkPack + extraRunTimes
        end

        for i in 0..workPackCount-1 do
            workPack = WorkPackage::new( )

            for j in 0..elementsPerWorkPack-1 do
                if(list[i*elementsPerWorkPack+j] !=nil)
                    task = list[i*elementsPerWorkPack+j]
                    workPack.addTask( task )
		end
            end

            workList.push( workPack )
        end
    end

    return workList
end


##
# Prints out the program usage and exits.
#
##
def printUsageAndExit
    puts "Usage: #{File::basename( __FILE__ )} out_path src_path [filter1 filter2 ...]"
    exit( 1 )
end

##
# Parses the arguments of the program using the
# global var ARGV. Badly formated arguments will
# cause it to print the program usage and exit.
#
# @return outPath - The path where the profiles will be outputed to.
# @return srcPath - Path to the MoSync source tree.
# @return filterMap - A map of filters to use for selecting runtimes
##
def parseArgs 
    # Parse args
    if ( ARGV.length < 2 )
        printUsageAndExit( )
    end
    outPath = Pathname::new( ARGV[0] ).realpath.to_s
    srcPath = Pathname::new( ARGV[1] ).realpath.to_s

    filterMap = Hash::new( )
    if ( ARGV.length > 2 )
        ARGV[2,ARGV.length-1].each do |name|  
            filterMap[name] = true
        end
    end
    
    return outPath, srcPath, filterMap
end


##
# The main function of the program. Will parse arguments,
# build profiles and then build the runtimes concurrently
# based on how many cores there exists on the machine.
#
##
def main
    outPath, srcPath, filterMap = parseArgs( )

    # Create profiles, collect the build commands and filter
    puts "-------------------\n"
    puts "Building profiles...\n"
    puts "-------------------\n"
    buildTasks = runConvAndCollectOutput( outPath, srcPath )
    puts "OK\n\n"
    
    # Filter tasks
    if ( filterMap.empty? == false )
        buildTasks = buildTasks.select do |task|
            name = task.getRuntimeName( )
            type = getRuntimeTypeFromName( name ).getName( )
            filterMap[type].nil? == false
        end
    end
  
    # Group runtimes and build work packages
    groupedBuildTasks = groupRuntimeList( buildTasks )
    workList = createWorkList( groupedBuildTasks ).shuffle( )

    # Do a concurrent build
    threadCount = getCoreCount( )
    threadCount = threadCount >= 2 ? threadCount : 2

    startTime = Time::now( ).to_f
    finished = runBuildTasks( srcPath, workList, threadCount )
    totalTime = (Time::now( ).to_f-startTime)

    puts "\n\n-------------------"
    if ( finished == true )
        puts "Built #{buildTasks.length} runtimes in %.1f seconds" % totalTime
        puts "On average - %.1f seconds/runtime" % (totalTime/buildTasks.length)
    else
        puts " Build failed"
    end
    puts "-------------------"

    exit( finished ? 0 : 1 )
end


#
# Entry point
#
main( )
