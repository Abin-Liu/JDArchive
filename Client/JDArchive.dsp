# Microsoft Developer Studio Project File - Name="JDArchive" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=JDArchive - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JDArchive.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JDArchive.mak" CFG="JDArchive - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JDArchive - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "JDArchive - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JDArchive - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "ZipArchive" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../JDArchive.exe"

!ELSEIF  "$(CFG)" == "JDArchive - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "ZipArchive" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../JDArchive.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "JDArchive - Win32 Release"
# Name "JDArchive - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClientListView.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DatabaseCache.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadTreeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\JDArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\JDArchive.rc
# End Source File
# Begin Source File

SOURCE=.\JDClientSession.cpp
# End Source File
# Begin Source File

SOURCE=.\JDImgeList.cpp
# End Source File
# Begin Source File

SOURCE=.\JDPacketMap.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrmPriviledges.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrmServerResponse.cpp
# End Source File
# Begin Source File

SOURCE=.\MainfrmTasks.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClientListView.h
# End Source File
# Begin Source File

SOURCE=.\ClientTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DatabaseCache.h
# End Source File
# Begin Source File

SOURCE=.\DownloadTreeManager.h
# End Source File
# Begin Source File

SOURCE=.\JDArchive.h
# End Source File
# Begin Source File

SOURCE=..\JDArchiveCommDef.h
# End Source File
# Begin Source File

SOURCE=.\JDClientSession.h
# End Source File
# Begin Source File

SOURCE=.\JDImgeList.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\admin.ico
# End Source File
# Begin Source File

SOURCE=.\res\bmp.ico
# End Source File
# Begin Source File

SOURCE=.\res\busy.ico
# End Source File
# Begin Source File

SOURCE=.\res\hicolorbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hicolorbarmain.bmp
# End Source File
# Begin Source File

SOURCE=.\res\htm.ico
# End Source File
# Begin Source File

SOURCE=.\res\info.ico
# End Source File
# Begin Source File

SOURCE=.\res\installp.ico
# End Source File
# Begin Source File

SOURCE=.\res\JDArchive.ico
# End Source File
# Begin Source File

SOURCE=.\res\JDArchive.rc2
# End Source File
# Begin Source File

SOURCE=.\res\login.ico
# End Source File
# Begin Source File

SOURCE=.\res\nouser.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TreeBusy.ico
# End Source File
# Begin Source File

SOURCE=.\res\TreeFolderClose.ico
# End Source File
# Begin Source File

SOURCE=.\res\TreeFolderOpen.ico
# End Source File
# Begin Source File

SOURCE=.\res\TreeServerConnect.ico
# End Source File
# Begin Source File

SOURCE=.\res\TreeServerDisconnect.ico
# End Source File
# Begin Source File

SOURCE=.\res\TreeUnknown.ico
# End Source File
# Begin Source File

SOURCE=.\res\txt.ico
# End Source File
# Begin Source File

SOURCE=.\res\update.ico
# End Source File
# Begin Source File

SOURCE=.\res\user.ico
# End Source File
# End Group
# Begin Group "Reusable"

# PROP Default_Filter ""
# Begin Group "CNewMenu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewMenu\MyTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewMenu.h
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewMenuBar.h
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu\NewToolBar.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BrowseCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\FlatSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=..\LBClient.h
# End Source File
# Begin Source File

SOURCE=.\LBListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LBListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\LBNetwork.h
# End Source File
# Begin Source File

SOURCE=.\LBStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\LBStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\MD5.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5.h
# End Source File
# Begin Source File

SOURCE=.\PersistFrameImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\PersistFrameImpl.h
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TextToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TextToolBar.h
# End Source File
# Begin Source File

SOURCE=.\TooltipTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\TooltipTreeView.h
# End Source File
# Begin Source File

SOURCE=.\WinAppEx.cpp
# End Source File
# Begin Source File

SOURCE=.\WinAppEx.h
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddRoleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddRoleDlg.h
# End Source File
# Begin Source File

SOURCE=.\CatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CatDlg.h
# End Source File
# Begin Source File

SOURCE=.\CatInitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CatInitDlg.h
# End Source File
# Begin Source File

SOURCE=.\CatPrivDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CatPrivDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChangePwdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePwdDlg.h
# End Source File
# Begin Source File

SOURCE=.\DownloadTreeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadTreeDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\PatchConfirmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchConfirmDlg.h
# End Source File
# Begin Source File

SOURCE=.\PatchUploadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchUploadDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\RecoverDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecoverDlg.h
# End Source File
# Begin Source File

SOURCE=.\RecoverListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecoverListDlg.h
# End Source File
# Begin Source File

SOURCE=.\RoleListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RoleListDlg.h
# End Source File
# Begin Source File

SOURCE=.\ServerCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\UploadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserListDlg.h
# End Source File
# Begin Source File

SOURCE=.\VerManageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VerManageDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\LBNetwork.lib
# End Source File
# Begin Source File

SOURCE=..\LBClient.lib
# End Source File
# End Target
# End Project
