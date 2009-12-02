
module FlagsChanged
	def initFlags
		# save cflags to disk, for use as dependency.
		# but first read it back from disk, if existing.
		@FLAGSFILE = @NAME + ".flags"
		if(File.exists?(@FLAGSFILE)) then
			@OLDFLAGS = open(@FLAGSFILE) { |f| f.read }
		end
		#puts "Oldflags: #{@OLDFLAGS.inspect}"
	end
	
	def execFlags
		if(@OLDFLAGS != @FLAGS) then
			open(@FLAGSFILE, 'w') { |f| f.write(@FLAGS) }
		end
	end
	
	def flagsNeeded?(log=true)
		#puts "Oldflags: #{@OLDFLAGS.inspect} newflags: #{@FLAGS.inspect}"
		if(@OLDFLAGS != @FLAGS)
			puts "Because the flags have changed:" if(log)
			return true
		end
		return false
	end
end
