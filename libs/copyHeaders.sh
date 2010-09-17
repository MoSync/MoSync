#!/bin/sh

#
# Copy all the headers
#

if [ -d $MOSYNCDIR/include ]; then
	rm -rf $MOSYNCDIR/include
	mkdir $MOSYNCDIR/include
	mkdir $MOSYNCDIR/include/GLES
else
	mkdir $MOSYNCDIR/include
	mkdir $MOSYNCDIR/include/GLES
fi

cd MAStd
cp GLES/*.h $MOSYNCDIR/include/GLES/
cp *.h $MOSYNCDIR/include
cp ../libsupc++/new $MOSYNCDIR/include/
cp ../libsupc++/*.h $MOSYNCDIR/include/
cd ..

cd MAUtil
mkdir $MOSYNCDIR/include/MAUtil
cp *.h $MOSYNCDIR/include/MAUtil/
cd ..

cd MTXml
mkdir $MOSYNCDIR/include/MTXml
cp *.h $MOSYNCDIR/include/MTXml/
cd ..

cd MAFS
mkdir $MOSYNCDIR/include/MAFS
cp *.h $MOSYNCDIR/include/MAFS/
cd ..

cd MATest
mkdir $MOSYNCDIR/include/MATest
cp *.h $MOSYNCDIR/include/MATest/
cd ..

cd MAUI
mkdir $MOSYNCDIR/include/MAUI
cp *.h $MOSYNCDIR/include/MAUI/
cd ..

cd kazlib
mkdir $MOSYNCDIR/include/kazlib
cp *.h $MOSYNCDIR/include/kazlib/
cd ..

cd MAP
mkdir $MOSYNCDIR/include/MAP
cp *.h $MOSYNCDIR/include/MAP/
cd ..
