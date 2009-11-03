BD = "../.."

require "#{BD}/rules/native_lib.rb"

idlc = NativeLibWork.new
class << idlc
	@SOURCES = ["."]
	@NAME = "idl-common"
end

idlc.invoke
