#!/bin/sh

# Get RPM dependencies if possible
if [ -e "/usr/lib/rpm/find-requires" ]
then
	list=""

	req=$(echo $1 | /usr/lib/rpm/find-requires)
	if [ ! "$?" = "0" ]; then
		exit 1
	fi

	for l in $(echo $req | sed 's/ /, /g')
	do
		if [ "$list" = "" ] 
		then
			list="\"$l\""
		else
			list="$list, \"$l\""
		fi

	done
	echo $list	
fi
exit 0
