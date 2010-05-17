set PATH=\Symbian\s60v5\S60_5th_Edition_SDK_v1.0\bin;\Symbian\s60v5\S60_5th_Edition_SDK_v1.0\epoc32\tools\;\Perl\bin\;\Symbian\s60v5\S60_5th_Edition_SDK_v1.0\epoc32\gcc\bin\;\Program\CSL Arm Toolchain\bin\
set EPOCROOT=\Symbian\s60v5\S60_5th_Edition_SDK_v1.0\

call bldmake bldfiles
call abld clean gcce urel
call abld build gcce urel
REM call abld clean armv5 urel
REM call abld build armv5 urel
