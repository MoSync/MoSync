# Copyright (C) 2010 MoSync AB
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
