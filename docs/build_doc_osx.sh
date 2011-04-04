#!/bin/sh

DOCSDIR=$(pwd)

# Create directories
if [ -d $MOSYNCDIR/docs ]; then
	rm -rf $MOSYNCDIR/docs
	mkdir $MOSYNCDIR/docs
else
	mkdir $MOSYNCDIR/docs
fi

# Set doxygen's output
export MOSYNC_DOXYGEN_OUTPUT_DIR=$MOSYNCDIR/docs
# Copy change log and release notes
cp release_notes.txt $MOSYNC_DOXYGEN_OUTPUT_DIR
cp changelog.txt $MOSYNC_DOXYGEN_OUTPUT_DIR

# Copy doxyfile
cp ../libs/Doxyfile $MOSYNCDIR/include/Doxyfile

# Build documentation
cd $MOSYNCDIR/include
doxygen

