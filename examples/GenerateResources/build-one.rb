require 'FileUtils'
require File.expand_path(ENV['MOSYNCDIR'] + '/rules/util.rb')
require File.expand_path(ENV['MOSYNCDIR'] + '/rules/mosync_util.rb')
require './config.rb'

PROJECT_NAME = 'GenRes'

def tryCopy(profile, ending)
	deviceName = profile.split('/')[1]
	src = "FinalOutput/Release/#{profile}/package/#{PROJECT_NAME}.#{ending}"
	dst = "build/pkg/#{PROJECT_NAME} #{deviceName}.#{ending}"
	FileUtils.cp(src, dst) if(File.exist?(src))
end

def buildOne(profile, doBuild)
	FileUtils.mkdir_p('build/pkg/')
	if(doBuild)
		FileUtils.mv('profile.rb', 'profile.bak', :force=>true) if(File.exist?('profile.rb'))
		f = open('profile.rb', 'w')
		f.write(
			"PROFILE = '#{profile}'\n"
		)
		f.close
		sh "ruby workfile.rb lst"
		FileUtils.mv('profile.bak', 'profile.rb', :force=>true) if(File.exist?('profile.bak'))
		f = open('finalizer.txt', 'w')
		f.write(
			"begin #{profile} end\n"
		)
		f.close
		sh "#{mosyncdir}/eclipse/mosyncc.exe -noSplash -application com.mobilesorcery.sdk.builder.headless -data #{WORKSPACE} -project #{PROJECT_NAME} -f finalizer.txt"
	end
	tryCopy(profile, 'sisx')
	tryCopy(profile, 'apk')
	tryCopy(profile, 'jar')
end

p = ARGV[0]
puts "Profile: #{p}"
buildOne(p, true)
