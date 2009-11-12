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

require "#{BD}/build_rules/pipe.rb"

# CLASSPATH should contain API definitions suitable for preverification.
# STATIC_JAR should contain a MoSync runtime JAR compiled with Static Java support.
# PREVERIFY should contain the name of the preverifier executable.

CLASSPATH = ENV['CLASSPATH']
STATIC_JAR = ENV['STATIC_JAR']
PREVERIFY = ENV['PREVERIFY']

def build_static_java(t)
	sh "cd #{BUILDDIR} && #{PIPETOOL}#{PIPEFLAGS} -java" +
		" \"-gcj=\\\"--CLASSPATH=#{CLASSPATH};#{STATIC_JAR}\\\"\"" +
		" -sld -collect-stabs -B tempProgram #{expand_paths(t.prerequisites)}"
	rm "#{BUILDDIR}ms.class"
	#sh(cmd) do |ok, res|
		#if(!ok)
			#error("pipe-tool failure: #{res}")
		#end
	#end
end

def preverify()
	sh "cd #{BUILDDIR} && \"#{PREVERIFY}\" -classpath \"#{CLASSPATH};#{STATIC_JAR}\" -d . ."
end

# inserts srcFile into srcJar, stores result in dst.
def jarInsert(dst, srcJar, srcFile)
	sh "cp #{srcJar} #{dst}"
	sh "zip -j #{dst} #{srcFile}"
end

class StaticJava
	def StaticJava.invoke
		unverifiedClassfile = "#{BUILDDIR}StaticCode.class"
		file unverifiedClassfile => OBJECTS do |t|
			build_static_java(t)
		end
		#preverify
		verifiedClassfile = "#{BUILDDIR}output/StaticCode.class"
		file verifiedClassfile => unverifiedClassfile do
			preverify()
		end
		#optimize	#optional
		
		#pack into JAR
		jarFile = "#{BUILDDIR}#{EXENAME}.jar"
		jarTask = file jarFile => verifiedClassfile do
			jarInsert(jarFile, STATIC_JAR, "#{BUILDDIR}*.class #{BUILDDIR}data_section.bin")
		end
		jarTask.invoke
	end
end

class BlackBerryStaticJava
	def BlackBerryStaticJava.invoke
		#rapc jar -> cod
		codFile = "#{BUILDDIR}#{EXENAME}.cod"
		codTask = file codFile => "#{BUILDDIR}#{EXENAME}.jar" do |t|
			sh "rapc -nowarn \"-import=#{CLASSPATH}\" -midlet #{t.prerequisites}"
		end
		codTask.invoke
		#javaloader
		sh "javaloader load #{codFile}"
	end
end
