#!/usr/bin/ruby

# Verifies that git configs "user.name" and "user.email" have acceptable values.

require 'fileutils'
require File.expand_path("#{File.dirname(__FILE__)}/../rules/util.rb")

name = open('|git config user.name').read.strip
email = open('|git config user.email').read.strip

if(!name || name.length == 0)
	raise "Please set git config user.name"
end

if(!email || email.length == 0)
	raise "Please set git config user.email"
end

if(email.endsWith('.local') || email.include?('(none)') || !email.include?('@'))
	puts "user.email: "+email
	raise "Invalid git config user.email. Please fix it."
end
