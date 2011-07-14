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

require "#{File.dirname(__FILE__)}/error.rb"
require "#{File.dirname(__FILE__)}/host.rb"
require 'singleton'

$stdout.sync = true
$stderr.sync = true

def default(constant, value)
	s = ("if(defined?(@#{constant.to_s}) == nil) then @#{constant.to_s} = #{value.inspect} end")
	eval(s)
end

def set_const(name, value)
	s = "#{name.to_s} = #{value.inspect}"
	eval(s)
end

def default_const(constant, value)
	s = ("if(defined?(#{constant.to_s}) == nil) then #{constant.to_s} = #{value.inspect} end")
	eval(s)
end

def set_class_var(c, sym, val)
	c.send(:class_variable_set, sym, val)
end

def get_class_var(c, sym)
	c.send(:class_variable_get, sym)
end

class String
	def ext(newEnd)
		doti = rindex('.')
		slashi = rindex('/')
		if(doti && slashi) then if(slashi > doti) then
			return self + newEnd
		end end
		return self[0, doti] + newEnd
	end
	
	def getExt
		doti = rindex('.')
		slashi = rindex('/')
		if(doti && slashi) then
			if(slashi > doti) then
				return nil
			end
		end
		return self[doti..self.length]
	end
	
	def noExt
		doti = rindex('.')
		return self[0, doti]
	end
	
	# Returns true if self begins with with.
	def beginsWith(with)
		return false if(self.length < with.length)
		return self[0, with.length] == with
	end
end

def sh(cmd)
	# Print the command to stderr.
	warn cmd
	# Open a process.
	IO::popen(cmd) do |io|
		# Pipe the process's output to our stdout.
		while !io.eof?
			line = io.gets
			puts line
	    end
	    # Check the return code
	    exitCode = Process::waitpid2(io.pid)[1].exitstatus
	    if(exitCode != 0) then
			error "Command failed, code #{exitCode}"
	    end
	end
end

class Object
	def need(*args)
		vars = instance_variables.inject({}) { |h,var| h[var.to_sym] = true; h }
		#puts instance_variables
		#puts vars.inspect
		args.each do |var|
			var = var.to_sym
			#puts "#{var}: #{vars[var].inspect}"
			if(!vars[var])
				error "Undefined variable: #{var}"
			end
		end
	end
end

def verbose_rm_rf(list)
	case list
	when Array
		arr = list.collect do |p| p.to_str end
		puts "Remove #{arr.inspect}"
	else
		puts "Remove '#{list}'"
	end
	FileUtils.rm_rf(list)
end

HashMergeAdd = Proc.new {|key, old, new| old + new }


# returns a command-line string with the correct invocation of sed for all platforms
def sed(script)
	file = open("|sed --version 2>&1")
	if((file.gets.beginsWith('GNU sed') && HOST != :win32) ||
		HOST == :darwin)
		return "sed '#{script}'"
	else
		return "sed #{script}"
	end
end

# EarlyTime is a fake timestamp that occurs _before_ any other time value.
# Its instance is called EARLY.
class EarlyTime
	include Comparable
	include Singleton
	
	def <=>(other)
		return 0 if(other.instance_of?(EarlyTime))
		return -1
	end
	
	def to_s
		"<EARLY TIME>"
	end
end
EARLY = EarlyTime.instance

# LateTime is a fake timestamp that occurs _after_ any other time value.
# Its instance is called LATE.
class LateTime
	include Comparable
	include Singleton
	
	def <=>(other)
		return 0 if(other.instance_of?(LateTime))
		return 1
	end
	
	def to_s
		"<LATE TIME>"
	end
end
LATE = LateTime.instance

# Extension to the standard Time class to allow comparison with EarlyTime and LateTime.
class Time
	alias work_original_compare :<=>
	def <=>(other)
		if(other.instance_of?(EarlyTime) || other.instance_of?(LateTime))
			return - other.<=>(self)
		else
			return work_original_compare(other)
		end
	end
end

# Extension to class File, to make sure that the drive letter is always lowercase.
# This resolves an issue where programs were rebuilt due to file paths being changed,
# itself due to strange behaviour in the Windows command-line console.
class File
	if(HOST == :win32)
		def self.expand_path_fix(p)
			ep = self.expand_path(p)
			return ep if(ep.length <= 3)
			if(ep[1,1] == ':')
				ep[0,1] = ep[0,1].downcase
			end
			return ep
		end
	else
		class << self	# alias class methods, rather than instance methods
			alias_method(:expand_path_fix, :expand_path)
		end
	end
end

def min(a, b)
	return a if(a < b)
	return b
end

def max(a, b)
	return (a > b) ? a : b
end
