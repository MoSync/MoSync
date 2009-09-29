REM make -f Icons_scalable_dc.mk resource

SET EPOCROOT=\Symbian\9.2\S60_3rd_FP1\
SET GROUPDIR=%CD%

REM cd %EPOCROOT%Epoc32\tools
REM mifconv %EPOCROOT%epoc32\data\z\resource\apps\MoSync_icon.mif /c32 %GROUPDIR%\..\gfx\qgn_menu_helloworldbasic.svg

cd ..\gfx\
%EPOCROOT%Epoc32\tools\mifconv MoSync_icon.mif /c32 qgn_menu_helloworldbasic.svg
