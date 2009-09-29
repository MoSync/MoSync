#!/bin/bash

##################################
##                              ##
## To generate the IDL Compiler ##
## then generate maapi.h        ##
##                              ## 
##################################


## Assuming we are running in timedgen.sh's $DOCSDIR/docs
## replicated below
## DOCSDIR=/home/benchua/dev/documentation/mosync/


## Get into the idl directory
echo "Let's do IDL..."
pushd ../tools/idl

## Compile the IDL compiler
echo "Compiling IDL compiler..."
g++ main.cpp -o idlcomp

## Run the IDL compiler
echo "Running IDL compiler..."
./idlcomp

## Copy it to MAStd directory
echo "Copying maapi.h to MAStd/..."
cp ./maapi.h ../../libs/MAStd/
cp ./cpp_defs.h ../../intlibs/helpers/

## Get out
echo "Done with IDL..."
popd


## $DOCSDIR/docs/gendox.sh should be run after this




