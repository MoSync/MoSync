#!/bin/sh
gcc -o getbuildinfo getbuildinfo.c
./getbuildinfo $MOSYNCDIR/bin/version.dat
