#!/bin/sh

#
# Check for PlistBuddy
#
if [ -e "/usr/libexec/PlistBuddy" ]; then
	PLISTBUDDY='/usr/libexec/PlistBuddy'
fi

# Error
if [ "$PLISTBUDDY" = "" ]; then
	echo "Could not locate PlistBuddy!"
	exit 1
fi

#
# If ~/.MacOSX/environment.plist exists, add MOSYNCDIR to it
# otherwise create the file with default content and add MOSYNCDIR
#
if [ -f ~/.MacOSX/environment.plist ]; then
	/usr/libexec/PlistBuddy -c "Delete :MOSYNCDIR" ~/.MacOSX/environment.plist
	/usr/libexec/PlistBuddy -c "Add :MOSYNCDIR string /Applications/MoSync" ~/.MacOSX/environment.plist
else
	if [ ! -d ~/.MacOSX ]; then
		mkdir ~/.MacOSX
	fi
	touch ~/.MacOSX/environment.plist
	echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> ~/.MacOSX/environment.plist
	echo "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> ~/.MacOSX/environment.plist
	echo "<plist version=\"1.0\">" >> ~/.MacOSX/environment.plist
	echo "<dict>" >> ~/.MacOSX/environment.plist
	echo "<key>MOSYNCDIR</key>" >> ~/.MacOSX/environment.plist
	echo "<string>/Applications/MoSync</string>" >> ~/.MacOSX/environment.plist
	echo "</dict>" >> ~/.MacOSX/environment.plist
	echo "</plist>" >> ~/.MacOSX/environment.plist
fi

#
# Generate keys for symbian packaging
#
/Applications/MoSync/bin/openssl genrsa -rand -des -passout pass:default 1024 > /Applications/MoSync/etc/default.key
/Applications/MoSync/bin/openssl req -new -x509 -nodes -sha1 -days 3650 -key /Applications/MoSync/etc/default.key -batch -config /Applications/MoSync/bin/openssl.cnf > /Applications/MoSync/etc/default.cert

#
# Final corrections
#
ln -s /Applications/MoSync/eclipse/mosync.app /Applications/MoSync/MoSync
chmod -R 777 /Applications/MoSync/eclipse/configuration
chmod 755 /Applications/MoSync/eclipse/mosync.app/Contents/MacOS/mosync
ln -s /Applications/MoSync/eclipse/mosync.app/Contents/MacOS/mosync /Applications/MoSync/eclipse/mosyncc
chmod -R 775 /Applications/MoSync/examples/*