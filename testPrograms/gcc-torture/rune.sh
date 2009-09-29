#!/bin/sh

#echo Executing $1 ...
echo $MOSYNCDIR/bin/more.exe -noscreen -program $1.moe

if $MOSYNCDIR/bin/more.exe -noscreen -program $1.moe;
then
	RES=0
else
	RES=1
fi

#echo Result: $RES

mv -f log.txt $1.loge
if [ $RES = 0 ]; then
	touch $1.1e
	rm -f $1.0e
else
	touch $1.0e
	rm -f $1.1e
	mv -f _masterdump.s $1.ms
fi

#echo Exiting...

exit $RES
