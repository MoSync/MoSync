
# add functions to this class to allow them to be used as command-line targets
# if no command-line target is chosen, "default" is run.
class Targets
	class Target
		def initialize(name, preqs, &block)
			@name = name
			@preqs = preqs
			@block = block
		end
		
		def invoke
			#puts "preqs of '#{@name}'"
			@preqs.each do |p| p.invoke end
			#puts "block of '#{@name}'"
			@block.call
		end
	end
	
	@@targets = {}
	
	def Targets.add(args, &block)
		case args
		when Hash
			fail "Too Many Task Names: #{args.keys.join(' ')}" if args.size > 1
			fail "No Task Name Given" if args.size < 1
			name = args.keys[0]
			preqs = args[name]
			preqs = [preqs] if (String===preqs) || (Regexp===preqs) || (Proc===preqs) || (Symbol===preqs)
			preqs = preqs.collect do |p| @@targets[p] end
		else
			name = args
			preqs = []
		end
		@@targets.store(name, Target.new(name, preqs, &block))
	end
	
	# parse ARGV
	def Targets.setup
		@@goals = [:more]	#temp hack
	end
	
	def Targets.invoke
		@@goals.each { |t| @@targets[t].invoke }
	end
end

Targets.setup

def target(args, &block)
	Targets.add(args, &block)
end
