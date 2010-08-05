#!/usr/bin/ruby

require 'fileutils.rb'
require File.expand_path('../../../../rules/targets.rb')
require File.expand_path('../../../../rules/task.rb')
require File.expand_path('../../../../rules/loader_md.rb')

JAVA_ME_LIB = ENV['JAVA_ME_LIB']
TMPCLASS_DIR = ENV['PROJECT_DIR'] + '/tmpclasses'
CLASS_DIR = ENV['PROJECT_DIR'] + '/classes'
JAVA_DIR = ENV['PROJECT_DIR'] + '/src'
SHARED_DIR = '../../shared'

SHARED_SOURCES = [
	'BigPhatError',
	'Binary',
	'BinaryInterface',
	#'BufferlessPipeStream',
	'Core',
	'LimitedLengthInputStream',
	'LittleEndianDataInputStream',
	'ThreadPool',
	'UBin',
	'RefByteArrayOutputStream',
	'ImageCache',
	'MAPanicReport',
]
JAVAME_SOURCES = [
	'MAMidlet',
	'Syscall',
	'MainCanvas',
	#'PngStream',
	'MoSync',
	#'ExtensionHandler',
	#'AudioBufferDataSource',
]
CLDC10_SOURCES = [
	'Real',
]

def escape(fn)
	fn.gsub(/\\/, "\\\\\\\\").gsub("/", "\\/")
end

class JavaPreprocessTask < FileTask
	def initialize(work, dir, name)
		super(work, "#{JAVA_DIR}/#{name}.java")
		@dir = dir
		@name = name
		@prerequisites = [FileTask.new(work, "#{@dir}/#{@name}.jpp")]
		
		@DEPFILE = "build/#{@name}.mf"
		if(!needed?(false)) then
			@prerequisites = MakeDependLoader.load(@DEPFILE, @NAME)
		end
	end
	
	def needed?(log = true)
		return true if(super(log))
		if(!File.exists?(@DEPFILE))
			puts "Because the dependency file is missing:" if(log)
			return true
		end
		return false
	end
	
	def execute
		$stderr.puts "preprocessing #{@dir}/#{@name}.jpp"
		
		tempDepFileName = "build/#{@name}.mft"
		
		#@REM sed reformats gcc's error output so that Visual Studio can understand it
		sh("xgcc -x c -E -MMD -MF #{tempDepFileName} -D_JavaME -I#{SHARED_DIR} -Isrc" +
			" -o build/#{@name}.jtmp \"#{@prerequisites[0]}\" 2>&1 | sed s/\([a-zA-Z/]\+\)\(.[a-zA-Z]\+\):\([0-9]\+\):/\\1\\2(\\3):/")
		
		# problem: if xgcc fails due to a preprocessing error, it doesn't return an error value.
		# it does, however, output an empty dependency file, which causes later rebuilds to fail.
		# we have to check for this emtpy file and throw an error if we find it.
		if(File.size(tempDepFileName) == 0)
			error("xgcc failed silently")
		end
		
		sh("sed s/#{@name}.o/#{escape(@NAME)}/ < build/#{@name}.mft > #{@DEPFILE}")
		
		#@REM sed changes gcc's file position lines into comments
		#@REM the second sed changes source code filenames into numbers for the PUBLIC_DEBUG version
		sh("sed \"s/^# /\\/\\//\" < build/#{@name}.jtmp > \"#{@NAME}\"")
	end
end

class JavaCompileTask < Task
	def initialize(work, preqs)
		super(work)
		@prerequisites = preqs
	end
	def execute
		# clean up
		FileUtils.rm Dir.glob("#{TMPCLASS_DIR}*.class")
		FileUtils.rm Dir.glob("#{CLASS_DIR}*.class")
		# compile
		if(PLATFORM == 'cldc10')
			baseJar = 'cldcapi10'
		elsif(PLATFORM == 'cldc11')
			baseJar = 'cldcapi11'
		else
			error("Unsupported platform: #{PLATFORM}")
		end
		jars = "#{JAVA_ME_LIB}\\jsr082.jar;#{JAVA_ME_LIB}\\#{baseJar}.jar;" +
			"#{JAVA_ME_LIB}\\midpapi20.jar;#{JAVA_ME_LIB}\\wma20.jar;" +
			"#{JAVA_ME_LIB}\\jsr179.jar;#{JAVA_ME_LIB}\\jsr75.jar;" +
			"#{JAVA_ME_LIB}\\mmapi.jar"
		sh("javac -source 1.4 -target 1.4 -d #{TMPCLASS_DIR} -classpath #{TMPCLASS_DIR} " +
			"-bootclasspath #{jars} #{JAVA_DIR}/*.java")
		# preverify
		sh("preverify -d #{CLASS_DIR} -classpath #{jars} #{TMPCLASS_DIR}")
	end
end

work = Work.new
work.instance_eval do
	def setup
		javaFiles = JAVAME_SOURCES.collect { |n| JavaPreprocessTask.new(self, 'src', n) }
		javaFiles += SHARED_SOURCES.collect { |n| JavaPreprocessTask.new(self, SHARED_DIR, n) }
		
		default_const(:PLATFORM, 'cldc11')
		if(PLATFORM == 'cldc10')
			javaFiles += CLDC10_SOURCES.collect { |n| JavaPreprocessTask.new(self, 'src', n) }
		elsif(PLATFORM != 'cldc11')
			error("Unsupported platform: #{PLATFORM}")
		end
		
		@prerequisites = [DirTask.new(self, 'build'), DirTask.new(self, CLASS_DIR), DirTask.new(@self, JAVA_DIR),
			DirTask.new(self, TMPCLASS_DIR), JavaCompileTask.new(self, javaFiles)]
	end
end

target :default do
	work.invoke
end

# parses ARGV
Targets.invoke
