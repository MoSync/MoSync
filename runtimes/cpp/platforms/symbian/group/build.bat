devices -setdefault @S60_2nd_FP3:com.nokia.series60
set EPOCROOT=\Symbian\8.1a\S60_2nd_FP3\
call bldmake bldfiles
call abld build armi urel
pause
