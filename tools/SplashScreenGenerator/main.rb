#!/usr/bin/ruby
# encoding: UTF-8

# the above line, although it looks like a comment, is actually parsed by the Ruby engine to set
# the default String encoding for this program.

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

# This script generates a splash screen for the IDE, using RMagick operatations

require 'rubygems'
require 'RMagick'

SV = RUBY_VERSION.split('.')
if(SV[0] == '1' && SV[1] == '8')
	$KCODE = "UTF8"
end

HEADER_TEXT = 'MoSync mobile development SDK'

version = ['Developer build', 'Unknown']

PLATFORMS = [
	'Android 1.5, 1.6, 2.1',
	'iOS (iPhone, iPad, iPod Touch) 3.x',
	'Java ME MIDP 2',
	'Moblin 2.x',
	'Symbian S60 2nd, 3rd, 5th edition',
	'Windows Mobile 5.0 - 6.5',
	'Windows Pocket PC 2003',
	'Windows Smartphone 2003'
]

if ARGV.length() == 2
	version[0] = ARGV[0] # MoSync SDK version
	version[1] = ARGV[1] # SVN revision
else
	index = 0
	File.open('\mb\revision', 'r') do |f|
		while (line = f.gets)
			version[index] = line
			index = index + 1
		end
	end
end

buildNightly = false
nightly = ENV['NIGHTLY']
if nightly.class != NilClass
	buildNightly = true
end

COPYRIGHT = "Copyright © 2004-#{Time.new.year.to_s}. All rights reserved. " + 
            "MoSync and the MoSync logo are registered trademarks of MoSync AB."

img = Magick::Image.read('template.png').first
img2 = Magick::Image.read('template_installer.png').first
img_gs = Magick::Image.read('template2.png').first
img2_gs = Magick::Image.read('template_installer2.png').first

header = Magick::Draw.new

header.annotate(img, 271, 200, 275, 110, HEADER_TEXT) do
	self.font = 'MyriadPro-Bold'
	self.pointsize = 19
	self.font_weight = Magick::BoldWeight
	self.fill = 'white'
	self.gravity = Magick::NorthWestGravity
end

header.annotate(img_gs, 271, 200, 275, 110, HEADER_TEXT) do
	self.font = 'MyriadPro-Bold'
	self.pointsize = 19
	self.font_weight = Magick::BoldWeight
	self.fill = 'white'
	self.gravity = Magick::NorthWestGravity
end


if(buildNightly == true)
	puts "Injecting string to images : Nightly Build r#{version[1].strip}"
	header.annotate(img, 271, 340, 275, 130, "Nightly Build r#{version[1].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 14
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end

	header.annotate(img2, 271, 340, 180, 260, "Nightly Build r#{version[1].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 12
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end
	
	header.annotate(img_gs, 271, 340, 275, 130, "Nightly Build r#{version[1].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 14
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end
		
	header.annotate(img2_gs, 271, 340, 180, 260, "Nightly Build r#{version[1].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 12
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end
	
else
	puts "Injecting string to images : Version #{version[0].strip}"
	header.annotate(img, 271, 340, 275, 130, "Version #{version[0].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 14
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end

	header.annotate(img2, 271, 340, 180, 260, "Version #{version[0].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 12
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end
	
	header.annotate(img_gs, 271, 340, 275, 130, "Version #{version[0].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 14
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end
	
	header.annotate(img2_gs, 271, 340, 180, 260, "Version #{version[0].strip}" ) do
		self.font = 'Verdana'
		self.pointsize = 12
		self.font_weight = Magick::LighterWeight
		self.fill = 'white'
		self.gravity = Magick::NorthWestGravity
	end
end
	
platforms = Magick::Image.read("caption:#{PLATFORMS.join(', ')}") do 
	self.size = "300x200"
	self.pointsize = 12
	self.fill = '#b0b0b0'
	self.background_color = '#00000000'
end

copyright = Magick::Image.read("caption:#{COPYRIGHT}") do 
	self.size = "600x100"
	self.pointsize = 11
	self.fill = '#b0b0b0'
	self.background_color = '#00000000'
end

img = img.composite(platforms.first, 275, 160, Magick::ScreenCompositeOp)
img = img.composite(copyright.first, 20, 358, Magick::ScreenCompositeOp)

img_gs = img_gs.composite(platforms.first, 275, 160, Magick::ScreenCompositeOp)
img_gs = img_gs.composite(copyright.first, 20, 358, Magick::ScreenCompositeOp)

img.write('bmp3:splash.bmp')
img2.write('bmp3:installer_splash.bmp')
img.write('png24:userguide_start_1.png')
img_gs.write('png24:userguide_start_3.png')
img2_gs.write('png24:userguide_install_2.png')
