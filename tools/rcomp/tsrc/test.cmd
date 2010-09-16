@rem
@rem Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of the License "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem

@ECHO OFF
REM TEST.CMD ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
REM
REM Called to run tests on the resource compiler.
REM
REM P1 may be a test number or "ALL" to cause all the tests to be run.
REM
REM ---------------------------------------------------------------------------
REM
SETLOCAL
SET n_tests=2

REM
IF "%1" == "" GOTO HELP
IF /I "%1" == "help" GOTO help
REM
IF /I "%1" == "ALL" GOTO test_all
REM

FOR %%i IN (1,2) DO IF "%1" == "%%i" GOTO valid_test

ECHO ** problem: %1 is not a recognised test
ENDLOCAL
GOTO :EOF

:valid_test
CALL :test_%1

ENDLOCAL
GOTO :EOF

:test_all
ECHO Running through each test...
FOR /L %%i IN (1,1,%n_tests%) DO CALL :test_%%i
GOTO end


:test_1
ECHO Test 1 : Compare output with previous version

IF EXIST test1.lis ERASE/Q TEST1.LIS

ECHO.
ECHO TUTEXT (16-bit)
ECHO Expect five problem messages about deprecated characters...
ECHO.
rcomp -u -sTUTEXT.RSS -oTUTEXT16.RSC.NEW -hTUTEXT0.RSG.NEW
FC TUTEXT0.RSG.NEW TUTEXT0.RSG >> test1.LIS
FC/B TUTEXT16.RSC.NEW TUTEXT16.RSC >> test1.LIS


ECHO.
ECHO TWTEXT (16-bit only)...
ECHO.
rcomp -u -sTWTEXT.RSS -oTWTEXT16.RSC.NEW -hTWTEXT0.RSG.NEW
FC TWTEXT0.RSG.NEW TWTEXT0.RSG >> test1.LIS
FC/B TWTEXT16.RSC.NEW TWTEXT16.RSC >> test1.LIS





DIR *lis |findstr/i test1

ECHO Check the contents of test1.LIS for successful comparisons...
ECHO A successful run yields a file of approximately 336 bytes in size.
PAUSE


GOTO :EOF


:test_2
ECHO Test 2 : Demonstrate behaviour with source errors

FOR /F %%j IN (TEST2.DAT) DO cpp -I . -D%%j < ERROR1.RSS | rcomp -u -oERROR1.RSC -hERROR1.RSG

ECHO.
ECHO See the documentation in test.HTM for an explanation of the error messages
PAUSE

GOTO :EOF



:help
ECHO Usage: TEST parameter
ECHO where parameter may be "HELP", "ALL" or a test number.
ECHO The current tests are:
ECHO  1 - Test compiler produces identical output to previous versions
ECHO  2 - Test behaviour with various errors
GOTO end



:end
ENDLOCAL
REM ---------  end of TEST.CMD

