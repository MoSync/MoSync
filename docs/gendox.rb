#!/usr/bin/ruby

require 'fileutils'
require '../rules/util.rb'

include FileUtils::Verbose

MOSYNC_DOXYGEN_OUTPUT_DIR = "#{ENV['MOSYNCDIR']}/docs/"
MOSYNC_INCLUDE_DIR = "#{ENV['MOSYNCDIR']}/include/"

mkdir_p(MOSYNC_DOXYGEN_OUTPUT_DIR)

cp('release_notes.txt', MOSYNC_DOXYGEN_OUTPUT_DIR)
cp('changelog.txt', MOSYNC_DOXYGEN_OUTPUT_DIR)

cd '..'
SRCDIR = pwd
sh 'ruby workfile.rb libs'

cp('libs/Doxyfile', MOSYNC_INCLUDE_DIR)

cd MOSYNC_INCLUDE_DIR
if(HOST == :win32)
	sh "#{SRCDIR}/tools/ReleasePackageBuild/build_package_tools/bin/doxygen"
else
	sh 'doxygen'
end
