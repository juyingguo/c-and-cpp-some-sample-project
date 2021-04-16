# Microsoft Developer Studio Project File - Name="FileTransferServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FileTransferServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FileTransferServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FileTransferServer.mak" CFG="FileTransferServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FileTransferServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FileTransferServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FileTransferServer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FileTransferServer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FileTransferServer - Win32 Release"
# Name "FileTransferServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Protocol\Model\ConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Control.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\FileModel.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\FileReadModel.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTransferServer.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTransferServer.rc
# End Source File
# Begin Source File

SOURCE=.\FileTransferServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Format.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Model.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerControl.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\ServerModel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Tray.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\ValidJudge.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Protocol\Kernel\AppConfig.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\ConfigFile.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Control.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\FileModel.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\FileReadModel.h
# End Source File
# Begin Source File

SOURCE=.\FileTransferServer.h
# End Source File
# Begin Source File

SOURCE=.\FileTransferServerDlg.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Format.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Includes.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Model.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ServerControl.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\ServerModel.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Tray.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\ValidJudge.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\FileTransferServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\FileTransferServer.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\XPStyle.manifest
# End Source File
# End Target
# End Project
