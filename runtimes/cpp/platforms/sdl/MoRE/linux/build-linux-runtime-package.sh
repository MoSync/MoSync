#!/bin/sh


#
# Error handling
#
handle_error()
{
	echo -e "\n\n---------------------------------"
	if [ "$1" = "rake" ]; then
		echo -e "Error: Rake failed, see log"
	elif [ "$1" = "more" ]; then
		echo -e "Error: Failed to build MoRE, see log"
	elif [ "$1" = "tar" ]; then
		echo -e "Error: Failed to extract package template"
	elif [ "$1" = "copy_more" ]; then
		echo -e "Error: While copying more"
	elif [ "$1" = "meta" ]; then
		echo -e "Error: While copying more"
	else
		echo -e "Error: Unknown cause"
	fi
	echo -e "---------------------------------\n"

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
	echo -e "\n\n---------------------------------"
	echo -e "Building $1 template"
	echo -e "---------------------------------\n"

	if [ -e  "$path/distro" ]; then
		rm -Rf $path/distro
	fi
	mkdir $path/distro
	mkdir $path/distro/tmp
	mkdir $path/distro/tmp/bin

	export MOSYNCDIR=$path/distro/tmp

	echo -e "\n\n---------------------------------"
	echo -e "Performing rake clean"
	echo -e "---------------------------------\n"
	cd $path/../../../../../
	if [ "$1" = "rel" ]; then
		rake clean CONFIG=""
	else
		rake clean
	fi
	echo -e "\nOK"

	echo -e "\n\n---------------------------------"
	echo -e "Attempting to build MoRE"
	echo -e "---------------------------------\n"
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

	echo -e "\nOK"

	# Now extract the template
	echo -e "\n\n---------------------------------"
	echo -e "Extracting template"
	echo -e "---------------------------------\n"
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
	echo -e "\n\n---------------------------------"
	echo -e "Generating template meta data"
	echo -e "---------------------------------\n"
	../generate-meta.sh usr/local/%appname%/bin/run | tee .meta/.meta
	if [ "$?" -ne "0" ]; then
		handle_error "meta"
	fi
	echo -e "OK"

	echo -e "\n\n---------------------------------"
	echo -e "Packing template"
	echo -e "---------------------------------\n"
	tar cvvfz $curr/runtime.${dist}.r${rev}.${arch}.$1.tar.gz $(find .meta -type f | sort) bin usr

	echo -e "\n\n---------------------------------"
	echo -e "Clean up"
	echo -e "---------------------------------\n"

	rm -Rf $path/distro
	cd $curr
	echo -e "OK"

	return 0
}



#######################   Main   #########################

# Initial vars
arch=$(uname -m | sed 's/i.../i586/')
curr=$(pwd)
path=$(pwd)/$(echo -e $0 | sed 's/build-linux-template.sh//')
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
echo -e "\n\n---------------------------------"
echo -e "Done"
echo -e "---------------------------------\n"

exit 0



