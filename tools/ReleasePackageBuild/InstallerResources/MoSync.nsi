;NSIS Modern User Interface
;Header Bitmap Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

!include "MUI.nsh"

!define ALL_USERS
!include WriteEnvStr.nsh

!include "registerExtension.nsh"

;SetCompress off
SetCompress force
SetCompressor /SOLID lzma

;--------------------------------
;General
!define APP_NAME "MoSync"
!define INSTDIR_REG_ROOT "HKLM"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
  
  ;Name and file
  Name "${APP_NAME}"
  OutFile "MoSyncSetup.exe"

  ;Default installation folder
  InstallDir "c:\MoSync"

  ;Get installation folder from registry if available
  ;InstallDirRegKey ${INSTDIR_REG_ROOT} "Software\MoSync" "Install_Dir"  
  InstallDirRegKey ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "Install_Dir"

!define MULTIUSER_EXECUTIONLEVEL Highest
;!define MULTIUSER_NOUNINSTALL ;Uncomment if no uninstaller is created
!include MultiUser.nsh
;--------------------------------
;Include the Uninstall log header
;!include "AdvUninstLog.nsh"
;!insertmacro UNATTENDED_UNINSTALL
;!insertmacro INTERACTIVE_UNINSTALL
;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE
  !define MUI_ICON "icon.ico"
  !define MUI_UNICON "icon.ico"
   
  !define MUI_HEADERIMAGE_BITMAP "installer.bmp" ; optional
  !define MUI_ABORTWARNING

  !define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\eclipse\mosync.exe"
  !define MUI_FINISHPAGE_SHOWREADME_TEXT "Open the MoSync IDE."
  !define MUI_FINISHPAGE_NOAUTOCLOSE

  !define MUI_DIRECTORYPAGE_VERIFYONLEAVE

;--------------------------------
;Pages
  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_COMPONENTS

  !define MUI_PAGE_CUSTOMFUNCTION_LEAVE "DirVerify"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Function DirVerify
  Push " "
  Push $INSTDIR
  Call StrCSpn
  Pop $R0 ; $R0 == !
  StrCmp $R0 "" +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "Install destination cannot contain spaces. Please choose a destination that doesn't contain spaces."
    Abort
FunctionEnd

Function CheckNotifyJavaVersion
  readRegStr $0 HKLM "SOFTWARE\JavaSoft\Java Runtime Environment" CurrentVersion

  StrCmp $0 "" +2
     Goto CheckVersion

  MessageBox MB_OK|MB_ICONEXCLAMATION "You have no Java Runtime Environment. You need at least version 1.6 to run the MoSync IDE. Please download and install the latest version."
  Goto CheckNotifyJavaVersionEnd

  CheckVersion:
  Push "1.6"
  Push $0
  Call VersionCheck
  Pop $1
  StrCmp $1 "1" +2
     Goto CheckNotifyJavaVersionEnd
  MessageBox MB_OK|MB_ICONEXCLAMATION "You have version $0 of the Java Runtime Environment. You need at least version 1.6 to run the MoSync IDE. Please download and install the latest version."
  CheckNotifyJavaVersionEnd:
FunctionEnd


Section "MoSync (required)" MoSync
  SectionIn RO
  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...

  IfFileExists "$INSTDIR\bin\*.*" feError
  IfFileExists "$INSTDIR\docs\*.*" feError
  IfFileExists "$INSTDIR\eclipse\*.*" feError
  IfFileExists "$INSTDIR\etc\*.*" feError
  IfFileExists "$INSTDIR\examples\*.*s" feError
  IfFileExists "$INSTDIR\include\*.*" feError
  IfFileExists "$INSTDIR\lib\*.*" feError
  IfFileExists "$INSTDIR\null\*.*" feError
  IfFileExists "$INSTDIR\profiles\*.*" feError
  IfFileExists "$INSTDIR\skins\*.*" feError
  IfFileExists "$INSTDIR\templates\*.*" feError

  goto getOnWithIt

feError:
  MessageBox MB_OKCANCEL|MB_ICONINFORMATION "The old MoSync product files in the installation directory will now be removed. Your projects and workspaces will be kep, but please make sure you have a backup, just in case. Click OK to continue the installation process, or CANCEL to abort." IDOK removeoldfiles IDCANCEL abortinstallation
removeoldfiles:
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\eclipse"
  RMDir /r "$INSTDIR\etc"
  RMDir /r "$INSTDIR\examples"
  RMDir /r "$INSTDIR\include"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\null"
  RMDir /r "$INSTDIR\profiles"
  RMDir /r "$INSTDIR\skins"
  RMDir /r "$INSTDIR\templates"
  Delete $INSTDIR\MoSyncRules.rules
  Delete $INSTDIR\uninstall.exe
  Delete "$SMPROGRAMS\MoSync\*.*"
  
  goto getOnWithIt
  
abortinstallation:

  Quit
  
getOnWithIt:
  ;!insertmacro UNINSTALL.LOG_OPEN_INSTALL 

  File /nonfatal /r "bin"
  File /nonfatal /r "docs"
  File /nonfatal /r "eclipse"
  File /nonfatal /r "examples"
  File /nonfatal /r "include"
  File /nonfatal /r "profiles"
  File /nonfatal /r "lib"
  File /nonfatal /r "skins"
  File /nonfatal /r "templates"
  File /nonfatal /r "etc"
  File /nonfatal "MoSyncRules.rules"    

  ;!insertmacro UNINSTALL.LOG_CLOSE_INSTALL
  CreateDirectory "$INSTDIR\projects"

  ;Store installation folder
  WriteRegStr HKLM SOFTWARE\MoSync "Install_Dir" "$INSTDIR"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MoSync" "DisplayName" "MoSync"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MoSync" "Publisher" "Mobile Sorcery"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MoSync" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MoSync" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MoSync" "NoRepair" 1
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"
 
; add environment variables
  Push "$INSTDIR\bin"
  Call AddToPath
 
  Push "MOSYNCDIR"
  Push $INSTDIR
  Call WriteEnvStr

; register icon
  ;${registerExtension} "$INSTDIR\bin\MoBuildGUI.exe" ".mopro" "MoSync Project File" "$INSTDIR\bin\gfx\mopro_icon.ico,0"

SectionEnd

Section "Prerequisites" Prerequisites
;  SetOutPath $INSTDIR\Prerequisites
    SectionIn RO
    File "vcredist_x86.exe"
    ;ExecWait "vcredist_x86.exe"
    ExecWait 'vcredist_x86.exe /q:a /c:"VCREDI~3.EXE /q:a /c:""msiexec /i vcredist.msi /qb!"" "'
Goto endVcRedist
  endVcRedist:
    Delete $INSTDIR\vcredist_x86.exe  

;   CreateDirectory "$INSTDIR\etc"
    ExecWait 'cmd /C "$INSTDIR\bin\openssl.exe genrsa -rand -des -passout pass:default 1024 > $INSTDIR\etc\default.key"'
    ExecWait 'cmd /C "$INSTDIR\bin\openssl.exe req -new -x509 -nodes -sha1 -days 3650 -key $INSTDIR\etc\default.key -batch -config $INSTDIR\bin\openssl.cnf > $INSTDIR\etc\default.cert"'

SectionEnd

;Section "Visual Studio Paths" VisualStudioPaths
;    ExecWait "$INSTDIR\bin\set_vc_dirs.bat"
;SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts" StartMenu

  CreateDirectory "$SMPROGRAMS\MoSync"
    CreateShortCut "$SMPROGRAMS\MoSync\MoSync IDE.lnk" "$INSTDIR\eclipse\mosync.exe" "" "$INSTDIR\eclipse\mosync.exe" 0
    CreateShortCut "$SMPROGRAMS\MoSync\BMFont Generator.lnk" "$INSTDIR\bin\BMFont\bmfont.exe" "" "$INSTDIR\bin\BMFont\bmfont.exe" 1
	CreateShortCut "$SMPROGRAMS\MoSync\Online User Guides.lnk" "$INSTDIR\bin\MoSyncOnlineDocs.URL" "" "$INSTDIR\bin\MoSyncOnlineDocs.URL" 2
    CreateShortCut "$SMPROGRAMS\MoSync\Uninstall MoSync.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 3
SectionEnd


; Optional section (can be disabled by the user)
Section "Desktop Shortcuts" Desktop

  CreateShortCut "$DESKTOP\MoSync.lnk" "$INSTDIR\eclipse\mosync.exe" "" "$INSTDIR\eclipse\mosync.exe" 0

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_MoSyncDesc ${LANG_ENGLISH} "Install the MoSync sdk with tools and libraries."
  LangString DESC_StartMenuDesc ${LANG_ENGLISH} "Add start menu shortcuts."
  LangString DESC_DesktopDesc ${LANG_ENGLISH} "Add desktop shortcuts."
  LangString DESC_PrerequisitesDesc ${LANG_ENGLISH} "Install MoSync prerequisites."
;  LangString DESC_VisualStudioPaths ${LANG_ENGLISH} "Add MoSync Visual Studio paths."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${MoSync} $(DESC_MoSyncDesc)
    !insertmacro MUI_DESCRIPTION_TEXT ${StartMenu} $(DESC_StartMenuDesc)   
    !insertmacro MUI_DESCRIPTION_TEXT ${Desktop} $(DESC_DesktopDesc)      
    !insertmacro MUI_DESCRIPTION_TEXT ${Prerequisites} $(DESC_PrerequisitesDesc)    
;    !insertmacro MUI_DESCRIPTION_TEXT ${VisualStudioPaths} $(DESC_VisualStudioPaths)       

  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  MessageBox MB_OK|MB_ICONEXCLAMATION "This uninstaller will remove all MoSync shortcuts, registry entries and product files. Workspaces and projects will be left intact. Just delete them manually if you no longer need them."

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MoSync"
  DeleteRegKey HKLM SOFTWARE\MoSync

  ; Unregister icon
  ${unregisterExtension} ".mopro" "MoSync Project File"

  ; Remove files and uninstaller
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\eclipse"
  RMDir /r "$INSTDIR\etc"
  RMDir /r "$INSTDIR\examples"
  RMDir /r "$INSTDIR\include"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\null"
  RMDir /r "$INSTDIR\profiles"
  RMDir /r "$INSTDIR\skins"
  RMDir /r "$INSTDIR\templates"
  Delete $INSTDIR\MoSyncRules.rules
  Delete $INSTDIR\uninstall.exe

  ; Remove Desktop shortcut
  Delete "$DESKTOP\MoSync.lnk"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\MoSync\*.*"
  
  ; Remove start menu directories used
  RMDir /r "$SMPROGRAMS\MoSync"
   
  ;uninstall from path, must be repeated for every install logged path individual
  ;!insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR"

  ;uninstall from path, must be repeated for every install logged path individual
  ;!insertmacro UNINSTALL.LOG_UNINSTALL "$APPDATA\${APP_NAME}"

  ;end uninstall, after uninstall from all logged paths has been performed
  ;!insertmacro UNINSTALL.LOG_END_UNINSTALL
  
  ; remove environment variables  
  Push "$INSTDIR\bin"
  Call un.RemoveFromPath

  Push "LIB"
  Push "$INSTDIR\lib\win32"
  Call un.RemoveFromEnvVar
 
  ;Likewise RemoveFromPath could be
  Push "MOSYNCDIR"
  Call un.DeleteEnvStr

  ;System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'

SectionEnd


XPStyle on

Function .onInit

	!insertmacro MULTIUSER_INIT

        # the plugins dir is automatically deleted when the installer exits
        InitPluginsDir
        File /oname=$PLUGINSDIR\splash.bmp "installer_splash.bmp"

        advsplash::show 1000 1000 1000 0xff00ff $PLUGINSDIR\splash
    
        ;create/update log always within .onInstSuccess function    
        ;!insertmacro UNINSTALL.LOG_PREPARE_INSTALL

	Call CheckNotifyJavaVersion
FunctionEnd

Function .onInstSuccess
	;create/update log always within .onInstSuccess function
	;!insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

Function UN.onInit
         ;begin uninstall, could be added on top of uninstall section instead
	!insertmacro MULTIUSER_UNINIT
	;!insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
FunctionEnd


;-------------------------------------------
; Functions for adding environment variables
;-------------------------------------------
!ifndef _AddToPath_nsh
!define _AddToPath_nsh
 
!verbose 3
!include "WinMessages.NSH"
!verbose 4
 
!ifndef WriteEnvStr_RegKey
  !ifdef ALL_USERS
    !define WriteEnvStr_RegKey \
       'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
  !else
    !define WriteEnvStr_RegKey 'HKCU "Environment"'
  !endif
!endif
 
; AddToPath - Adds the given dir to the search path.
;        Input - head of the stack
;        Note - Win9x systems requires reboot
 
Function AddToPath
  Exch $0
  Push $1
  Push $2
  Push $3
 
  # don't add if the path doesn't exist
  IfFileExists "$0\*.*" "" AddToPath_done
 
  ReadEnvStr $1 PATH
  Push "$1;"
  Push "$0;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  Push "$1;"
  Push "$0\;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  GetFullPathName /SHORT $3 $0
  Push "$1;"
  Push "$3;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  Push "$1;"
  Push "$3\;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
 
  Call IsNT
  Pop $1
  StrCmp $1 1 AddToPath_NT
    ; Not on NT
    StrCpy $1 $WINDIR 2
    FileOpen $1 "$1\autoexec.bat" a
    FileSeek $1 -1 END
    FileReadByte $1 $2
    IntCmp $2 26 0 +2 +2 # DOS EOF
      FileSeek $1 -1 END # write over EOF
    FileWrite $1 "$\r$\nSET PATH=%PATH%;$3$\r$\n"
    FileClose $1
    SetRebootFlag true
    Goto AddToPath_done
 
  AddToPath_NT:
    ReadRegStr $1 ${WriteEnvStr_RegKey} "PATH"
    StrCmp $1 "" AddToPath_NTdoIt
      Push $1
      Call Trim
      Pop $1
      StrCpy $0 "$1;$0"
    AddToPath_NTdoIt:
      WriteRegExpandStr ${WriteEnvStr_RegKey} "PATH" $0
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
 
  AddToPath_done:
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd
 
; RemoveFromPath - Remove a given dir from the path
;     Input: head of the stack
 
Function un.RemoveFromPath
  Exch $0
  Push $1
  Push $2
  Push $3
  Push $4
  Push $5
  Push $6
 
  IntFmt $6 "%c" 26 # DOS EOF
 
  Call un.IsNT
  Pop $1
  StrCmp $1 1 unRemoveFromPath_NT
    ; Not on NT
    StrCpy $1 $WINDIR 2
    FileOpen $1 "$1\autoexec.bat" r
    GetTempFileName $4
    FileOpen $2 $4 w
    GetFullPathName /SHORT $0 $0
    StrCpy $0 "SET PATH=%PATH%;$0"
    Goto unRemoveFromPath_dosLoop
 
    unRemoveFromPath_dosLoop:
      FileRead $1 $3
      StrCpy $5 $3 1 -1 # read last char
      StrCmp $5 $6 0 +2 # if DOS EOF
        StrCpy $3 $3 -1 # remove DOS EOF so we can compare
      StrCmp $3 "$0$\r$\n" unRemoveFromPath_dosLoopRemoveLine
      StrCmp $3 "$0$\n" unRemoveFromPath_dosLoopRemoveLine
      StrCmp $3 "$0" unRemoveFromPath_dosLoopRemoveLine
      StrCmp $3 "" unRemoveFromPath_dosLoopEnd
      FileWrite $2 $3
      Goto unRemoveFromPath_dosLoop
      unRemoveFromPath_dosLoopRemoveLine:
        SetRebootFlag true
        Goto unRemoveFromPath_dosLoop
 
    unRemoveFromPath_dosLoopEnd:
      FileClose $2
      FileClose $1
      StrCpy $1 $WINDIR 2
      Delete "$1\autoexec.bat"
      CopyFiles /SILENT $4 "$1\autoexec.bat"
      Delete $4
      Goto unRemoveFromPath_done
 
  unRemoveFromPath_NT:
    ReadRegStr $1 ${WriteEnvStr_RegKey} "PATH"
    StrCpy $5 $1 1 -1 # copy last char
    StrCmp $5 ";" +2 # if last char != ;
      StrCpy $1 "$1;" # append ;
    Push $1
    Push "$0;"
    Call un.StrStr ; Find `$0;` in $1
    Pop $2 ; pos of our dir
    StrCmp $2 "" unRemoveFromPath_done
      ; else, it is in path
      # $0 - path to add
      # $1 - path var
      StrLen $3 "$0;"
      StrLen $4 $2
      StrCpy $5 $1 -$4 # $5 is now the part before the path to remove
      StrCpy $6 $2 "" $3 # $6 is now the part after the path to remove
      StrCpy $3 $5$6
 
      StrCpy $5 $3 1 -1 # copy last char
      StrCmp $5 ";" 0 +2 # if last char == ;
        StrCpy $3 $3 -1 # remove last char
 
      WriteRegExpandStr ${WriteEnvStr_RegKey} "PATH" $3
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
 
  unRemoveFromPath_done:
    Pop $6
    Pop $5
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd
 
 
 
; AddToEnvVar - Adds the given value to the given environment var
;        Input - head of the stack $0 environement variable $1=value to add
;        Note - Win9x systems requires reboot
 
Function AddToEnvVar
 
  Exch $1 ; $1 has environment variable value
  Exch
  Exch $0 ; $0 has environment variable name
 
  DetailPrint "Adding $1 to $0"
  Push $2
  Push $3
  Push $4
 
 
  ReadEnvStr $2 $0
  Push "$2;"
  Push "$1;"
  Call StrStr
  Pop $3
  StrCmp $3 "" "" AddToEnvVar_done
 
  Push "$2;"
  Push "$1\;"
  Call StrStr
  Pop $3
  StrCmp $3 "" "" AddToEnvVar_done
  
 
  Call IsNT
  Pop $2
  StrCmp $2 1 AddToEnvVar_NT
    ; Not on NT
    StrCpy $2 $WINDIR 2
    FileOpen $2 "$2\autoexec.bat" a
    FileSeek $2 -1 END
    FileReadByte $2 $3
    IntCmp $3 26 0 +2 +2 # DOS EOF
      FileSeek $2 -1 END # write over EOF
    FileWrite $2 "$\r$\nSET $0=%$0%;$4$\r$\n"
    FileClose $2
    SetRebootFlag true
    Goto AddToEnvVar_done
 
  AddToEnvVar_NT:
    ReadRegStr $2 ${WriteEnvStr_RegKey} $0
    StrCpy $3 $2 1 -1 # copy last char
    StrCmp $3 ";" 0 +2 # if last char == ;
      StrCpy $2 $2 -1 # remove last char
    StrCmp $2 "" AddToEnvVar_NTdoIt
      StrCpy $1 "$2;$1"
    AddToEnvVar_NTdoIt:
      WriteRegExpandStr ${WriteEnvStr_RegKey} $0 $1
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
 
  AddToEnvVar_done:
    Pop $4
    Pop $3
    Pop $2
    Pop $0
    Pop $1
 
FunctionEnd
 
; RemoveFromEnvVar - Remove a given value from a environment var
;     Input: head of the stack
 
Function un.RemoveFromEnvVar
 
  Exch $1 ; $1 has environment variable value
  Exch
  Exch $0 ; $0 has environment variable name
 
  DetailPrint "Removing $1 from $0"
  Push $2
  Push $3
  Push $4
  Push $5
  Push $6
  Push $7
 
  IntFmt $7 "%c" 26 # DOS EOF
 
  Call un.IsNT
  Pop $2
  StrCmp $2 1 unRemoveFromEnvVar_NT
    ; Not on NT
    StrCpy $2 $WINDIR 2
    FileOpen $2 "$2\autoexec.bat" r
    GetTempFileName $5
    FileOpen $3 $5 w
    GetFullPathName /SHORT $1 $1
    StrCpy $1 "SET $0=%$0%;$1"
    Goto unRemoveFromEnvVar_dosLoop
 
    unRemoveFromEnvVar_dosLoop:
      FileRead $2 $4
      StrCpy $6 $4 1 -1 # read last char
      StrCmp $6 $7 0 +2 # if DOS EOF
        StrCpy $4 $4 -1 # remove DOS EOF so we can compare
      StrCmp $4 "$1$\r$\n" unRemoveFromEnvVar_dosLoopRemoveLine
      StrCmp $4 "$1$\n" unRemoveFromEnvVar_dosLoopRemoveLine
      StrCmp $4 "$1" unRemoveFromEnvVar_dosLoopRemoveLine
      StrCmp $4 "" unRemoveFromEnvVar_dosLoopEnd
      FileWrite $3 $4
      Goto unRemoveFromEnvVar_dosLoop
      unRemoveFromEnvVar_dosLoopRemoveLine:
        SetRebootFlag true
        Goto unRemoveFromEnvVar_dosLoop
 
    unRemoveFromEnvVar_dosLoopEnd:
      FileClose $3
      FileClose $2
      StrCpy $2 $WINDIR 2
      Delete "$2\autoexec.bat"
      CopyFiles /SILENT $5 "$2\autoexec.bat"
      Delete $5
      Goto unRemoveFromEnvVar_done
 
  unRemoveFromEnvVar_NT:
    ReadRegStr $2 ${WriteEnvStr_RegKey} $0
    StrCpy $6 $2 1 -1 # copy last char
    StrCmp $6 ";" +2 # if last char != ;
      StrCpy $2 "$2;" # append ;
    Push $2
    Push "$1;"
    Call un.StrStr ; Find `$1;` in $2
    Pop $3 ; pos of our dir
    StrCmp $3 "" unRemoveFromEnvVar_done
      ; else, it is in path
      # $1 - path to add
      # $2 - path var
      StrLen $4 "$1;"
      StrLen $5 $3
      StrCpy $6 $2 -$5 # $6 is now the part before the path to remove
      StrCpy $7 $3 "" $4 # $7 is now the part after the path to remove
      StrCpy $4 $6$7
 
      StrCpy $6 $4 1 -1 # copy last char
      StrCmp $6 ";" 0 +2 # if last char == ;
      StrCpy $4 $4 -1 # remove last char
 
      WriteRegExpandStr ${WriteEnvStr_RegKey} $0 $4
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
 
  unRemoveFromEnvVar_done:
    Pop $7
    Pop $6
    Pop $5
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd
 
 
 
 
!ifndef IsNT_KiCHiK
!define IsNT_KiCHiK
 
###########################################
#            Utility Functions            #
###########################################
 
; IsNT
; no input
; output, top of the stack = 1 if NT or 0 if not
;
; Usage:
;   Call IsNT
;   Pop $R0
;  ($R0 at this point is 1 or 0)
 
!macro IsNT un
Function ${un}IsNT
  Push $0
  ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $0 "" 0 IsNT_yes
  ; we are not NT.
  Pop $0
  Push 0
  Return
 
  IsNT_yes:
    ; NT!!!
    Pop $0
    Push 1
FunctionEnd
!macroend
!insertmacro IsNT ""
!insertmacro IsNT "un."
 
!endif ; IsNT_KiCHiK
 
; StrStr
; input, top of stack = string to search for
;        top of stack-1 = string to search in
; output, top of stack (replaces with the portion of the string remaining)
; modifies no other variables.
;
; Usage:
;   Push "this is a long ass string"
;   Push "ass"
;   Call StrStr
;   Pop $R0
;  ($R0 at this point is "ass string")
 
!macro StrStr un
Function ${un}StrStr
Exch $R1 ; st=haystack,old$R1, $R1=needle
  Exch    ; st=old$R1,haystack
  Exch $R2 ; st=old$R1,old$R2, $R2=haystack
  Push $R3
  Push $R4
  Push $R5
  StrLen $R3 $R1
  StrCpy $R4 0
  ; $R1=needle
  ; $R2=haystack
  ; $R3=len(needle)
  ; $R4=cnt
  ; $R5=tmp
  loop:
    StrCpy $R5 $R2 $R3 $R4
    StrCmp $R5 $R1 done
    StrCmp $R5 "" done
    IntOp $R4 $R4 + 1
    Goto loop
done:
  StrCpy $R1 $R2 "" $R4
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Exch $R1
FunctionEnd
!macroend
!insertmacro StrStr ""
!insertmacro StrStr "un."
 
!endif ; _AddToPath_nsh
 
Function Trim ; Added by Pelaca
	Exch $R1
	Push $R2
Loop:
	StrCpy $R2 "$R1" 1 -1
	StrCmp "$R2" " " RTrim
	StrCmp "$R2" "$\n" RTrim
	StrCmp "$R2" "$\r" RTrim
	StrCmp "$R2" ";" RTrim
	GoTo Done
RTrim:	
	StrCpy $R1 "$R1" -1
	Goto Loop
Done:
	Pop $R2
	Exch $R1
FunctionEnd

Function un.DeleteDirIfEmpty
  FindFirst $R0 $R1 "$0\*.*"
  strcmp $R1 "." 0 NoDelete
   FindNext $R0 $R1
   strcmp $R1 ".." 0 NoDelete
    ClearErrors
    FindNext $R0 $R1
    IfErrors 0 NoDelete
     FindClose $R0
     Sleep 1000
     RMDir /r "$0"
  NoDelete:
   FindClose $R0
FunctionEnd

Function StrCSpn
 Exch $R0 ; string to check
 Exch
 Exch $R1 ; string of chars
 Push $R2 ; current char
 Push $R3 ; current char
 Push $R4 ; char loop
 Push $R5 ; char loop
 
  StrCpy $R4 -1
 
  NextChar:
  StrCpy $R2 $R1 1 $R4
  IntOp $R4 $R4 - 1
   StrCmp $R2 "" StrOK
 
   StrCpy $R5 -1
 
   NextCharCheck:
   StrCpy $R3 $R0 1 $R5
   IntOp $R5 $R5 - 1
    StrCmp $R3 "" NextChar
    StrCmp $R3 $R2 0 NextCharCheck
     StrCpy $R0 $R2
     Goto Done
 
 StrOK:
 StrCpy $R0 ""
 
 Done:
 
 Pop $R5
 Pop $R4
 Pop $R3
 Pop $R2
 Pop $R1
 Exch $R0
FunctionEnd

Function VersionCheck
  Exch $0 ;second versionnumber
  Exch
  Exch $1 ;first versionnumber
  Push $R0 ;counter for $0
  Push $R1 ;counter for $1
  Push $3 ;temp char
  Push $4 ;temp string for $0
  Push $5 ;temp string for $1
  StrCpy $R0 "-1"
  StrCpy $R1 "-1"
  Start:
  StrCpy $4 ""
  DotLoop0:
  IntOp $R0 $R0 + 1
  StrCpy $3 $0 1 $R0
  StrCmp $3 "" DotFound0
  StrCmp $3 "." DotFound0
  StrCpy $4 $4$3
  Goto DotLoop0
  DotFound0:
  StrCpy $5 ""
  DotLoop1:
  IntOp $R1 $R1 + 1
  StrCpy $3 $1 1 $R1
  StrCmp $3 "" DotFound1
  StrCmp $3 "." DotFound1
  StrCpy $5 $5$3
  Goto DotLoop1
  DotFound1:
  Strcmp $4 "" 0 Not4
    StrCmp $5 "" Equal
    Goto Ver2Less
  Not4:
  StrCmp $5 "" Ver2More
  IntCmp $4 $5 Start Ver2Less Ver2More
  Equal:
  StrCpy $0 "0"
  Goto Finish
  Ver2Less:
  StrCpy $0 "1"
  Goto Finish
  Ver2More:
  StrCpy $0 "2"
  Finish:
  Pop $5
  Pop $4
  Pop $3
  Pop $R1
  Pop $R0
  Pop $1
  Exch $0
FunctionEnd
