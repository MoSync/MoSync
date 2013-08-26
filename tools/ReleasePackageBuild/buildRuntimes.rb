# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


require 'fileutils'

def returnToBuild(path, ret)
	FileUtils.cd path
	exit ret
end

# The the current path
cpath = FileUtils.pwd

# Make the output folder
FileUtils.mkdir_p "#{ENV['MOSYNC_PROFILES_PATH']}"

internalBuild = ENV['MOSYNC_INTERNAL_BUILD']

# If this is not an internal MoSync build, generate the runtimes which is possible to create from Windows
# Symbian, JavaME, Windows Mobile and Android
# Othervise we shall use the distributed build system

if(internalBuild == nil)

	puts "Building runtimes"

	FileUtils.cd "#{ENV['MOSYNC_TRUNK']}/tools/profileConverter"
	
	returnToBuild cpath, system("ruby conv.rb -dst #{ENV['MOSYNC_PROFILES_PATH']}")
else
	
	puts "Build MoSync Internal runtimes"
	
	FileUtils.cd "#{ENV['MOSYNC_RELEASE_BUILD_PATH']}/private-runtimebuilder"
	FileUtils.cp 'lib/distributed/ClientBuilder.rb', 'ClientBuilder.rb'
	ret = system "ruby ClientBuilder.rb profiles.zip #{ENV['MOSYNC_RELEASE_BUILD_PATH']}/mosync-trunk -w"
	if true == ret
		ret = system "unzip profiles.zip -d #{ENV['MOSYNC_PATH']}"
	end
	
	returnToBuild cpath, ret
end
