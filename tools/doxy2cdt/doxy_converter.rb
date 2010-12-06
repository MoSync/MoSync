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

require 'rexml/document'
include REXML

##
# This class is used to convert a doxygen XML index to a CDT XML index.
#
class DoxyConverter
  def initialize(base_path)
    @m_base_path = base_path
  end

  ##
  # Converts the input doxygen xml tree.
  #
  # @param doxy_index_xml_doc An XML document tree for the CDT input xml.
  # @param result_cdt_doc     The resulting XML tree for the CDT index.
  # @param title              Title of the CDT index.
  # @param type               Type of index, should be either HELP_TYPE_C or
  #                           HELP_TYPE_CPP.
  # @return The root of the XML CDT index.
  #
  def doxy_to_cdt_xml(doxy_index_xml_doc, result_cdt_doc, title, type)
    documentation = result_cdt_doc.add_element( "documentation" )
    help_book = create_help_book( title, type )
    documentation.add( help_book )

    doxy_index_xml_doc.each_element( "/doxygenindex/compound/" ) do | compound |
      compound_name = get_name( compound )
      compound_refid = compound.attributes[ "refid" ]

      # Create an entry element for the namespace
      href = create_root_href( compound_refid )
      entry = Element.new( "entry" )
      entry.add_attribute( 'keyword', remove_namespace( compound_name ) )
      entry.add_element( "topic", { 'title' => REXML::Text.normalize( compound_name ), 'href' => href } )
      entry.add_element( "functionSummary" )
      help_book.push( entry )

      # Create an entry for each component
      compound_members = []
      compound.each_element( "member/" ) do | member |

        # For now filter out the variables of structs and classes
        # since CDT does not support multiple search words and variable
        # names are likely to collide.
        if( member.attributes[ "kind" ] == "variable" )
          next
        end

        compound_members.push( member )
      end

      entries = create_entries( compound_members, compound_refid, compound_name );

      entries.each do | entry |
        help_book.add( entry )
      end
    end

    return result_cdt_doc

  end

  ##
  # Creates a CDT help book of the given type with the given title.
  #
  # @param title Title of the help book.
  # @param type Type of index, should be either HELP_TYPE_C or HELP_TYPE_CPP.
  # @return An XML element representing the help book.
  #
  private
  def create_help_book(title, type)
    help_book = Element.new( "helpBook" )
    help_book.add_attribute( 'title' , title )
    help_book.add_attribute( "type", type )
    return help_book
  end

  ##
  # Creates an entry for the given CDT compound tree.
  #
  # @param a CDT compound entry, which consists of one or more members.
  # @return a list of XML elements corresponding to the list of members.
  #
  private
  def create_entries( members, compound_refid, compound_name )
    entries = []
    
    if( not compound_refid or not compound_name )
      return entries
    end

    members.each do | member |
       member_name = get_name( member )
       member_refid = member.attributes[ "refid" ]
       href = create_href( compound_refid, member_refid )

       entry = Element.new( "entry" )
       entry.add_attribute( 'keyword', remove_namespace( member_name ) )
       title = REXML::Text.normalize( member_name ) + " (" + compound_name + ")"
       entry.add_element( "topic", { 'title' => title, 'href' => href } )
       entry.add_element( "functionSummary" )

       entries.push( entry )
    end

    return entries
  end

  ##
  # Creates an anchored link from the compound and member refids. This is
  # used in the index to display the documentation of a specific element.
  #
  # @param compond_refid A string containing the refid attribute of a <compound>
  #                      element.
  # @param member_refid  A string containing the refid attribute of a <member>
  #                      element.
  # @param return The anchored link.
  # @return the anchored link to the given compound member.
  #
  private
  def create_href(compound_refid, member_refid)
    html_file = create_root_href( compound_refid )

    # The anchor seems to be the last part of the refid of which
    # the first character is removed.
    member_anchor = member_refid.split( "_" ).last.slice( 1..-1 )

    return html_file, "#", member_anchor
  end

  ##
  # Returns the link to the html file that contains the description of
  # the given compound refid.
  #
  # @param compound_refid The refid to link.
  # @return the link to the compound's html file from the base path.
  #
  private
  def create_root_href(compound_refid)
    return @m_base_path, compound_refid, ".html"
  end

  ##
  # Returns the contents of the <name> tag contained in the
  # given XML element.
  #
  # @param xml_element An XML element containing a <name> tag.
  # @return the contents of the first <name> tag.
  #
  private
  def get_name(xml_element)
    name_element = xml_element.elements[ "name/" ]

    if( name_element and name_element.text )
      return name_element.text
    else
      return ""
    end
  end

  ##
  # Returns the identifier of a nested namespace. For example
  # 'NAMEPSACE1::NAMESPACE2::ID' is converted to 'ID'.
  #
  # @param symbol a string containing a nested namespace.
  # @return the string without a namespace
  #
  private
  def remove_namespace(symbol)
    if( not symbol.index( "::" ) )
      return symbol
    else
      return symbol.split( "::" ).last
    end
  end

end