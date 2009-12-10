# Generate some source files
#
# Note that the YACC and LEX tools used expect to see Unix-style
# path names and will hang horribly if given DOS pathnames

YACC= t:\mks\bin\yacc.exe -t    -LC -P t:/mks/etc/yyparse.cpp
LEX=  t:\mks\bin\lex.exe  -a -T -LC -P t:/mks/etc/yylex.cpp

GENERATED_FILES= \
	rcomp.cpp \
	rcomp.hpp \
	rcompl.cph \
	rcompl.hpp 

rcompl.cph rcompl.hpp : ..\rcomp.l
	set ROOTDIR=t:\mks
	$(LEX) -o rcompl.cph -D rcompl.hpp ../rcomp.l

rcomp.cpp rcomp.hpp : ..\rcomp.y
	set ROOTDIR=t:\mks
	$(YACC) -o rcomp.cpp -D rcomp.hpp ../rcomp.y

do_nothing:
	@rem do nothing

#
# The targets invoked by bld...
#

# Do the work in the MAKMAKE target, in the hope of getting the files
# created in time to scan them in the processing of RCOMP.MMP

MAKMAKE : $(GENERATED_FILES)

BLD : MAKMAKE

SAVESPACE : MAKMAKE

CLEAN : 
	erase $(GENERATED_FILES)

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

FINAL : do_nothing

RELEASABLES : do_nothing

