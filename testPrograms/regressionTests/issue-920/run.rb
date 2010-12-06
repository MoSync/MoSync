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

require 'timeout'

@MOSYNCDIR = ENV['MOSYNCDIR']

##
# Builds a MoSync test program to use for testing.
# NOTE: It requires MoSync to be installed.
#
# @throws Exception if building of the test program fails.
##
def buildTestProgram
  puts "==== Building test program ====\n"
  if ( system( "#{@MOSYNCDIR}/bin/xgcc -S -o main.s testprogram/main.cpp" ) == false )
    raise Exception.new( "Failed to compile MoSync program" )
  end

  if ( system( "#{@MOSYNCDIR}/bin/pipe-tool -s#{@MOSYNCDIR}/lib/pipe -B program main.s mastd.lib" ) == false )
    raise Exception.new( "Failed to compile MoSync program" )
  end
end

##
# Tests for issue 920
#   see http://code.google.com/p/mosync/issues/detail?id=920
#
# @param total_list An array where each element is the number
#                   of iterations to run each pass.
##
def testBug ( total_list )
  result     = Array.new( )  
  more_path  = "#{@MOSYNCDIR}/bin/MoRE"

  puts "\n\n==== Running test ===="

  # Actual test
  total_list.each do |iterations|
    failed = 0

    puts "Will now run #{iterations} iterations"
    1.upto( iterations ) do |i|    
      proc = IO.popen( "#{more_path} -program program" )
      pid  = proc.pid

      #
      # Wait up to fifteen seconds for MoRE to finish,
      # if fifteen seconds has passed, it will kill
      # MoRE and count it as a deadlock.
      #
      begin
        Timeout.timeout( 15 ) { Process.wait( pid ) }      
      rescue Exception => e
        Process.kill( "KILL", pid )      
        failed = failed + 1
        puts "\tTotal #{i}, failed #{failed}"
      end

      proc.close( )
    end

    result.push( iterations => failed )  
  end

  # Go through result
  total_failures = 0
  result.each do |elem|
    elem.each() do |iter, fails|
      total_failures = total_failures + fails
      puts "Ran #{iter} iterations with #{fails} failures"
    end
  end

  return total_failures
end

##
# Entry point
#
##
def main
  buildTestProgram( )
  fails = testBug( [10, 100, 500, 1000, 10000] )
  puts "\n\n==== Report ===="
  if ( fails > 0 )
    puts "Failed: The bug occured #{fails} times in total."
  else
    puts "Passed: The bug did not occur."
  end
end

main





