#!/bin/sh

#
# Outputs a message
#
print_msg()
{
	printf "\n\n%s\n" "---------------------------------"
	printf "%s\n"     " $1"
	printf "%s\n\n"   "---------------------------------"
}

#
# Error handling
#
handle_error()
{
	if [ "$1" = "rake" ]; then
		reason="Rake failed, see log"
	elif [ "$1" = "more" ]; then
		reason="Failed to build MoRE, see log"
	elif [ "$1" = "tar" ]; then
		reason="Failed to extract package template"
	elif [ "$1" = "copy_more" ]; then
		reason="While copying more"
	elif [ "$1" = "svn_export" ]; then
		reason="Failed to do svn export of template"
	elif [ "$1" = "meta" ]; then
		reason="While copying more"
	else
		reason="Unknown cause"
	fi

	print_msg "Error: $reason"

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
	if [ "$1" = "dbg" ]; then
		build_type="Debug"
	else
		build_type="Release"
	fi

	print_msg "Building $build_type template"
	if [ -e  "$path/distro" ]; then
		chmod 755 -R $path/distro
	fi
	mkdir $path/distro
	mkdir $path/distro/tmp
	mkdir $path/distro/tmp/bin

	export MOSYNCDIR=$path/distro/tmp

	if [ ! "$2" = "-noclean" ]; then
		print_msg "Performing rake clean"
		cd $path/../../../
		if [ "$1" = "rel" ]; then
			rake clean CONFIG=""
		else
			rake clean
		fi
		printf "\n%s\n" "OK"
	fi

	print_msg "Attempting to build MoRE"
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

	printf "\n%s\n" "OK"

	# Copy template
	print_msg "Copying template package"
	if [ -e template ]; then
		rm -Rf template
	fi
	svn --force export ../template ./
	if [ "$?" -ne "0" ]; then
		handle_error "svn_export"
	fi


	# Copy MoRE
	cp tmp/bin/moemu opt/%appname%/bin/run
	if [ "$?" -ne "0" ]; then
		handle_error "copy_more"
	fi
	printf "\n%s\n" "OK"

	# Generate its meta data
	print_msg "Generating template meta data"
	../generate-meta.sh opt/%appname%/bin/run | tee .meta/.meta
	if [ "$?" -ne "0" ]; then
		handle_error "meta"
	fi
	printf "\n%s\n" "OK"

	#
	print_msg "Packing custom template"
	tar cvvfz $curr/runtime.${dist}.r${rev}.${arch}.$1.tar.gz $(find .meta -type f | sort) usr opt

	#
	print_msg "Performing clean up"
	rm -Rf $path/distro
	cd $curr
	printf "\n%s\n" "OK"

	return 0
}



#######################   Main   #########################

# Initial vars
arch=$(uname -m | sed 's/i.../i386/')
curr=$(pwd)
path=$(pwd)/$(echo $0 | sed 's/\/build-linux-runtime-package.sh//')
dist=""; distro_name
cd $path/../../../
rev=$(svn info | grep Revision | awk '{print $2}')
cd $curr

#
# Build
#
buildTemplate "dbg" $1
buildTemplate "rel" $1

#
# Finished
# 
print_msg "Done"

exit 0



