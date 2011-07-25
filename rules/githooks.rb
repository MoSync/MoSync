# Copyright (C) 2011 Mobile Sorcery AB
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

require 'FileUtils'
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
		FileUtils.ln_s(target, source, :verbose => true)
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
