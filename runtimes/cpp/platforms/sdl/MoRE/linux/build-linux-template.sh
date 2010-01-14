#!/bin/sh


#
# Error handling
#
handle_error()
{
	echo "\n\n---------------------------------"
	if [ "$1" = "rake" ]; then
		echo "Error: Rake failed, see log"
	elif [ "$1" = "more" ]; then
		echo "Error: Failed to build MoRE, see log"
	elif [ "$1" = "tar" ]; then
		echo "Error: Failed to extract package template"
	elif [ "$1" = "copy_more" ]; then
		echo "Error: While copying more"
	elif [ "$1" = "meta" ]; then
		echo "Error: While copying more"
	else
		echo "Error: Unknown cause"
	fi
	echo "---------------------------------\n"

	exit 1
}

#
# Find which distro
#
distro_name()
{
	if [ -e "/etc/moblin-release" ]; then
		dist="moblin"
	elif [ -e "/etc/lsb-release" ]; then
		dist="ubuntu"
	elif [ -e "/etc/SUSE-release" ]; then
		dist="suse"
	elif [ -e "/etc/redhat-release" ]; then
		dist="redhat"
	elif [ -e "/etc/redhat_version" ]; then
		dist="redhat"
	elif [ -e "/etc/fedora-release" ]; then
		dist="fedora"
	elif [ -e "/etc/slackware-release" ]; then
		dist="slackware"
	elif [ -e "/etc/slackware_version" ]; then
		dist="slackware"
	elif [ -e "/etc/debian-release" ]; then
		dist="debian"
	elif [ -e "/etc/debian_version" ]; then
		dist="debian"
	elif [ -e "/etc/mandrake-release" ]; then
		dist="mandrake"
	elif [ -e "/etc/gentoo-release" ]; then
		dist="gentoo"
	fi
}

#
# Builds a template
#
#
buildTemplate()
{
	echo "\n\n---------------------------------"
	echo "Building $1 template"
	echo "---------------------------------\n"

	if [ -e  "$path/distro" ]; then
		rm -Rf $path/distro
	fi
	mkdir $path/distro
	mkdir $path/distro/tmp
	mkdir $path/distro/tmp/bin

	export MOSYNCDIR=$path/distro/tmp

	echo "\n\n---------------------------------"
	echo "Performing rake clean"
	echo "---------------------------------\n"
	cd $path/../../../../../
	if [ "$1" = "rel" ]; then
		rake clean CONFIG=""
	else
		rake clean
	fi
	echo "\nOK"

	echo "\n\n---------------------------------"
	echo "Attempting to build MoRE"
	echo "---------------------------------\n"
	if [ "$1" = "rel" ]; then
		rake more CONFIG=""
	else
		rake more
	fi

	if [ "$?" -ne "0" ]; then
		handle_error "rake"
	fi

	#Check if it built ok
	cd $path/distro
	if [ ! -e  "tmp/bin/moemu" ]; then
		handle_error "more"
	fi

	echo "\nOK"

	# Now extract the template
	echo "\n\n---------------------------------"
	echo "Extracting template"
	echo "---------------------------------\n"
	cp ../mosync-linux-template.tar.gz .
	tar zxvvf mosync-linux-template.tar.gz
	if [ "$?" -ne "0" ]; then
		handle_error "tar"
	fi

	# Copy MoRE
	cp tmp/bin/moemu usr/local/%appname%/bin/run
	if [ "$?" -ne "0" ]; then
		handle_error "copy_more"
	fi

	# Generate its meta data
	echo "\n\n---------------------------------"
	echo "Generating template meta data"
	echo "---------------------------------\n"
	../generate-meta.sh usr/local/%appname%/bin/run > .meta/.meta
	if [ "$?" -ne "0" ]; then
		handle_error "meta"
	fi
	echo "OK"

	echo "\n\n---------------------------------"
	echo "Packing template"
	echo "---------------------------------\n"
	tar cvvfz $curr/more-r${rev}.${dist}.${arch}.$1.tar.gz $(find .meta -type f | sort) bin usr

	echo "\n\n---------------------------------"
	echo "Clean up"
	echo "---------------------------------\n"

	rm -Rf $path/distro
	cd $curr
	echo "OK"

	return 0
}



#######################   Main   #########################

# Initial vars
arch=$(uname -m | sed 's/i.../i586/')
curr=$(pwd)
path=$(pwd)/$(echo $0 | sed 's/build-linux-template.sh//')
dist=""; distro_name
cd $path/../../../../../
rev=$(svn info | grep Revision | awk '{print $2}')
cd $curr

#
# Build
#
buildTemplate "dbg"
buildTemplate "rel"

#
# Finished
# 
echo "\n\n---------------------------------"
echo "Done"
echo "---------------------------------\n"

exit 0



