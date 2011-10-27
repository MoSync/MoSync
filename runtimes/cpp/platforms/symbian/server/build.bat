call ..\group-ed3\setPath.bat

devices -setdefault @S60_3rd_FP1:com.nokia.s60
call bldmake bldfiles
call abld build gcce urel
call make_unsigned_sis.bat
