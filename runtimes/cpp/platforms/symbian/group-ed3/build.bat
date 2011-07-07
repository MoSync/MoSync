call setPath.bat

devices -setdefault @S60_3rd_FP1:com.nokia.s60
call bldmake bldfiles
call abld build gcce urel
