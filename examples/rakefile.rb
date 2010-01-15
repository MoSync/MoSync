BD = ".."
require "#{BD}/build_rules/host.rb"
require "#{BD}/build_rules/defaults.rb"

SUBDIRS = ["3dLines", 
           "MinUI",
           "MAUI/MAUIex",
           "AdvGraphics",
           "debugging",
           "MAStx",
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
           "OtaLoad"]
#TODO: MoRaw

require "../build_rules/subdirs_mosync.rb"
