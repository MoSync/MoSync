#!/usr/bin/ruby

require "../../../rules/util.rb"
require "../../../rules/mosync_util.rb"
require "FileUtils"

msbuildpath = "/Windows/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe"

# build everything
sh "#{msbuildpath} mosync/mosyncRuntime/mosyncRuntime.csproj"

# build directory structure
FileUtils.rm_rf(["template"])
FileUtils.mkpath(["template/Libraries", "template/Libraries/mosyncRuntime", "template/RebuildData"])

FileUtils.cp_r Dir.glob('mosync/mosyncRuntime/Bin/Release/*.*'), 'template/Libraries/mosyncRuntime', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosyncRuntime/Bin/Debug/*.*'), 'template/Libraries/mosyncRuntime', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync/*.*'), 'template', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync/Properties'), 'template', :verbose => true

#sh "winphone-builder -input-file template/mosync.csproj -output-type rebuilt -output-file template/mosync_rebuilt.csproj"
#sh "winphone-builder -input-file template/mosync.csproj -output-type interpreted -output-file template/mosync_interpreted.csproj"
#FileUtils.rm(["template/mosync.csproj"])

FileUtils.rm_rf([mosyncdir+"/profiles/runtimes/winphone/1/template"])
FileUtils.rm_rf([mosyncdir+"/profiles/runtimes/winphone/1/template/extension_dev_template"])
FileUtils.mkpath([
					mosyncdir+"/profiles/runtimes/winphone",
					mosyncdir+"/profiles/runtimes/winphone/1",
					mosyncdir+"/profiles/runtimes/winphone/1/template",
					mosyncdir+"/profiles/runtimes/winphone/1/extension_dev_template"])
FileUtils.cp_r "template", mosyncdir+"/profiles/runtimes/winphone/1"

FileUtils.cp_r "mosync/mosyncExtensionTemplate", mosyncdir+"/profiles/runtimes/winphone/1"
FileUtils.cp_r "mosync/mosyncExtensionTemplate", mosyncdir+"/profiles/runtimes/winphone/1"
