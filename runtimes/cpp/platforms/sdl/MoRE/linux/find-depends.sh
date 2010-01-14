#!/bin/sh

# Get DEB dependencies if possible
if [ -e "$(which dpkg-shlibdeps)" ]; then
	cur=$(pwd)
	bin=$(pwd)/$1

	if [ ! -e "/tmp/debian" ]; then
		mkdir /tmp/debian
	fi
	if [ ! -e "/tmp/debian/control" ]; then
		mkdir /tmp/debian/control
	fi

	cd /tmp/
	list=$(dpkg-shlibdeps -O $bin 2>/dev/null)
	if [ ! "$?" = "0" ]; then
		cd $cur	
		exit 1
	fi

	list=$(echo $list | sed 's/shlibs:Depends=//' | sed 's/, /\", \"/g')
	if [ ! "$list" = "" ]; then	
		echo "\"$list\""
	fi
	cd $cur
fi
exit 0


