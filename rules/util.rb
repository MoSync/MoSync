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
end

def sh(cmd)
	#TODO: optimize by removing the extra shell
	#the Process class should be useful.
	$stderr.puts cmd
	if(!system(cmd)) then
		error "Command failed: '#{$?}'"
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
	if(file.gets == 'GNU sed' && HOST != :win32)
		return "sed '#{script}'"
	else
		return "sed #{script}"
	end
end
