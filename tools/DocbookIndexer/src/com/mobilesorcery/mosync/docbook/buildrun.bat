cd ..\..\..\..
unzip -ou %1\jhighlight-1.0.jar com/*

cd com\mobilesorcery\mosync\docbook
del *.class
javac -Xlint:unchecked -extdirs %1 *.java

cd ..\..\..\..
del *.c
del *.cpp
del *.h
del *.s
del output\*.html
del output\*.xml
copy maheaders.txt maheaders.h
java com.mobilesorcery.mosync.docbook.DocParser %2
@REM cd ..
