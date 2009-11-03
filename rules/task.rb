require 'rules/defaults.rb'
require 'fileutils.rb'

class TaskBase
	#@prerequisites	#Task[]
	#@work	#Work
	
	def initialize()
		@prerequisites = []
	end
	
	def invoke
		@prerequisites.each do |p| p.invoke end
		if(self.needed?) then
			self.execute
		end
	end
	
	def timestamp
		EARLY
	end
	
	def needed?
		true
	end
	
	def dump(level)
		@prerequisites.each do |p| p.dump(level+1) end
	end
end

class Work < TaskBase
	def initialize
		@prerequisites = nil
	end
	
	def invoke
		if(@prerequisites == nil) then
			setup
		end
		@prerequisites.each do |p| p.invoke end
	end
	
	# invoke the workfile of another directory, as if it would have been called from the command line.
	def Work.invoke_subdir(dir, *args)
		m = Module.new
		m.const_set("ARGV", args)
		script = open(dir + "/workfile.rb") { |f| f.read }
		m.module_eval(script)
	end
end

class BuildWork < Work
	include Defaults
	def setup
		set_defaults
		@prerequisites = [DirTask.new(self, @BUILDDIR)]
		setup2
		@root.dump(0)
	end
end

class Task < TaskBase
	def initialize(work)
		super()
		@work = work
	end
end

class FileTask < Task
	#@name	#String
	
	def initialize(work, name)
		super(work)
		@NAME = name.to_s
	end
	
	def to_s
		@NAME
	end
	
	# Is this file task needed?  Yes if it doesn't exist, or if its time stamp
	# is out of date.
	def needed?
		return true unless File.exist?(@NAME)
		return true if out_of_date?(timestamp)
		return false
	end
	
	# Time stamp for file task.
	def timestamp
		if File.exist?(@NAME)
			File.mtime(@NAME)
		else
			EARLY
		end
	end
	
	# Are there any prerequisites with a later time than the given time stamp?
	def out_of_date?(stamp)
		@prerequisites.any? { |n| n.timestamp > stamp}
	end
	
	def dump(level)
		puts (" " * level) + @NAME
		super
	end
end

##unused
class DirTask < FileTask
	def execute
		FileUtils.mkdir_p @name
	end
end
