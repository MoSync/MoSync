
if exist %1 goto continue
goto c2
:continue
rmdir /S /Q %1
:c2
mkdir %1
