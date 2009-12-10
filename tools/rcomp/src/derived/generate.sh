#!/bin/sh

lex  -o rcompl.cph --header-file=rcompl.hpp  ../rcomp.l
yacc ../rcomp.y -o rcomp.cpp -d

