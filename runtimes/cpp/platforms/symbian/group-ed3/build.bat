set EPOCROOT=\Symbian\9.2\S60_3rd_FP1\
set PATH=%EPOCROOT%epoc32\tools\;c:\Perl\bin\;%EPOCROOT%epoc32\gcc\bin\;c:\Program\CSL Arm Toolchain\bin\;%PATH%

devices -setdefault @S60_3rd_FP1:com.nokia.s60
call bldmake bldfiles
call abld build gcce urel
