#!/bin/bash

##################################
##                              ##
## To generate PanicDoc         ##
## then generate panics.html    ##
##                              ## 
##################################


## Assuming we are running in timedgen.sh's $DOCSDIR/docs
## replicated below
## DOCSDIR=/home/benchua/dev/documentation/mosync/


## Get into the PanicDoc directory
echo "Let's panic..."
pushd ../tools/PanicDoc

## Compile the PanicDoc
echo "Compiling PanicDoc..."
g++ PanicDoc.cpp -o pd -I../../intlibs -I.

## Run the PanicDoc
echo "Running PanicDoc..."
./pd > panics.html

## Copy it to docs directory
echo "Copying panics.html to docs/..."
cp ./panics.html ../../docs/

## Get out
echo "Done panicking..."
popd


## $DOCSDIR/docs/gendox.sh should be run after this

