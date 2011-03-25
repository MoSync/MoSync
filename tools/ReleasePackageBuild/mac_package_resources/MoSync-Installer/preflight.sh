#!/bin/sh

#
# Remove old MoSync
#
if [ -d /Applications/MoSync ]; then
	rm -rf /Applications/MoSync/*
fi
