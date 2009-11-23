require "#{File.dirname(__FILE__)}/defaults.rb"
require "#{File.dirname(__FILE__)}/targets.rb"
require 'fileutils'
require 'singleton'
require 'ftools'

class TaskBase
	#@prerequisites	#Task[]
	#@work	#Work
	
	# EarlyTime is a fake timestamp that occurs _before_ any other time value.
	class EarlyTime
		include Comparable
		include Singleton
		
		def <=>(other)
			-1
		end
		
		def to_s
			"<EARLY TIME>"
		end
	end
	EARLY = EarlyTime.instance 
	
	def initialize()
		@prerequisites = []
	end
	
	attr_accessor(:prerequisites)
	
	def invoke
		#puts "invoke: #{self}"
		@prerequisites.each do |p| p.invoke end
		if(self.needed?) then
			puts "Execute: #{self}"
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
		#puts "Work.invoke: #{@NAME.inspect}"
		
		if(@prerequisites == nil) then
			setup
			if(@prerequisites == nil)
				error "setup failed"
			end
		end
		
		# if you invoke a work without setting up any targets,
		# we will check for the "clean" goal here.
		if(Targets.size == 0)
			Targets.setup
			if(Targets.goals == [:clean])
				self.execute_clean
				return
			end
		end
		
		@prerequisites.each do |p| p.invoke end
	end
	
	# invoke the workfile of another directory, as if it would have been called from the command line.
	def Work.invoke_subdir(dir, *args)
		puts File.expand_path(dir) + " " + args.inspect
		fn = dir + "/workfile.rb"
		if(!File.exists?(fn))
			error "No workfile found in #{dir}"
		end
		
		if(defined?(Targets))
			Targets.reset(args)
		end
		
		oldDir = Dir.getwd
		Dir.chdir(dir)
		load(File.expand_path('workfile.rb'), true)
		Dir.chdir(oldDir)
	end
end

class BuildWork < Work
	include Defaults
	def setup
		#puts "BuildWork.setup: #{@NAME.inspect}"
		set_defaults
		@prerequisites = [DirTask.new(self, @BUILDDIR)]
		setup2
		#dump(0)
	end
	
	def execute_clean
		#puts "execute_clean in #{self.inspect}"
		verbose_rm_rf(@TARGET)
		verbose_rm_rf(@BUILDDIR)
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
	
	def to_str
		@NAME
	end
	
	def to_s
		@NAME
	end
	
	# Is this file task needed?  Yes if it doesn't exist, or if its time stamp
	# is out of date.
	def needed?
		if(!File.exist?(@NAME))
			puts "Because file does not exist: #{@NAME}"
			return true
		end
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
		@prerequisites.any? { |n|
			if(n.timestamp > stamp)
				puts "Because prerequisite '#{n}' is newer: #{@NAME}"
				return true
			end
		}
	end
	
	def dump(level)
		puts (" " * level) + @NAME
		super
	end
end

class DirTask < FileTask
	def execute
		FileUtils.mkdir_p @NAME
	end
end

class CopyFileTask < FileTask
	def initialize(work, name, src, preq = [])
		super(work, name)
		@src = src
		@prerequisites = [src] + preq
	end
	def execute
		puts "cp #{@NAME} #{@src}"
		File.copy(@src, @NAME)
	end
end
