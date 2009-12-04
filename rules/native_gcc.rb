require "#{File.dirname(__FILE__)}/gcc.rb"

# Base class.
class NativeGccWork < GccWork
	def gcc; "gcc"; end
	def gccmode; "-c"; end
	def object_ending; ".o"; end
end
