#!/usr/bin/ruby

require 'fileutils'
require 'settings.rb'
require "#{MOSYNC_SRC}rules/util.rb"

extend FileUtils::Verbose

# We'll need source code for MoSync, Eclipse and GCC.
# We'll need a target dir.

TARGET_MOSYNC = TARGET_BASE + 'mosync/'
TARGET_DEB = TARGET_MOSYNC + 'debian/'
TARGET = TARGET_DEB + 'usr/mosync/'

# We'll need to know which revision we're building.
# At the moment, we use SVN. Since we have separate checkouts for main and eclipse,
# we need to synchronize them to use the same revision. We'll use main's current revision as base.
# This will fail if main has had partial commits done. 
# Fix by running svn update in main's root directory (MOSYNC_SRC).
open("|svn info #{MOSYNC_SRC}").each do |line|
	rlb = 'Revision: '
	if(line.beginsWith(rlb))
		revString = line[rlb.length..line.length]
		if(revString =~ /\A\d+\Z/)
			REVISION = revString.to_i
		else
			error "Bad revision: #{revString}"
		end
	end
end

# We'll also need to know the major/minor version number
file = open("#{MOSYNC_SRC}tools/ReleasePackageBuild/major_minor_revision.txt")
MAJOR = file.gets.strip
MINOR = file.gets.strip
MOSYNC_VERSION = "#{MAJOR}.#{MINOR}"

puts "Building version #{MOSYNC_VERSION} r#{REVISION}"

if(false)
# Start by cleaning out any previous build.
rm_r(TARGET_BASE)

# Then construct our target directory structure.
def mktargetdir(subdir)
	mkdir_p(TARGET + subdir)
end

mktargetdir ''
mktargetdir 'bin'
mktargetdir 'etc'
mktargetdir 'libexec/gcc/mapip/3.4.6'
mktargetdir 'docs'

# Copy profiles.
cd(TARGET)
sh("tar xf #{PROFILES}")
sh('chmod -R +x profiles')	# Make directories readable.

# Build gcc.
cd(GCC_SRC)
if(!File.exist?('build/gcc/Makefile'))	# Configure takes some time, so do it only if it isn't already done.
	sh('./configure-linux.sh')
end
cd('build/gcc')
# This command WILL return an error code. Check for the presence of target files to ascertain success.
cmd = 'make all-gcc'
puts cmd
system cmd

# Copy gcc to target
cp('gcc/xgcc', TARGET + 'bin')
cp('gcc/cpp', TARGET + 'bin')
cp('gcc/cc1', TARGET + 'libexec/gcc/mapip/3.4.6')
cp('gcc/cc1plus', TARGET + 'libexec/gcc/mapip/3.4.6')

# Build the main part of MoSync
cd(MOSYNC_SRC)
ENV['MOSYNCDIR'] = TARGET
sh('ruby workfile.rb CONFIG=""')

# Build documentation
cp("#{MOSYNC_SRC}libs/Doxyfile", "#{TARGET}include/Doxyfile")
cd("#{TARGET}include")
ENV['MOSYNC_DOXYGEN_OUTPUT_DIR'] = "#{TARGET}docs"
sh('doxygen')

# Generate splash screen
cd("#{MOSYNC_SRC}tools/SplashScreenGenerator")
sh("ruby main.rb #{VERSION} #{REVISION}")
cp('splash.bmp', "#{ECLIPSE_SRC}com.mobilesorcery.sdk.product/")
cp('about.png', "#{ECLIPSE_SRC}com.mobilesorcery.sdk.product/")

# Build MoSync IDE
cd("#{ECLIPSE_SRC}com.mobilesorcery.sdk.product/build")
if(!File.exist?('target-platform.zip'))
	ln(ECLIPSE_PLATFORM, 'target-platform.zip')
end
sh('ant -l ant-log.txt release')
mv('buildresult/I.MoSync/MoSync-linux.gtk.x86-unzipped/mosync', TARGET + 'eclipse')

# Copy extra files
def copy_bin(name)
	cp("#{MOSYNC_SRC}tools/ReleasePackageBuild/build_package_tools/mosync_bin/#{name}", "#{TARGET}bin")
end
copy_bin('openssl.cnf')
copy_bin('unifont-5.1.20080907.ttf')
end

# Build DEB package
# sed control.tmpl > TARGET_DEB/control
INSTALLED_SIZE = open("|du -s #{TARGET}").gets("\t").strip
mkdir_p("#{TARGET_DEB}DEBIAN")
sh("sed -e 's/$version/#{MOSYNC_VERSION}_r#{REVISION}/' -e 's/$installed-size/#{INSTALLED_SIZE}/'"+
	"< #{MOSYNC_SRC}tools/ReleasePackageBuild/linux/control.tmpl > #{TARGET_DEB}/DEBIAN/control")
# Then call dpkg
cd(TARGET_MOSYNC)
sh('dpkg-deb --build debian')
