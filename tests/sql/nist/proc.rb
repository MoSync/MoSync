# This is a Pro*C-to-C compiler, designed for the NIST test suite.
# Its output should be adaptable to any SQL library,
# such as sqlite, MySQL or MoSync.

#require File.expand_path('../../../rules/util.rb')

Var = Struct.new(:type, :name)

# name => type
# Populated by C variable declarations inside DECLARE sections.
VAR_TYPES = {}

input = open('basetab.pc', 'r')
output = open('basetab.c', 'w')

istr = input.read

pos = 0
while(pos < istr.length)
	execIndex = istr.index('EXEC SQL ', pos)
	if(!execIndex)	# we've reached the end.
		output << istr.slice(pos .. -1)
		break
	end
	output << istr.slice(pos, execIndex - pos)
	execIndex += 'EXEC SQL '.length
	semiColonIndex = istr.index(';', execIndex)
	raise 'SYNTAX ERROR' if(!semiColonIndex)
	sqlCode = istr.slice(execIndex, semiColonIndex - execIndex)
	pos = semiColonIndex + 1

	if(sqlCode == 'BEGIN DECLARE SECTION')
		endIndex = istr.index('EXEC SQL END DECLARE SECTION;', pos)
		declarations = istr.slice(pos, endIndex - pos)
		#p declarations
		a = declarations.scan(/(\w+) (\w+).*;/)
		#p a
		a.each do |v|
			VAR_TYPES[v[1]] = v[0].capitalize
		end
		next
	end
	next if(sqlCode == 'END DECLARE SECTION' || sqlCode.start_with?('COMMIT'))

	if(sqlCode.start_with?('SELECT'))
		# Syntax is like SELECT ... INTO :var1, :var2, et.al FROM ...
		# So for now, I assume everything between INTO and FROM are variable names.
		# Since C SQL APIs don't support that syntax, the INTO section will be removed.
		# Data will be retrieved via the API.
		intoIndex = sqlCode.index('INTO')
		raise 'SYNTAX ERROR' if(!intoIndex)
		fromIndex = sqlCode.index('FROM', intoIndex)
		raise 'SYNTAX ERROR' if(!fromIndex)
		#p sqlCode
		#p intoIndex
		#p fromIndex
		#p sqlCode.slice(0, intoIndex)
		#p sqlCode.slice(fromIndex .. -1)

		# parse variables
		varIndex = intoIndex + 'INTO'.length
		varString = sqlCode.slice(varIndex, fromIndex - varIndex)
		#p varString
#		vars = []
#		vpos = 0
#		while(vpos < varString.length)
#			def eatWhitespace
#				while(varString[vpos,1] ==
#			eatWhitespace
#		end
		varStrings = varString.split(' ')
		#p varStrings
		vars = varStrings.collect do |vs|
			raise 'SYNTAX ERROR' if(vs[0,1] != ':')
			name = vs[1..-1].chomp(',')
			Var.new(VAR_TYPES[name], name)
		end

		sqlCode = sqlCode.slice(0, intoIndex) + sqlCode.slice(fromIndex .. -1)
		output << "{\n"
		output << "\tSqlSelectResults res = sqlSelect(\"" << sqlCode.gsub("\n", "\"\n\"") << "\");\n"
		vars.each_with_index do |var, i|
			output << "\t#{var.name} = sqlGet#{var.type}(res, #{i});\n"
		end
		output << "\tsqlFinalize(res);\n"
		output << "}\n"
	else
		output << 'sqlExec("' << sqlCode.gsub("\n", "\"\n\"") << '");'
	end
end
