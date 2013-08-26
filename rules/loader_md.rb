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

require "#{File.dirname(__FILE__)}/host.rb"

# A dummy FileTask which allows header files to be removed without
# causing errors in the Work system.
# Removing header files without modifying the source files that include them
# will still cause compile errors.
class HeaderFileTask < FileTask
	def invoke
	end
end


class MakeDependLoader
	SPACE_MARK = "__&NBSP;__"

	# Load makefile dependencies in +fn+.
	# Return an array of Tasks.
	def MakeDependLoader.load(fn, target)
		res = nil
		target = target.gsub(' ', SPACE_MARK)
		open(fn) do |mf|
			lines = mf.read
			lines.gsub!(/\\ /, SPACE_MARK)
			lines.gsub!(/#[^\n]*\n/m, "")
			lines.gsub!(/\\\n/, ' ')
			lines.split("\n").each do |line|
				if(res) then
					error "MD: too many logical lines in file '#{fn}'"
				end
				res = process_line(fn, line, target)
			end
		end
		if(res == nil)
			res = []
		end
		return res
	end

	private

	# Process one logical line of makefile data.
	def MakeDependLoader.process_line(fn, line, target)
		index = line.index(': ')
		md = line[0, index].strip
		if(!filenamesEqual(target, md)) then
			puts "Warning: MD target mismatch in #{fn}. target: '#{target}' md: '#{md}'"
		end
		args = line[index+1, line.length]
		return [] if args.nil?
		depNames = args.split.map { |a| respace(a) }
		depTasks = depNames.collect { |d|
			HeaderFileTask.new(@work, d)
			#if(File.exists?(d) then
			#	FileTask.new(@work, d)
			#else
			#	MissingFileTask.new(@work, d)
			#end
		}
		return depTasks
	end

	def MakeDependLoader.respace(str)
		str.gsub(/#{SPACE_MARK}/, ' ')
	end
end
