require File.expand_path(ENV['MOSYNCDIR'] + '/rules/util.rb')

PROFILES = [
'Nokia/5800',
'Nokia/E70',
'Nokia/N73',
'Sony-Ericsson/K610i',
'Sony-Ericsson/W950i',
'Sony-Ericsson/C902',
'HTC/Wildfire',
'HTC/Paradise',
'HTC/Desire',
]

PROFILES.each do |p|
	sh "ruby build-one.rb #{p}"
end
