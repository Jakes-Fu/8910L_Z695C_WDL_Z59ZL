# Microsoft Developer Studio Project File - Name="http" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=http - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "http.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "http.mak" CFG="http - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "http - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "http - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "http"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "http - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "http - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W3 /Gm /GX /ZI /I "..\inc" /I "..\..\Win\include\os\\" /I "..\..\Win\include\ps\\" /I "..\..\tcpip\inc" /I "..\..\tcpip\src\h" /I "..\..\ssl\inc" /I "..\..\common\inc\string" /I "..\..\..\..\RTOS\export\inc" /I "..\..\..\..\common\export\inc" /I "..\..\..\..\MS_Ref\export\inc" /I "..\..\common\inc" /I "..\..\common\inc\encrypt" /I "..\..\..\export\inc" /I "..\..\qhsm\inc" /I "..\..\netmanager\inc" /I "..\..\common\inc\\" /I "..\..\common\inc\mem_debug" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "THREADX_OS" /D "_RTOS" /D "SSL_SUPPORT" /D "HTTP_MEM_DEBUG" /FR"" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/http.lib"

!ENDIF 

# Begin Target

# Name "http - Win32 Release"
# Name "http - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\http_api.c
# End Source File
# Begin Source File

SOURCE=..\src\http_auth.c
# End Source File
# Begin Source File

SOURCE=..\src\http_cache.c
# End Source File
# Begin Source File

SOURCE=..\src\http_cache_api.c
# End Source File
# Begin Source File

SOURCE=..\src\http_chunk.c
# End Source File
# Begin Source File

SOURCE=..\src\http_cookie_api.c
# End Source File
# Begin Source File

SOURCE=..\src\http_date_parser.c
# End Source File
# Begin Source File

SOURCE=..\src\http_event.c
# End Source File
# Begin Source File

SOURCE=..\src\http_header.c
# End Source File
# Begin Source File

SOURCE=..\src\http_hsm.c
# End Source File
# Begin Source File

SOURCE=..\src\http_request.c
# End Source File
# Begin Source File

SOURCE=..\src\http_response.c
# End Source File
# Begin Source File

SOURCE=..\src\http_serialize.c
# End Source File
# Begin Source File

SOURCE=..\src\http_session.c
# End Source File
# Begin Source File

SOURCE=..\src\http_signal.c
# End Source File
# Begin Source File

SOURCE=..\src\http_util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\http_api.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_auth.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_cache.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_cache_api.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_chunk.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_cookie_api.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_date_parser.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_event.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_header.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_hsm.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_request.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_response.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_serialize.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_session.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_signal.h
# End Source File
# Begin Source File

SOURCE=..\inc\http_util.h
# End Source File
# Begin Source File

SOURCE=..\inc\ssl_interface.h
# End Source File
# End Group
# End Target
# End Project
