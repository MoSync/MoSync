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

# To change this template, choose Tools | Templates
# and open the template in the editor.

require 'optparse'

# The base eclipse path
ECLIPSE_BASE_PATH = "com.mobilesorcery.sdk.help/docs/html/"

# Parser for command line arguments for the doxy2cdt converter.
class CmdArguments < Hash
  attr_reader :doxygen_input_xml, :cdt_output_xml, :eclipse_base_path

  def initialize( argv )
    super( )

    @eclipse_base_path = ECLIPSE_BASE_PATH

    parser = OptionParser.new do |options|
      options.banner = "Usage: doxy2cdt [options] input_dir output_xml"

      options.on( '-h', '--help', 'Displays this help message.' ) do
        puts options
        exit
      end

      options.on( '-b', "--base-path BASE_PATH", 'The base path that will be appended to each link.' ) do |base_path|
        if( base_path[-1,1] != File::SEPARATOR )
          base_path = base_path + File::SEPARATOR
        end
        @eclipse_base_path = base_path
      end

    end

    # Actually start to parse the arguments and remove them from argv
    parser.parse!( argv )

    if( argv.length( ) < 1 )
      puts parser.help
      exit
    end

    if( argv.length( ) >= 1 )
      @doxygen_input_xml = argv[0]
    end

    if( argv.length( ) >= 2 )
      @cdt_output_xml = argv[1]
    end
  end
end
