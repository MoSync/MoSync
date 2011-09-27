#!/usr/bin/ruby
# encoding: UTF-8

# the above line, although it looks like a comment, is actually parsed by the
# Ruby engine to set the default String encoding for this program.

# Copyright (C) 2011 MoSync AB
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

HEADER_TEXT = 'MoSync SDK'

version = ['Developer build', 'Unknown']

PLATFORMS = [
	'Android 1.5 - 2.3.3',
	'iOS (iPhone, iPad, iPod Touch) 3.0 - 4.3',
	'Java ME MIDP 2',
	'Moblin 2.x',
	'Symbian S60 2nd, 3rd, 5th edition',
	'Windows Mobile 5.0 - 6.5',
	'Windows Pocket PC 2003',
	'Windows Smartphone 2003'
]

if ARGV.length() == 4
	version[0] = ARGV[0] # MoSync SDK name
	version[1] = ARGV[1] # MoSync SDK date
	version[2] = ARGV[2] # git mosync hash
	version[3] = ARGV[3] # git eclipse hash
elsif ARGV.length() > 0
	version[0] = ARGV[0] # MoSync SDK name
	version[1] = ARGV[1] # MoSync SDK date
	version[2] = ""
	version[3] = ""
else
	raise "Splash Screen Generator error - no arguments provided"
end

COPYRIGHT = "Copyright © 2004-#{Time.new.year.to_s}. All rights reserved. " + 
           "MoSync and the MoSync logo are registered trademarks of MoSync AB."

img = Magick::Image.read('template.png').first
img2 = Magick::Image.read('template_installer.png').first

header = Magick::Draw.new

header.annotate(img, 271, 200, 275, 110, HEADER_TEXT) do
	self.font = 'MyriadPro-Bold'
	self.pointsize = 19
	self.font_weight = Magick::BoldWeight
	self.fill = 'white'
	self.gravity = Magick::NorthWestGravity
end


puts "Injecting string to images : Version #{version[0].strip}"

# Eclipse Splash

header.annotate(img, 271, 340, 275, 130, "#{version[0].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 14
	self.font_weight = Magick::LighterWeight
	self.fill = 'white'
	self.gravity = Magick::NorthWestGravity
end

header.annotate(img, 271, 340, 20, 314, "MoSync #{version[2].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 10
	self.font_weight = Magick::LighterWeight
	self.fill = '#b0b0b0'
	self.gravity = Magick::NorthWestGravity
end

header.annotate(img, 271, 340, 20, 326, "Eclipse #{version[3].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 10
	self.font_weight = Magick::LighterWeight
	self.fill = '#b0b0b0'
	self.gravity = Magick::NorthWestGravity
end

header.annotate(img, 271, 340, 495, 326, "#{version[1].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 10
	self.font_weight = Magick::LighterWeight
	self.fill = '#b0b0b0'
	self.gravity = Magick::NorthWestGravity
end


# Installer Splash

header.annotate(img2, 271, 340, 80, 230, "#{version[0].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 12
	self.font_weight = Magick::LighterWeight
	self.fill = 'white'
	self.gravity = Magick::NorthWestGravity
end

header.annotate(img2, 271, 340, 80, 245, "MoSync #{version[2].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 10
	self.font_weight = Magick::LighterWeight
	self.fill = '#b0b0b0'
	self.gravity = Magick::NorthWestGravity
end

header.annotate(img2, 271, 340, 80, 257, "Eclipse #{version[3].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 10
	self.font_weight = Magick::LighterWeight
	self.fill = '#b0b0b0'
	self.gravity = Magick::NorthWestGravity
end

header.annotate(img2, 271, 340, 80, 269, "#{version[1].strip}" ) do
	self.font = 'Verdana'
	self.pointsize = 10
	self.font_weight = Magick::LighterWeight
	self.fill = '#b0b0b0'
	self.gravity = Magick::NorthWestGravity
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

img.write('bmp3:splash.bmp')
img2.write('bmp3:installer_splash.bmp')
