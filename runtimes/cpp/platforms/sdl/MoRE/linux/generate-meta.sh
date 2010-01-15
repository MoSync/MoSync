#!/bin/sh
script_dir=$(echo $0 | sed 's/generate-meta.sh//')

# RPM dependencis
list=$($script_dir/find-requires.sh $1)
if [ "$?" = "0" ]; then
	if [ ! "$list" = "" ]; then
		requires=",\"requires\"  : [$list]"
	fi
fi

# DEB dependecies
list=$($script_dir/find-depends.sh $1)
if [ "$?" = "0" ]; then
	if [ ! "$list" = "" ]; then
		depends=",\"depends\"   : [$list]"
	fi
fi

if [ "$requires" = "" ]; then
	if [ "$depends" = "" ]; then
		echo "Error: Failed to generate dependencies" 1>&2
		exit 1
	fi
fi	

echo "{
	\"postinst\"  : \".meta/postinst\",
	\"programDir\": \"usr/local/%appname%/bin\",
	\"parseList\" : [ \"bin/%appname%\", \"usr/share/applications/%appname%.desktop\" ],
	\"iconList\"  : [ {\"type\": \"png\", \"size\":16, \"path\":\"usr/share/icons/hicolor/16x16/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":32, \"path\":\"usr/share/icons/hicolor/32x32/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":48, \"path\":\"usr/share/icons/hicolor/48x48/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":64, \"path\":\"usr/share/icons/hicolor/64x64/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":128, \"path\":\"usr/share/icons/hicolor/128x128/apps/%appname%.png\"}, {\"type\": \"png\", \"size\":256, \"path\":\"usr/share/icons/hicolor/256x256/apps/%appname%.png\"}, {\"type\": \"svg\", \"path\":\"usr/share/icons/hicolor/scalable/apps/%appname%.svg\"} ]
	$depends
	$requires
}"




