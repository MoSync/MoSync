# Copyright (C) 2011 MoSync AB
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.

require 'stringio'
require 'rexml/document'
require 'pathname'
require "#{File.dirname(__FILE__)}/profiles.rb"
require "#{File.dirname(__FILE__)}/task.rb"
require "#{File.dirname(__FILE__)}/util.rb"

class ConvertToPngTask < FileTask
	# will preserve aspect ratio,
	# unless the attribute preserveAspectRatio="none" is set in the <svg> tag.
	def initialize(work, targetDir, resName, src, width, height)
		@width = width
		@height = height
		@src = src
		@resName = resName
		if(@src.to_s.getExt == '.svg')
			fixSvgAspectRatio
		end
		name = "#{targetDir}/#{File.basename(src).noExt}_#{@width}x#{@height}.png"
		super(work, name)
		@prerequisites += [src, DirTask.new(work, targetDir)]
	end
	def resName
		return @resName
	end
	def resType
		return "image"
	end
	def execute
		if(@src.to_s.getExt == '.svg')
			sh "java -jar \"#{mosyncdir}/bin/batik/batik-rasterizer.jar\" -w #{@width} -h #{@height}"+
				" -d \"#{File.expand_path(@NAME)}\" \"#{File.expand_path(@src)}\""
			if(!File.exist?(@NAME))	# in case batik fails silently.
				error('batik failed!')
			end
		else
			sh "#{mosyncdir}/bin/ImageMagick/convert -resize #{@width}x#{@height} -background none \"#{@src}\" \"#{@NAME}\""
		end
	end
	# Read the pixel dimensions of the SVG src and calculate the appropriate size.
	# Change @width and @height from maximum to actual.
	def fixSvgAspectRatio
		doc = REXML::Document.new(File.new(@src))
		doc.elements.each('svg') do |svg|
			@sw = svg.attributes['width'].to_s.to_i
			@sh = svg.attributes['height'].to_s.to_i
		end
		#p @sw
		#p @sh
		# try out each transformation, see which one fits
		ww = @width
		wh = (@width * @sh) / @sw
		hh = @height
		hw = (@height * @sw) / @sh
		#p ww
		#p wh
		#p hw
		#p hh
		if(wh > @height)
			error 'WHAT?!?' if(hw > @width)
			@width = hw
			@height = hh
		else
			error 'WHAT?!?' if(hw < @width)
			@width = ww
			@height = wh
		end
		#puts "Real size: #{@width}x#{@height}"
	end
end

class BMFontTask < FileTask
	def initialize(work, fontName, isBold, fontSize)
		baseName = "#{fontName}#{isBold ? '_bold' : ''}_#{fontSize}"
		@confName = "build/#{baseName}.bmfc"
		@fntFileName = "build/#{baseName}.fnt"
		@pngFileName = "build/#{baseName}_0.png"
		@badPngFileName = "build/#{baseName}_1.png"
		super(work, @fntFileName)
		templateFile = FileTask.new(work, 'bmfont.bmfc.template')
		@prerequisites = [templateFile]

		@buf = open(templateFile.to_s).read
		@buf.gsub!('%fontName%', fontName)
		@buf.gsub!('%fontSize%', fontSize.to_s)
		@buf.gsub!('%isBold%', isBold ? '1' : '0')
		@buf.gsub!('%bitmapHeight%', ((fontSize * fontSize) / 4).to_s)

		@ec = open(@confName).read if(File.exist?(@confName))
	end
	def needed?(log = true)
		return true if(super(log))
		if(@buf != @ec)
			puts "Because config has changed:" if(log)
			return true
		end
		if(!File.exist?(@pngFileName))
			puts "Because file does not exist: #{@pngFileName}"
			return true
		end
		return false
	end
	def execute
		file = open(@confName, 'w')
		file.write(@buf)
		file.close
		@ec = @buf

		FileUtils::rm_f(@badPngFileName)
		sh "#{mosyncdir}/bin/BMFont/bmfont -c \"#{@confName}\" -o \"#{@fntFileName}\""
		if(File.exist?(@badPngFileName))
			error 'Font bitmap overflow!'
		end
	end
end

class MofTask < FileTask
	def initialize(work, targetDir, id, fontName, isBold, fontSize, color)
		@bmTask = BMFontTask.new(work, fontName, isBold, fontSize)
		name = "#{targetDir}/#{File.basename(@bmTask).noExt}_#{color}.mof"

		@pngFileName = @bmTask.to_s.ext('_00.png');
		if(!File.exist?(@pngFileName))
			@pngFileName = @bmTask.to_s.ext('_0.png');
		end

		super(work, name)
		@prerequisites = [@bmTask, DirTask.new(work, targetDir)]
		@color = color
		@resName = "font_#{id}"
	end
	def resName
		return @resName
	end
	def resType
		return "ubin\n.include"
	end
	def execute
		sh "#{mosyncdir}/bin/mof -fontData \"#{@bmTask}\" -fontImage \"#{@pngFileName}\" -outFile \"#{@NAME}\" -fontColor #{@color}"
	end
end

class StaticImageTask < FileTask
	def initialize(work, src)
		super(work, src)
		@src = src
	end
	def resName
		return File.basename(@src).noExt
	end
	def resType
		return "image"
	end
end

class MediaTask < FileTask
	def initialize(work, src, mimeType)
		super(work, src)
		@src = src
		@mimeType = mimeType
	end
	def resName
		return File.basename(@src).noExt
	end
	def resType
		return "media \"#{@mimeType}\","
	end
end

class GeneratedLstTask < MemoryGeneratedFileTask
	# resources: Array of objects that respond to resName and resType,
	# such as ConvertToPngTask, MofTask, StaticImageTask and MediaTask.
	# profile: The result from Profiles.parse.
	def initialize(work, resources, profile)
		super(work, 'build/res.lst')
		@prerequisites << DirTask.new(work, 'build')
		@prerequisites += resources

		io = StringIO.new
		first = true
		io.write("// Screen size: #{profile['MA_PROF_CONST_SCREENSIZE_X']}x#{profile['MA_PROF_CONST_SCREENSIZE_Y']}\n")
		io.write("\n")
		resources.each do |c|
			if(first)
				first = false
			else
				io.write("\n")
			end
			io.write(".res RES_#{c.resName.upcase.gsub(".", "_")}\n")
			io.write(".#{c.resType} \"#{Pathname.new(c.to_s).relative_path_from(Pathname.new('build'))}\"\n")
		end
		@buf = io.string
	end
end
