#!/bin/sh

# Get RPM dependencies if possible
if [ -e "/usr/lib/rpm/find-requires" ]
then
	list=""
	for l in $(echo $1 | /usr/lib/rpm/find-requires | sed 's/ /, /g')
	do
		if [ "$list" = "" ] 
		then
			list="\"$l\""
		else
			list="$list \"$l\""
		fi

	done
	echo "\"requires\" : [$list]"	
fi
