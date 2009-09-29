@echo off

@cd MATest
@mkdir "%MOSYNCDIR%\include\MATest"
copy *.h "%MOSYNCDIR%\include\MATest\"
@mobuild -build MATest.mopro
@cd ..
