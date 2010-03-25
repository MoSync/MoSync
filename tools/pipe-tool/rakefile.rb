BD = "../.."

require "#{BD}/build_rules/host.rb"

#---------------------------------------------------------------------------------
# SOURCES is a list of directories containing source code
# LIBRARIES is a list of libraries that this project needs
#---------------------------------------------------------------------------------
SOURCES = ["."]
IGNORED_FILES =	["Emu.c", "BrewRebuild.c", "Peeper.c", "JavaCodeGen.c", "disas.c"]
EXTRA_CFLAGS = " -Wno-shadow -Wno-write-strings -Wno-unused-function -Wno-multichar -D_CRT_SECURE_NO_DEPRECATE -DUSE_ZLIB"
LIBRARIES = ["z"]
if(HOST == "win32") then
	EXETARGET = "#{MOSYNCDIR}/bin/pipe-tool.exe"
else
	EXETARGET = "#{MOSYNCDIR}/bin/pipe-tool"
end


#---------------------------------------------------------------------------------
# actual build rules
#---------------------------------------------------------------------------------

require "#{BD}/build_rules/native.rb"

SOURCE_OBJECTS.each do |o| task o => "PBProto.h" end

file "PBProto.h" => ALL_SOURCEFILES do
	if(HOST == "win32") then
		sh "protobuild.exe"
	elsif(HOST == "linux") then
		sh "#{BUILDPATH}protobuild"
	else
		error("Unsupported platform!")
	end
end
