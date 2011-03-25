pexports -v -h ..\include\BluetoothAPIs.h %SystemRoot%\system32\bthprops.cpl > bthprops.def
dlltool -k -l libbthprops_ex.a -d bthprops.def -D bthprops.cpl
