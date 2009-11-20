require File.expand_path('../../rules/native_mosync.rb')

work = MoSyncExe.new
work.instance_eval do
	@SOURCES = ["."]
	@IGNORED_FILES =	["Emu.c", "BrewRebuild.c", "Peeper.c", "JavaCodeGen.c", "disas.c"]
	@EXTRA_CFLAGS = " -Wno-strict-prototypes -Wno-missing-prototypes -Wno-old-style-definition" +
		" -Wno-missing-noreturn -Wno-shadow -Wno-unreachable-code -Wno-write-strings -Wno-multichar" +
		" -Wno-missing-format-attribute -D_CRT_SECURE_NO_DEPRECATE -DUSE_ZLIB"
	# -Wno-unused-function
	@LIBRARIES = ["z"]
	@NAME = "pipe-tool"
	
	setup
	
	so = @all_sourcefiles
	cb = @COMMON_BUILDDIR
	protoTask = FileTask.new(self, "PBProto.h")
	protoTask.instance_eval do
		@prerequisites = so
		@COMMON_BUILDDIR = cb
		def execute
			puts @COMMON_BUILDDIR
			sh "#{@COMMON_BUILDDIR}protobuild"
		end
	end
	@prerequisites = [protoTask] + @prerequisites
end

work.invoke
