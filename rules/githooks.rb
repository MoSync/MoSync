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
require "#{File.dirname(__FILE__)}/util.rb"

# If run at the root level of a Git working copy,
# this function creates a symbolic link from "githooks" to ".git/hooks"
# If ".git/hooks" already exists and is not the desired symbolic link,
# it is renamed to ".git/hooks.bak", unless the backup already exists,
# in which case an error is raised.
# This works even on Windows NTFS disks, thanks to the program "Junction".

# This function should be run at the beginning of the root build script,
# to ensure that every developer has version-controlled git hooks installed.
def enforceGithooks
	target = '.git/hooks'
	source = 'githooks'
	backup = '.git/hooks.bak'
	if(!File.exist?('.git'))
		puts '.git not found.'
		return
	end
	if(dirsAreEqual?(target, source))
		puts 'Hook directories are equal.'
		return
	end
	if(File.exist?(target))
		if(File.exist?(backup))
			error "#{target} is broken and #{backup} already exists."+
				" Please remove one of the directories."
		end
		FileUtils.mv(target, backup, :verbose => true)
	end
	if(HOST == :win32)
		sh "githooks/junction.exe #{target} #{source}"
	else
		FileUtils.ln_s('../'+source, target, :verbose => true)
	end
end

def nonDotEntries(dir)
	return Dir.entries(dir).reject do |file| file[0,1] == '.' end
end

def dirsAreEqual?(a, b)
	return false if(!File.exist?(a))
	return false if(!File.exist?(b))
	af = nonDotEntries(a)
	bf = nonDotEntries(b)
	return false if(af != bf)	# compare filenames
	af.each_with_index do |file, i|
		return false if(File.mtime(a+'/'+file) != File.mtime(b+'/'+bf[i]))
	end
	# We could check the contents of the files, but I think that's overkill.
	return true
end
