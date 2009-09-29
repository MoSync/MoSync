# Copyright (C) 2009 Mobile Sorcery AB
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

if(defined?(RAKEFILES) == nil) then
	RAKEFILES = [""]
end

RAKEFLAGS = RAKEFILES.collect {|rf| if(rf.length > 0) then " -f #{rf}" else "" end }

task :default do
	SUBDIRS.each do |dir| RAKEFLAGS.each do |rf|
		sh "cd #{dir} && rake -N#{rf} CONFIG=\"#{CONFIG}\""
	end end
end

task :clean do
	SUBDIRS.each do |dir| RAKEFLAGS.each do |rf|
		sh "cd #{dir} && rake -N#{rf} clean CONFIG=\"#{CONFIG}\""
	end end
end
