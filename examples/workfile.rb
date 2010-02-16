#!/usr/bin/ruby

require File.expand_path('../rules/task.rb')

SUBDIRS = [
	"3dLines", 
	"AdvGraphics",
	"debugging",
	"HelloMap",
	"location",
	"MapDemo",
	"MAStx",
	"MAUI/MAUIex",
	"MinUI",
	"Moblet/Connection",
	"Moblet/HelloMoblet",
	"Moblet/simple",
	"Moblet/Soap",
	"Moblet/Stylus",
	"Moblet/timers",
	"MoSketch",
	"MoSound",
	"MoTooth",
	"MoTris",
	"MoTrix",
	"OtaLoad",
]

target :pipe do
	Work.invoke_subdirs(SUBDIRS, 'pipe')
end

target :default => :pipe

target :clean do
	Work.invoke_subdirs(SUBDIRS, 'clean')
end

Targets.invoke
