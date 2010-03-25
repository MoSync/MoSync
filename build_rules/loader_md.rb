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

module Rake

class Task
	def clear_prerequisites
		prerequisites.clear
		self
	end
end

  # Makefile loader to be used with the import file loader.
  class MakeDependLoader
    SPACE_MARK = "__&NBSP;__"

    # Load the makefile dependencies in +fn+.
    def load(fn)
      open(fn) do |mf|
        lines = mf.read
        lines.gsub!(/\\ /, SPACE_MARK)
        lines.gsub!(/#[^\n]*\n/m, "")
        lines.gsub!(/\\\n/, ' ')
        lines.split("\n").each do |line|
          process_line(line)
        end
      end
    end

    private

    # Process one logical line of makefile data.
    def process_line(line)
      index = line.index(':')
      file_tasks = line[0, index]
      args = line[index+1, line.length]
      return if args.nil?
      dependents = args.split.map { |d| respace(d) }
      file_tasks.strip.split.each do |file_task|
        file_task = respace(file_task)
	myTask = file file_task
        file file_task => dependents
	#puts "add '#{file_task}' => #{dependents}"
	#puts "pre '#{file_task}' => #{(file file_task).prerequisites}"
	preqs = myTask.prerequisites
	myTask.clear_prerequisites
	rejected = preqs.reject! do |p|
		res = (!File.exists?(p) && p != (file file_task).prerequisites[0])
		if(res) then
			puts "Reject #{file_task} => '#{p}': #{res}"
		end
		res
	end
	#puts "Rejected #{file_task}: #{rejected != nil}"
	#puts "Rejected #{file_task}: #{rejected}"
	file file_task => preqs
	#puts "pre '#{file_task}' => #{(file file_task).prerequisites}"
      end
    end
    
    def respace(str)
      str.gsub(/#{SPACE_MARK}/, ' ')
    end
  end

  # Install the handler
  Rake.application.add_loader('md', MakeDependLoader.new)
end

