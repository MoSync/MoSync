BD = "../.."
require "#{BD}/build_rules/host.rb"
require "#{BD}/build_rules/defaults.rb"

SUBFILES = ["rakefile.makesis.rb", "rakefile.signsis.rb"]

def do_subfile(file, target="")
	sh "call rake -f #{file} #{target} CONFIG=\"#{CONFIG}\""
end

def do_subfiles(files, target="")
	files.each do |file| do_subfile(file, target) end
end

task :default do
	do_subfiles(SUBFILES)
end

task :clean do
	do_subfiles(SUBFILES, "clean")
end
