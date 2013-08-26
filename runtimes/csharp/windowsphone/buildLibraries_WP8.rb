#!/usr/bin/ruby

require "../../../rules/util.rb"
require "../../../rules/mosync_util.rb"
require "FileUtils"

msbuildpath = "/Windows/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe"

# build everything
sh "#{msbuildpath} mosync/mosyncRuntime_solution_WP8/mosyncRuntime_WP8.sln"

# build directory structure
FileUtils.rm_rf(["template_WP8"])
FileUtils.mkpath(["template_WP8/Libraries", "template_WP8/mosyncRuntime", "template_WP8/mosync_WP8", "template_WP8/MoSyncLibrary"])
FileUtils.mkpath(["template_WP8/Libraries/Media", "template_WP8/Libraries/Media/debug_lib_build"])
FileUtils.mkpath(["template_WP8/packages"])

FileUtils.cp_r Dir.glob('mosync/Libraries/*.*'), 'template_WP8/Libraries', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync_WP8/*.*'), 'template_WP8/mosync_WP8', :verbose => true
FileUtils.cp_r Dir.glob('mosync/Libraries/Media/debug_lib_build/*.*'), 'template_WP8/Libraries/Media/debug_lib_build', :verbose => true
FileUtils.cp_r Dir.glob('mosync/MoSyncLibrary/*.*'), 'template_WP8/MoSyncLibrary', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync_WP8/Properties'), 'template_WP8/mosync_WP8', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync_WP8/Toolkit.Content'), 'template_WP8/mosync_WP8', :verbose => true
FileUtils.cp_r Dir.glob('mosync/packages/*.*'), 'template_WP8/packages', :verbose => true

sh "xcopy \"mosync/mosyncRuntime\" \"template_WP8/mosyncRuntime\" /S /I /EXCLUDE:xcopyExcludes.txt"

FileUtils.rm_rf([mosyncdir+"/profiles/runtimes/winphone/1/template_WP8"])
FileUtils.mkpath([
					mosyncdir+"/profiles/runtimes/winphone",
					mosyncdir+"/profiles/runtimes/winphone/1",
					mosyncdir+"/profiles/runtimes/winphone/1/template_WP8"])
FileUtils.cp_r "template_WP8", mosyncdir+"/profiles/runtimes/winphone/1"
