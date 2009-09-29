:RMDIR
rmdir /S /Q "%MOSYNCDIR%\include"
sleep 1
IF EXIST "%MOSYNCDIR%\include" goto RMDIR
:MKDIR
mkdir "%MOSYNCDIR%\include"
mkdir "%MOSYNCDIR%\include\GLES"
IF NOT EXIST "%MOSYNCDIR%\include" goto MKDIR

cd mastd
copy "GLES\*.h" "%MOSYNCDIR%\include\GLES\"
copy *.h "%MOSYNCDIR%\include\"
copy "..\libsupc++\new" "%MOSYNCDIR%\include\"
copy "..\libsupc++\*.h" "%MOSYNCDIR%\include\"
set DELHEAD=.
call private_headers.bat ..\delete_header.bat
cd ..

cd MAUtil
mkdir "%MOSYNCDIR%\include\MAUtil"
copy *.h "%MOSYNCDIR%\include\MAUtil\"
set DELHEAD=MAUtil
call private_headers.bat ..\delete_header.bat
cd ..

cd MTXml
mkdir "%MOSYNCDIR%\include\MTXml"
copy *.h "%MOSYNCDIR%\include\MTXml\"
cd ..

cd MAFS
mkdir "%MOSYNCDIR%\include\MAFS"
copy *.h "%MOSYNCDIR%\include\MAFS\"
cd ..

cd MATest
mkdir "%MOSYNCDIR%\include\MATest"
copy *.h "%MOSYNCDIR%\include\MATest\"
cd ..

cd MAUI
mkdir "%MOSYNCDIR%\include\MAUI"
copy *.h "%MOSYNCDIR%\include\MAUI\"
set DELHEAD=MAUI
call private_headers.bat ..\delete_header.bat
cd ..
