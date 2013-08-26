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
require "#{File.dirname(__FILE__)}/error.rb"

def switchPathSlashes(cmd)
	error("null path") if(!cmd)
	if(HOST == :win32)
		# bug in windows command line parser causes commands on the following format to fail:
		# \directory\subdir/executable
		# to work around, we substitute all \ for /.
		return cmd.gsub('\\', '/')
	end
	return cmd
end

# This should always be used instead of the ENV.
def mosyncdir
	return switchPathSlashes(ENV['MOSYNCDIR'])
end
