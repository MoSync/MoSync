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

# makeWidget.rb
#
# Takes an argument of the format Leonardo::Math::Vector3
# and generates a header file for that class.
#

require 'FileUtils'

class String
  def myCapitalize
    self[0,1].upcase+self[1,self.length-1]
  end
end

# a method to print a number of tabs
def tabs(s, n)
  n.times do s << "\t" end
end

def setProp(widgetName, type, name)
  case type
    when 'int' then "#{widgetName}->#{name} = atoi(value);"
    when 'char*' then "strncpy(#{widgetName}->#{name}, value, MIN(strlen(value)+1, MAX_STRING_LENGTH));"
    when 'Handle' then "#{widgetName}->#{name} = (Handle)atoi(value);"
  end
end

def getProp(widgetName, type, name)
  case type
    when 'int' then "sprintf(buf, \"%d\", #{widgetName}->#{name});"  
    when 'char*' then "strcpy(buf, #{widgetName}->#{name});"
    when 'Handle' then "sprintf(buf, \"%d\", (int)#{widgetName}->#{name});"  
  end
end

def makeSetter(widgetName, type, name)
  ret = "void MAUI#{widgetName}_set#{name.myCapitalize}(MAUIWidget* w, #{type} #{name}) {\n"
  ret << "\tMAUI#{widgetName}* #{widgetName[0,1].downcase} = (MAUI#{widgetName}*)w;\n"
  unless type == 'char*'
    ret << "\t#{widgetName[0,1].downcase}->#{name} = #{name};\n"
  else
    ret << "\tstrncpy(#{widgetName[0,1].downcase}->#{name}, #{name}, MIN(strlen(#{name})+1, 127));\n"
  end
  ret << "}\n\n"
  ret
end

def makeGetter(widgetName, type, name)
  ret = "#{type} MAUI#{widgetName}_get#{name.myCapitalize}(MAUIWidget* w) {\n"
  ret << "\tMAUI#{widgetName}* #{widgetName[0,1].downcase} = (MAUI#{widgetName}*)w;\n"
  ret << "\treturn #{widgetName[0,1].downcase}->#{name};\n"
  ret << "}\n\n"
  ret
end

def initMember(name, m)
  case m[0]
    when 'char*' then "\t#{name}->#{m[1]} = malloc(MAX_STRING_LENGTH);\n"+
                      "\tmemset(#{name}->#{m[1]}, 0, MAX_STRING_LENGTH);\n"+
                      "\tstrcpy(#{name}->#{m[1]},"+(m[2] || "\"\"" ) +");\n"
    when 'int' then "\t#{name}->#{m[1]} = "+(m[2] || "0" )+";\n"
  end
end

# main program

print "Widget name (ex. Button) ? "
widgetName = gets.chomp

params = []

print "property? (type name default) "
while((str = gets.chomp) != "")
  print "property?  (type name default) "
  params << str.split(" ")
end

hText = ''
cText = ''


hText = <<END_OF_H
/*-- Copyright (C) 2004-2006 Mobile Sorcery AB --*/

#ifndef _MSAB_MAUI_#{widgetName.upcase}_H_
#define _MSAB_MAUI_#{widgetName.upcase}_H_

#include "MAUIWidget.h"

MAUIWidget* MAUI#{widgetName}_create(int x, int y, int width, int height);
void MAUI#{widgetName}_destroy(MAUIWidget* w);

#{params.collect {|p| "void MAUI"+widgetName+"_set"+p[1].myCapitalize+"(MAUIWidget* w, "+p[0]+" "+p[1]+");\n" }}#{params.collect {|p| p[0]+" MAUI"+widgetName+"_get"+p[1].myCapitalize+"(MAUIWidget* w);\n" }}

#endif

END_OF_H

cText = <<END_OF_C
#include "MAUI#{widgetName}.h"
#include "MAUIEngine.h"

#ifdef _WIN32
	#pragma warning (disable:4996)
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#endif

#define MAX_STRING_LENGTH 127
#define STREQ(a,b) (strcmp((a),(b))==0)
#ifndef MIN
  #define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#include <maapi.h>

/* ------------- "Inherited" struct -------------------------------------------- */

MAUIWIDGET_BEGIN(#{widgetName})
#{params.collect{|p|"\t"+p[0]+' '+p[1]+";\n"}}MAUIWIDGET_END(#{widgetName})

/* ------------- Implementation of "virtual" functions ------------------------- */


void MAUI#{widgetName}_draw(const MAUIWidget *w) {
	MAUI#{widgetName} *#{widgetName[0,1].downcase} = (MAUI#{widgetName}*)w;
	// TODO: add drawing code here
}

void MAUI#{widgetName}_destroy(MAUIWidget* w) {
	// TODO: add custom deallocation code here
	free(w);
}

void MAUI#{widgetName}_setProperty(MAUIWidget *w, const char *name, const char *value) {
	// TODO: Implement setting of properties
	MAUI#{widgetName} *#{widgetName[0,1].downcase} = (MAUI#{widgetName}*)w;
#{params[0,1].collect{|p|"\tif(STREQ(name, \"#{p[1]}\")) {\n\t\t#{setProp(widgetName[0,1].downcase,p[0],p[1])}\n\t}\n"}}
#{params[1,params.length-1].collect{|p|"\telse if(STREQ(name, \"#{p[1]}\")) {\n\t\t#{setProp(widgetName[0,1].downcase,p[0],p[1])}\n\t}\n\n"}}}

void MAUI#{widgetName}_getProperty(MAUIWidget *w, const char *name, char *buf) {
	// TODO: Implement getting of properties
	MAUI#{widgetName} *#{widgetName[0,1].downcase} = (MAUI#{widgetName}*)w;
#{params[0,1].collect{|p|"\tif(STREQ(name, \"#{p[1]}\")) {\n\t\t#{getProp(widgetName[0,1].downcase,p[0],p[1])}\n\t}\n"}}
#{params[1,params.length-1].collect{|p|"\telse if(STREQ(name, \"#{p[1]}\")) {\n\t\t#{getProp(widgetName[0,1].downcase,p[0],p[1])}\n\t}\n\n"}}}

MAUIWidgetVtable #{widgetName.downcase}Vtable = {
	MAUI#{widgetName}_draw,
	NULL, // TODO: custom MAUI#{widgetName}_contains (optional)
	NULL, // TODO: custom MAUI#{widgetName}_bounds (optional)
	NULL, // TODO: custom MAUI#{widgetName}_idle (optional)
	MAUI#{widgetName}_destroy,
	MAUI#{widgetName}_setProperty,
	MAUI#{widgetName}_getProperty
};

/* ------------- Implementation of "constructor" ------------------------------- */

MAUIWidget* MAUI#{widgetName}_create(int x, int y, int width, int height) {
	MAUI#{widgetName} *#{widgetName[0,1].downcase} = malloc(sizeof(MAUI#{widgetName}));
	MAUIWidget_init((MAUIWidget*)#{widgetName[0,1].downcase}, x, y, width, height);
#{params.collect {|p| initMember(widgetName[0,1].downcase, p) }}
	#{widgetName[0,1].downcase}->vtable = &#{widgetName.downcase}Vtable; 
	// TODO: add custom allocation and initialization code here

	return (MAUIWidget*) #{widgetName[0,1].downcase};
}

#{params.collect {|p| makeSetter(widgetName, p[0], p[1]) }}#{params.collect {|p| makeGetter(widgetName, p[0], p[1]) }}

END_OF_C

puts cText

path_h    = "MAUI" + widgetName + ".h";
path_c  = "MAUI" + widgetName + ".c";

hFile = File.new(path_h, 'w')
hFile.puts hText
hFile.close

cFile = File.new(path_c, 'w')
cFile.puts cText
cFile.close

#`svn add #{path_h}`
#`svn add #{path_c}`


