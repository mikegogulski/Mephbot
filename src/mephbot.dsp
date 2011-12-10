# Microsoft Developer Studio Project File - Name="mephbot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mephbot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mephbot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mephbot.mak" CFG="mephbot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mephbot - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mephbot - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mephbot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PICKIT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W2 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /dll /machine:I386 /out:"Release/mephbot.d2h"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "mephbot - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PICKIT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FAcs /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib /nologo /dll /incremental:no /map /debug /debugtype:both /machine:I386 /out:"Debug/mephbot.d2h" /pdbtype:sept /libpath:"."
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "mephbot - Win32 Release"
# Name "mephbot - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\automation.cpp
# End Source File
# Begin Source File

SOURCE=.\dijkstra.cpp
# End Source File
# Begin Source File

SOURCE=.\external\LinkedList\LinkedList.cpp
# End Source File
# Begin Source File

SOURCE=.\logging.cpp
# End Source File
# Begin Source File

SOURCE=.\maphack.cpp
# End Source File
# Begin Source File

SOURCE=.\mephbot.cpp
# End Source File
# Begin Source File

SOURCE=.\packethelper.cpp
# End Source File
# Begin Source File

SOURCE=.\skills.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\waypoint.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "D2Hackit files"

# PROP Default_Filter "*.cpp, *.h"
# Begin Source File

SOURCE=.\external\D2Client.h
# End Source File
# Begin Source File

SOURCE=.\external\D2HackIt.h
# End Source File
# Begin Source File

SOURCE=.\miscstructs.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\automation.h
# End Source File
# Begin Source File

SOURCE=.\dijkstra.h
# End Source File
# Begin Source File

SOURCE=.\logging.h
# End Source File
# Begin Source File

SOURCE=.\maphack.h
# End Source File
# Begin Source File

SOURCE=.\mephbot.h
# End Source File
# Begin Source File

SOURCE=.\packethelper.h
# End Source File
# Begin Source File

SOURCE=.\skills.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\waypoint.h
# End Source File
# End Group
# Begin Group "PKTK files"

# PROP Default_Filter "*.cpp *.h"
# Begin Source File

SOURCE=.\pktk\D2Item.cpp
# End Source File
# Begin Source File

SOURCE=.\pktk\D2Items.h
# End Source File
# Begin Source File

SOURCE=.\pktk\D2MagicAffixes.cpp
# End Source File
# Begin Source File

SOURCE=.\pktk\D2MagicAffixes.h
# End Source File
# Begin Source File

SOURCE=.\pktk\D2Sets.cpp
# End Source File
# Begin Source File

SOURCE=.\pktk\D2Sets.h
# End Source File
# Begin Source File

SOURCE=.\pktk\D2Uniques.cpp
# End Source File
# Begin Source File

SOURCE=.\pktk\D2Uniques.h
# End Source File
# Begin Source File

SOURCE=.\pktk\DiabloItems.cpp
# End Source File
# Begin Source File

SOURCE=.\pktk\DiabloItems.h
# End Source File
# Begin Source File

SOURCE=.\pktk\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\pktk\Mouse.h
# End Source File
# Begin Source File

SOURCE=.\pktk\MString.cpp
# End Source File
# Begin Source File

SOURCE=.\pktk\MString.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\mephbot-dev.txt"
# End Source File
# Begin Source File

SOURCE=.\mephbot.aut
# End Source File
# Begin Source File

SOURCE=.\mephbot.ini
# End Source File
# Begin Source File

SOURCE=.\mephbot.txt
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
