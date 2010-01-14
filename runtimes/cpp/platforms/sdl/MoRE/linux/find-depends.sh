#!/bin/sh

# Get DEB dependencies if possible
if [ -e "$(which dpkg-shlibdeps)" ]
then
	echo "\"depends\" : [\"$(dpkg-shlibdeps -O moemu 2>/dev/null | sed 's/shlibs:Depends=//' | sed 's/, /\", \"/g')\"]"
fi

