# Microsoft Developer Studio Project File - Name="FileTransferClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FileTransferClient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FileTransferClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FileTransferClient.mak" CFG="FileTransferClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FileTransferClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FileTransferClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FileTransferClient - Win32 Release"

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

!ELSEIF  "$(CFG)" == "FileTransferClient - Win32 Debug"

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

# Name "FileTransferClient - Win32 Release"
# Name "FileTransferClient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClientControl.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\ClientModel.cpp
# End Source File
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

SOURCE=.\FileTransferClient.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTransferClient.rc
# End Source File
# Begin Source File

SOURCE=.\FileTransferClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\FileWriteModel.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Format.cpp
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\Model.cpp
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

SOURCE=.\ClientControl.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\ClientModel.h
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

SOURCE=.\FileTransferClient.h
# End Source File
# Begin Source File

SOURCE=.\FileTransferClientDlg.h
# End Source File
# Begin Source File

SOURCE=..\Protocol\Model\FileWriteModel.h
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

SOURCE=.\res\FileTransferClient.ico
# End Source File
# Begin Source File

SOURCE=.\res\FileTransferClient.rc2
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
