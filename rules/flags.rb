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


# This module contains functions for saving and comparing compile or link flags.
# execFlags and flagsNeeded require the function "cFlags".
module FlagsChanged
	# Call from initialize.
	def initFlags
		# save cflags to disk, for use as dependency.
		# but first read it back from disk, if existing.
		@FLAGSFILE = @NAME + ".flags"
		if(File.exists?(@FLAGSFILE)) then
			@OLDFLAGS = open(@FLAGSFILE) { |f| f.read }
		end
		#puts "Oldflags: #{@OLDFLAGS.inspect}"
	end
	
	# Call from execute.
	def execFlags
		if(@OLDFLAGS != cFlags) then
			open(@FLAGSFILE, 'w') { |f| f.write(cFlags) }
			@OLDFLAGS = cFlags
		end
	end
	
	# Call from needed?.
	def flagsNeeded?(log=true)
		#puts "Oldflags: #{@OLDFLAGS.inspect} newflags: #{@FLAGS.inspect}"
		if(@OLDFLAGS != cFlags)
			puts "Because the flags have changed:" if(log)
			#puts "Old: #{@OLDFLAGS}" if(log)
			#puts "New: #{@FLAGS}" if(log)
			return true
		end
		return false
	end
end
