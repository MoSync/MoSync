#!/usr/bin/ruby

# Copyright (C) 2010 MoSync AB
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

require "../../../rules/util.rb"
require "../../../rules/mosync_util.rb"
require "FileUtils"

msbuildpath = "/Windows/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe"

# build everything
sh "#{msbuildpath} mosync/mosyncRuntime/mosyncRuntime.csproj"
sh "#{msbuildpath} mosync/mosyncRuntime_solution_WP8/mosyncRuntime_WP8.sln"

# build directory structure
FileUtils.rm_rf(["template"])
FileUtils.rm_rf(["template_WP8"])
FileUtils.mkpath(["template/Libraries", "template/Libraries/mosyncRuntime", "template/RebuildData"])
FileUtils.mkpath(["template_WP8/Libraries", "template_WP8/mosyncRuntime", "template_WP8/mosync_WP8", "template_WP8/MoSyncLibrary"])
FileUtils.mkpath(["template_WP8/Libraries/Media", "template_WP8/Libraries/Media/debug_lib_build"])
FileUtils.mkpath(["template_WP8/packages"])

FileUtils.cp_r Dir.glob('mosync/mosyncRuntime/Bin/Release/*.*'), 'template/Libraries/mosyncRuntime', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosyncRuntime/Bin/Debug/*.*'), 'template/Libraries/mosyncRuntime', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync/*.*'), 'template', :verbose => true
FileUtils.cp_r Dir.glob('mosync/Libraries/*.*'), 'template_WP8/Libraries', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync_WP8/*.*'), 'template_WP8/mosync_WP8', :verbose => true
FileUtils.cp_r Dir.glob('mosync/Libraries/Media/debug_lib_build/*.*'), 'template_WP8/Libraries/Media/debug_lib_build', :verbose => true
FileUtils.cp_r Dir.glob('mosync/MoSyncLibrary/*.*'), 'template_WP8/MoSyncLibrary', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync/Properties'), 'template', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync_WP8/Properties'), 'template_WP8/mosync_WP8', :verbose => true
FileUtils.cp_r Dir.glob('mosync/mosync_WP8/Toolkit.Content'), 'template_WP8/mosync_WP8', :verbose => true
FileUtils.cp_r Dir.glob('mosync/packages/*.*'), 'template_WP8/packages', :verbose => true

sh "xcopy \"mosync/mosyncRuntime\" \"template_WP8/mosyncRuntime\" /S /I /EXCLUDE:xcopyExcludes.txt"

#sh "winphone-builder -input-file template/mosync.csproj -output-type rebuilt -output-file template/mosync_rebuilt.csproj"
#sh "winphone-builder -input-file template/mosync.csproj -output-type interpreted -output-file template/mosync_interpreted.csproj"
#FileUtils.rm(["template/mosync.csproj"])

FileUtils.rm_rf([mosyncdir+"/profiles/runtimes/winphone"])
FileUtils.mkpath([
					mosyncdir+"/profiles/runtimes/winphone",
					mosyncdir+"/profiles/runtimes/winphone/1",
					mosyncdir+"/profiles/runtimes/winphone/1/template",
					mosyncdir+"/profiles/runtimes/winphone/1/template_WP8",
					mosyncdir+"/profiles/runtimes/winphone/1/extension_dev_template"])
FileUtils.cp_r "template", mosyncdir+"/profiles/runtimes/winphone/1"
FileUtils.cp_r "template_WP8", mosyncdir+"/profiles/runtimes/winphone/1"

FileUtils.cp_r "mosync/mosyncExtensionTemplate", mosyncdir+"/profiles/runtimes/winphone/1"
FileUtils.cp_r "mosync/mosyncExtensionTemplate", mosyncdir+"/profiles/runtimes/winphone/1"
