# Microsoft Developer Studio Project File - Name="ClipBook Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ClipBook Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ClipBook Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ClipBook Server.mak" CFG="ClipBook Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ClipBook Server - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ClipBook Server - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ClipBook Server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "FORCE_UNICODE" /D "STRSAFE_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "FORCE_UNICODE" /D "STRSAFE_LIB" /D _WIN32_WINNT=0x0502 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /D "MIDL_PASS" /mktyplib203 /win32  /robust
# ADD MTL /nologo /D "NDEBUG" /D "MIDL_PASS" /mktyplib203 /win32  /robust
# ADD BASE RSC /l 0x409 /i "Release" /d "NDEBUG"
# ADD RSC /l 0x409 /i "Release" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib clusapi.lib resutils.lib strsafe.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib clusapi.lib resutils.lib strsafe.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "ClipBook Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "FORCE_UNICODE" /D "STRSAFE_LIB" /YX /FD /GZ     /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "FORCE_UNICODE" /D "STRSAFE_LIB" /D _WIN32_WINNT=0x0502 /YX /FD /GZ     /c
# ADD BASE MTL /nologo /D "_DEBUG" /D "MIDL_PASS" /mktyplib203 /win32  /robust
# ADD MTL /nologo /D "_DEBUG" /D "MIDL_PASS" /mktyplib203 /win32  /robust
# ADD BASE RSC /l 0x409 /i "Debug" /d "_DEBUG"
# ADD RSC /l 0x409 /i "Debug" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib clusapi.lib resutils.lib strsafe.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib clusapi.lib resutils.lib strsafe.lib /nologo /subsystem:windows /dll /debug /machine:I386

!ENDIF 

# Begin Target

# Name "ClipBook Server - Win32 Release"
# Name "ClipBook Server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\ClipBook Server.cpp"
# End Source File
# Begin Source File

SOURCE=".\ClipBook Server.def"
# End Source File
# Begin Source File

SOURCE=".\ClipBook Server.rc"
# End Source File
# Begin Source File

SOURCE=.\ClRes.cpp
# End Source File
# Begin Source File

SOURCE=.\CMgdClusCfgInit.cpp
# End Source File
# Begin Source File

SOURCE=.\CMgdResType.cpp
# End Source File
# Begin Source File

SOURCE=.\MgdResource.idl
# ADD MTL /tlb "MgdResource.tlb" /h "MgdResource.h"
# End Source File
# Begin Source File

SOURCE=.\StringUtils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClRes.h
# End Source File
# Begin Source File

SOURCE=.\CMgdClusCfgInit.h
# End Source File
# Begin Source File

SOURCE=.\CMgdResType.h
# End Source File
# Begin Source File

SOURCE=.\Guids.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StringUtils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CMgdResType.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\settings.txt
# End Source File
# End Target
# End Project
