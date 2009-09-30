
task :default => ["../idl2/opengl_generated.idl", "../../runtimes/java/shared/generated/opengl_invoke_generated.jpp",
	"../../libs/MAStd/GLES/magl_functions.h"]
	
GENFILES = Rake::Task[:default].prerequisites.collect {|path|
	base = File.basename(path)
	file path => base do |t|
		sh "cp #{base} #{path}"
	end
	file base => ["GLWrapperGen.rb", "gl_common_lite.h"] do
		sh "ruby GLWrapperGen.rb"
	end
	base
}

task :clean do
	Rake::Task[:default].prerequisites.each do |path|
		sh "rm -f #{File.basename(path)} #{path}"
	end
end
