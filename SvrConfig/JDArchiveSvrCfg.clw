; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CJDArchiveSvrCfgDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "JDArchiveSvrCfg.h"

ClassCount=3
Class1=CJDArchiveSvrCfgApp
Class2=CJDArchiveSvrCfgDlg

ResourceCount=3
Resource2=IDD_JDARCHIVESVRCFG_DIALOG
Resource1=IDR_MAINFRAME
Class3=CBusyDlg
Resource3=IDD_BUSYDLG

[CLS:CJDArchiveSvrCfgApp]
Type=0
HeaderFile=JDArchiveSvrCfg.h
ImplementationFile=JDArchiveSvrCfg.cpp
Filter=N

[CLS:CJDArchiveSvrCfgDlg]
Type=0
HeaderFile=JDArchiveSvrCfgDlg.h
ImplementationFile=JDArchiveSvrCfgDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CLEANUPNOW



[DLG:IDD_JDARCHIVESVRCFG_DIALOG]
Type=1
Class=CJDArchiveSvrCfgDlg
ControlCount=29
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_MAXCONN,edit,1350639744
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ROOTPATH,button,1342242816
Control6=IDC_DEBUG,button,1342242819
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_HOST,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_DBNAME,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_USERNAME,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_PASSWORD,edit,1350631584
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_BACKUPPATH,button,1342242816
Control19=IDC_STATIC,static,1342308352
Control20=IDC_INTERVAL,edit,1350639745
Control21=IDC_STATIC,static,1342308352
Control22=IDC_HOUR,edit,1350639745
Control23=IDC_STATIC,static,1342308352
Control24=IDC_AUTODEL,button,1342242819
Control25=IDC_DELDAYS,edit,1350639745
Control26=IDC_STATIC,static,1342308352
Control27=IDC_BACKUPNOW,button,1342242819
Control28=IDOK,button,1342242816
Control29=IDCANCEL,button,1342242816

[DLG:IDD_BUSYDLG]
Type=1
Class=CBusyDlg
ControlCount=1
Control1=IDC_TEXT,static,1342308865

[CLS:CBusyDlg]
Type=0
HeaderFile=BusyDlg.h
ImplementationFile=BusyDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TEXT
VirtualFilter=dWC

