#!/bin/bash
#
# Usage:
# build.sh <branch> <MOSYNC_HASH>
#
# If no arguments are specified, a nightly build is started.
#

# Error exit function
function error_exit
{
	echo "$1" 1>&2
	exit 1
}

#
# Start
#
echo "----------------------------------------------------"
echo "Starting..."
echo "----------------------------------------------------"

# Set the working directory
script_dir=$(pwd)
# Default values for NIGHTLY build
BUILDTYPE=$(echo "nightly")
BRANCH=$(echo "master")
# Get the date
DATE=`date "+%y%m%d-%H%M"`

MOSYNCDIR=$(echo "$script_dir/mac_package_resources/MoSync-pkg/$BUILDTYPE")
SOURCEDIR=$(echo "$script_dir/../../")
ECLIPSEDIR=$(echo "$script_dir/../../../Eclipse")
RESULTDIR=$(echo "$script_dir/../../../result")
#
# Create package hierarchy
#
echo "----------------------------------------------------"
echo "Creating package hierarchy"
echo "----------------------------------------------------"

mkdir $MOSYNCDIR/
mkdir $MOSYNCDIR/bin
mkdir $MOSYNCDIR/bin/freetype219
mkdir $MOSYNCDIR/bin/freetype219/lib
mkdir $MOSYNCDIR/etc
mkdir $MOSYNCDIR/bin/ImageMagick
mkdir $MOSYNCDIR/libexec
mkdir $MOSYNCDIR/libexec/gcc
mkdir $MOSYNCDIR/libexec/gcc/mapip
mkdir $MOSYNCDIR/libexec/gcc/mapip/3.4.6
mkdir $MOSYNCDIR/docs
mkdir $MOSYNCDIR/examples
mkdir $MOSYNCDIR/examples/Moblet
mkdir $MOSYNCDIR/examples/MAUI
mkdir $RESULTDIR

# If no hash is specified, guess it
cd $SOURCEDIR
MOSYNC_HASH=`/usr/local/git/bin/git rev-parse HEAD`
# Guess Eclipse hash
cd $ECLIPSEDIR
ECLIPSE_HASH=`/usr/local/git/bin/git rev-parse HEAD`
# Generate a version text file
cd $SOURCEDIR
/usr/bin/touch VERSION.TXT
echo "$BUILDTYPE build" >> VERSION.TXT
echo "$DATE" >> VERSION.TXT
echo "$MOSYNC_HASH" >> VERSION.TXT
echo "$ECLIPSE_HASH" >> VERSION.TXT



echo "----------------------------------------------------"
echo "Installing gcc in $MOSYNCDIR"
echo "----------------------------------------------------"
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/xgcc $MOSYNCDIR/bin 
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/cpp $MOSYNCDIR/bin 
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/libexec/gcc/mapip/3.4.6/cc1 $MOSYNCDIR/libexec/gcc/mapip/3.4.6
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/libexec/gcc/mapip/3.4.6/cc1plus $MOSYNCDIR/libexec/gcc/mapip/3.4.6

#
# Generate splash screen
#
echo "----------------------------------------------------"
echo "Generating splash screen"
echo "----------------------------------------------------"
cd $SOURCEDIR/tools/SplashScreenGenerator
if /opt/local/bin/ruby main.rb "$BUILDTYPE build $DATE" $MOSYNC_HASH $ECLIPSE_HASH; then
	cp splash.bmp $ECLIPSEDIR/com.mobilesorcery.sdk.product/
	cp about.png $ECLIPSEDIR/com.mobilesorcery.sdk.product/
else
	error_exit "Could not generate splashscreen."
fi


#
# Build MoSync IDE in the background
#
echo "----------------------------------------------------"
echo "Building MoSync IDE"
echo "----------------------------------------------------"

cd $ECLIPSEDIR/com.mobilesorcery.sdk.product/build
cp $script_dir/target-platform.zip $ECLIPSEDIR/com.mobilesorcery.sdk.product/build/
ant release 

#
# Build MoSync
#
echo "----------------------------------------------------"
echo "Building MoSync"
echo "----------------------------------------------------"
cd $SOURCEDIR
./workfile.rb || error_exit "Could not build MOSYNC with DEBUG config."
./workfile.rb CONFIG="" || error_exit "Could not build MOSYNC with RELEASE config."
./workfile.rb USE_NEWLIB=1 || error_exit "Could not build NEWLIB with DEBUG config."
./workfile.rb CONFIG="" USE_NEWLIB=1 || error_exit "Could not build NEWLIB with RELEASE config."

# splash screen has been moved from here

#
# Copy dynamic libraries
#
echo "----------------------------------------------------"
echo "Copying dynamic libraries"
echo "----------------------------------------------------"
cp /sw/lib/libFLAC.8.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libSDL-1.2.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libSDL_image-1.2.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libSDL_sound-1.0.1.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libSDL_ttf-2.0.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libcrypto.0.9.8.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libexpat.1.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libfreeimage.3.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/freetype219/lib/libfreetype.6.dylib $MOSYNCDIR/bin/freetype219/lib || error_exit "Could not copy dynamic library."
cp /sw/lib/libjpeg.62.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libmikmod.3.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libmodplug.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libogg.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libpng12.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libsmpeg-0.4.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libspeex.1.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libssl.0.9.8.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libtiff.3.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libvorbis.0.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libvorbisfile.3.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libz.1.2.4.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/liblcms.1.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libjasper.1.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libdjvulibre.15.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libbz2.1.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."
cp /sw/lib/libiconv.2.dylib $MOSYNCDIR/bin || error_exit "Could not copy dynamic library."

cp /opt/local/lib/libMagickCore.3.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libMagickWand.3.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/liblcms.1.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libtiff.3.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libjpeg.8.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libfontconfig.1.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libfreetype.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libiconv.2.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libXext.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libXt.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libltdl.7.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libltdl.7.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libSM.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libICE.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libX11.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libXau.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."
cp /opt/local/lib/libXdmcp.6.dylib $MOSYNCDIR/bin/ImageMagick/ || error_exit "Could not copy dynamic library."



chmod -R +w $MOSYNCDIR/bin/*

#
# Copy binaries
#
echo "----------------------------------------------------"
echo "Copying binaries"
echo "----------------------------------------------------"
cp $(which zip) $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $(which unzip) $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $(which c++filt) $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp /opt/local/bin/openssl $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp /opt/local/bin/convert $MOSYNCDIR/bin/ImageMagick || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/mosync_bin/openssl.cnf $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/mosync_bin/unifont-5.1.20080907.ttf $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp -R $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/BMFont $MOSYNCDIR/bin/ || error_exit "Could not copy binary."
cp -R $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/bin/Batik $MOSYNCDIR/bin/ || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/*.dll $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/updater* $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/lcab $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/pcab.pl $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/mof* $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/iphoneos/iphonesim $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/tools/idl2/output/asm_config.lst $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp $SOURCEDIR/build/release/e32hack $MOSYNCDIR/bin || error_exit "Could not copy binary."
cp -R $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/osx_bin/android $MOSYNCDIR/bin/ || error_exit "Could not copy binary."
cp $SOURCEDIR/runtimes/java/platforms/android/mosync.keystore $MOSYNCDIR/etc || error_exit "Could not copy binary."
cp $SOURCEDIR/runtimes/java/platforms/android/AndroidProject/res/drawable/icon.png $MOSYNCDIR/etc || error_exit "Could not copy binary."

mv $MOSYNCDIR/bin/moemu $MOSYNCDIR/bin/MoRE

#
# Changing dynamic libraries paths
#
echo "----------------------------------------------------"
echo "Changing dynamic library paths"
echo "----------------------------------------------------"
cd $script_dir
python convert.py /sw/lib @loader_path $MOSYNCDIR/bin/*.dylib || error_exit "Could not change dylib path."
python convert.py /opt/local/lib @loader_path $MOSYNCDIR/bin/*.dylib || error_exit "Could not change dylib path."
python convert.py /sw/lib @loader_path $MOSYNCDIR/bin/MoRE || error_exit "Could not change dylib path."
python convert.py /opt/local/lib @loader_path $MOSYNCDIR/bin/MoRE || error_exit "Could not change dylib path."
python convert.py /sw/lib @loader_path $MOSYNCDIR/bin/mdb || error_exit "Could not change dylib path."
python convert.py /opt/local/lib @loader_path $MOSYNCDIR/bin/mdb || error_exit "Could not change dylib path."
python convert.py /sw/lib @loader_path $MOSYNCDIR/bin/openssl || error_exit "Could not change dylib path."
python convert.py /opt/local/lib @loader_path $MOSYNCDIR/bin/openssl || error_exit "Could not change dylib path."
python convert.py /sw/lib @loader_path $MOSYNCDIR/bin/ImageMagick/* || error_exit "Could not change dylib path."
python convert.py /opt/local/lib @loader_path $MOSYNCDIR/bin/ImageMagick/* || error_exit "Could not change dylib path."


#
# Copy Rules
#
echo "----------------------------------------------------"
echo "Copying rules"
echo "----------------------------------------------------"
cp -R $SOURCEDIR/rules $MOSYNCDIR/ || error_exit "Could copy rules"

#
# Copy examples and templates
#
echo "----------------------------------------------------"
echo "Copying examples and templates"
echo "----------------------------------------------------"

cp -R $SOURCEDIR/examples/examples.list $MOSYNCDIR/examples/examples.list || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/3dLines $MOSYNCDIR/examples/3dLines || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/AdvGraphics $MOSYNCDIR/examples/AdvGraphics || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/btServer $MOSYNCDIR/examples/Moblet/btServer || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/Connection $MOSYNCDIR/examples/Moblet/Connection || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/MoTooth $MOSYNCDIR/examples/Moblet/MoTooth || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/debugging $MOSYNCDIR/examples/debugging || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/HelloMap $MOSYNCDIR/examples/HelloMap || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/HelloMoblet $MOSYNCDIR/examples/Moblet/HelloMoblet || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/location $MOSYNCDIR/examples/location || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MapDemo $MOSYNCDIR/examples/MapDemo || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MAStx $MOSYNCDIR/examples/MAStx || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MAUI/MAUIex $MOSYNCDIR/examples/MAUI/MAUIex || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MinUI $MOSYNCDIR/examples/MinUI || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MoSketch $MOSYNCDIR/examples/MoSketch || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MoSound $MOSYNCDIR/examples/MoSound || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MoTris $MOSYNCDIR/examples/MoTris || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/MoTrix $MOSYNCDIR/examples/MoTrix || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/OtaLoad $MOSYNCDIR/examples/OtaLoad || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/QuakeMDL $MOSYNCDIR/examples/QuakeMDL || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/simple $MOSYNCDIR/examples/Moblet/simple || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/Soap $MOSYNCDIR/examples/Moblet/Soap || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/Stylus $MOSYNCDIR/examples/Moblet/Stylus || error_exit "Could not copy example."
cp -R $SOURCEDIR/examples/Moblet/timers $MOSYNCDIR/examples/Moblet/timers || error_exit "Could not copy example."

cp -R $SOURCEDIR/templates $MOSYNCDIR/templates || error_exit "Could not copy templates."

#
# Build runtimes
#

########################################
echo "----------------------------------------------------"
echo "Building runtimes"
echo "----------------------------------------------------"
cd $script_dir
MOSYNC_TRUNK=$SOURCEDIR
mkdir -p $MOSYNCDIR/profiles/runtimes/iphoneos/1/template
cd $SOURCEDIR/runtimes/cpp/platforms/iphone
cp Classes/impl/config_platform.h.example Classes/impl/config_platform.h 
/opt/local/bin/ruby buildLibraries.rb || error_exit "Could not build runtimes."





# Wait until Eclipse is built
# Then copy the result
echo "----------------------------------------------------"
echo "Waiting for IDE to be built"
echo "----------------------------------------------------"
#wait %1
cd $ECLIPSEDIR/com.mobilesorcery.sdk.product/build
if cp buildresult/I.MoSync/MoSync-macosx.cocoa.x86_64-trimmed.zip $MOSYNCDIR; then
	cd $MOSYNCDIR
	unzip MoSync-macosx.cocoa.x86_64-trimmed.zip
	mv mosync eclipse
	rm MoSync-macosx.cocoa.x86_64-trimmed.zip
	chmod +x $SOURCEDIR/build/release/PanicDoc
	$SOURCEDIR/build/release/PanicDoc -props >$MOSYNCDIR/eclipse/paniccodes.properties || error_exit "Could not make panic codes."
else
	error_exit "Could not find buildresult/I.MoSync/MoSync-macosx.cocoa.x86.zip"
fi

#
# Build documentation
#
echo "----------------------------------------------------"
echo "Building documentation"
echo "----------------------------------------------------"

cp $SOURCEDIR/libs/Doxyfile $MOSYNCDIR/include/Doxyfile
cp -r $SOURCEDIR/tools/ReleasePackageBuild/build_package_tools/mosync_docs/* $MOSYNCDIR/docs/
cd $MOSYNCDIR/include
export MOSYNC_DOXYGEN_OUTPUT_DIR=$MOSYNCDIR/docs
if /sw/bin/doxygen; then
	rm Doxyfile
	cp -R $MOSYNCDIR/docs/html $MOSYNCDIR/eclipse/plugins/com.mobilesorcery.sdk.help_1.0.0/docs/
else
	error_exit "Could not generate documentation."
fi
cd $SOURCEDIR/tools/doxy2cdt
ruby main.rb -b com.mobilesorcery.sdk.help/docs/html/ $MOSYNCDIR/docs/xml/index.xml $MOSYNCDIR/eclipse/plugins/com.mobilesorcery.sdk.help_1.0.0/docs/apireference.xml
rm -rf $MOSYNCDIR/docs/xml

#
# Build the OS X package
#
echo "----------------------------------------------------"
echo "Building the OS X package"
echo "----------------------------------------------------"
#/Developer/usr/bin/packagemaker -d $script_dir/mosync.pmdoc -v -i com.mosync -o $script_dir/MoSync-install.r$MOSYNC_HASH.mpkg || error_exit "Could not make OS X package."
cd $script_dir/mac_package_resources/MoSync-Installer
/usr/local/bin/freeze -v MoSync-$BUILDTYPE.packproj || error_exit "Could not make OS X package."
mv build/MoSync-$BUILDTYPE.pkg build/MoSync-$DATE.pkg || error_exit "Could rename OS X package."

#
# Creates a disk image
#
echo "----------------------------------------------------"
echo "Creating a disk image"
echo "----------------------------------------------------"
hdiutil create $script_dir/mac_package_resources/MoSync-Installer/build/MoSync-$DATE.dmg -srcfolder  $script_dir/mac_package_resources/MoSync-Installer/build/MoSync-$DATE.pkg -ov || error_exit "Could not make disk image."

# 
#  Moving to the results folder

echo "----------------------------------------------------"
echo "moving the package to results folder"
echo "----------------------------------------------------"
	mv $script_dir/mac_package_resources/MoSync-Installer/build/MoSync-$DATE.dmg $RESULTDIR || error_exit "Could not move the image."



