#!/usr/bin/ruby

require File.expand_path(ENV['MOSYNCDIR'] + '/rules/mosync_resources.rb')
require File.expand_path(ENV['MOSYNCDIR'] + '/rules/mosync_exe.rb')
require File.expand_path(ENV['MOSYNCDIR'] + '/rules/util.rb')
require './profile.rb'

# Create the work object, which collects all tasks.
@work = PipeExeWork.new

# Generate resources based on chosen device profile.
P = Profiles.parse(PROFILE)
# P is now a hash of all the profile info.

# Parse screen size.
w = P['MA_PROF_CONST_SCREENSIZE_X'].to_i
h = P['MA_PROF_CONST_SCREENSIZE_Y'].to_i
error "Missing profile information: MA_PROF_CONST_SCREENSIZE_X" if(w <= 0)
error "Missing profile information: MA_PROF_CONST_SCREENSIZE_y" if(h <= 0)
minSize = min(w, h)

BUILDRESDIR = 'build/res'

# helper functions
def font(id, name, bold, size, color)
	MofTask.new(@work, BUILDRESDIR, id, name, bold, size, color)
end
def scale(name, src, width, height)
	ConvertToPngTask.new(@work, BUILDRESDIR, name, FileTask.new(self, 'res/'+src), width, height)
end
def static(src)
	StaticImageTask.new(@work, FileTask.new(self, 'res/'+src))
end
def media(src, mimeType)
	MediaTask.new(@work, FileTask.new(self, 'res/'+src), mimeType)
end



# Define fonts.
MINIMUM_FONT_SIZE = 12
bigFontSize = max(minSize / 12, MINIMUM_FONT_SIZE)
smallFontSize = max(minSize / 16, MINIMUM_FONT_SIZE)
FONTS = [
	font('arial', 'Arial', false, bigFontSize, '222222'),
	font('courier', 'Courier New', false, smallFontSize, '000000'),
]

# Define images.
SCALED_IMAGES = [
	scale('combo5', 'combo_plain.svg', minSize/5, minSize/5),
	scale('combo10', 'combo_plain.svg', minSize/10, minSize/10),
]

STATICS = [
	static('spinner_anim.png'),
	media('mobilesorcery2.mp3', 'audio/mpeg'),
]



# Collect all the resource tasks.
ALL_RESOURCES = SCALED_IMAGES + FONTS + STATICS

lstTask = GeneratedLstTask.new(@work, ALL_RESOURCES, P)

# Define the Work.
@work.instance_eval do
	@resourceTask = PipeTask.new(self, 'build/resources', [lstTask], ' -xerr -R')
	@SOURCES = ['src']
	@EXTRA_INCLUDES = ["#{mosyncdir}/profiles/vendors/#{PROFILE}", '.', 'build']
	@LIBRARIES = ['mautil', 'maui']

	stack = 16*1024
	heap = 64*1024
	data = heap + stack + 128*1024
	@EXTRA_LINKFLAGS = " -datasize=#{data} -stacksize=#{stack} -heapsize=#{heap}"

	@NAME = 'GenerateResources'
	@PACK_MODEL = PROFILE
	@PACK_PARAMETERS = ' --s60v3uid ed784a93 --s60v2uid 05b99094'
end

# Define command-line targets.
target :default do
	@work.invoke
end

target :lst do
	lstTask.invoke
end

# Invoke the chosen target.
Targets.invoke
