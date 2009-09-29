#!/bin/sh

#echo Executing $1 ...
echo $MOSYNCDIR/bin/more.exe -noscreen -program $1.moo

if $MOSYNCDIR/bin/more.exe -noscreen -program $1.moo;
then
	RES=0
else
	RES=1
fi

#echo Result: $RES

mv -f log.txt $1.log
if [ $RES = 0 ]; then
	touch $1.1
	rm -f $1.0
else
	touch $1.0
	rm -f $1.1
fi

#echo Exiting...

exit $RES
