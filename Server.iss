; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "金都资料库服务器"
#define MyAppVersion "2.12"
#define MyAppPublisher "无锡金都机械设备有限公司"
#define MyAppURL "http://www.jindu-wx.com/"
#define MyAppExeName "JDArchiveSvrCfg.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{B2A91DAB-703F-4300-92BD-6C9A26299A75}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppCopyright=Copyright (C) 2011 Abin, {#MyAppPublisher}

DefaultDirName={reg:HKLM\Software\Abin\JDArchiveSvr,Install Path|{pf}\JDArchiveSvr}
DefaultGroupName={#MyAppName}
OutputDir=Distribute
OutputBaseFilename=JDArchiveSvr_Setup {#MyAppVersion}
Compression=lzma
SolidCompression=yes 

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "JDArchiveSvr.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "LBNetwork.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "LBServer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "ServerConfig.ini"; DestDir: "{app}"; Flags: onlyifdoesntexist uninsneveruninstall
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{userprograms}\{#MyAppName}\服务器设置"; Filename: "{app}\{#MyAppExeName}"
Name: "{userprograms}\{#MyAppName}\启动服务"; Filename: "net.exe"; Parameters: "start JDArchiveSvr"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 2
Name: "{userprograms}\{#MyAppName}\停止服务"; Filename: "net.exe"; Parameters: "stop JDArchiveSvr"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 3
Name: "{userprograms}\{#MyAppName}\查看调试日志"; Filename: "notepad.exe"; Parameters: "{app}\Service.log"; WorkingDir: "{app}"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 1

[Run]
Filename: "{app}\JDArchiveSvr.exe"; Parameters: "-install"; WorkingDir: "{app}"; StatusMsg: "正在注册服务..."; Flags: waituntilterminated runhidden
Filename: "{app}\{#MyAppExeName}"; Parameters: "-silent"; WorkingDir: "{app}"; Flags: waituntilterminated hidewizard
Filename: "net.exe"; Parameters: "start JDArchiveSvr"; StatusMsg: "正在启动服务..."; Flags: waituntilterminated runhidden

[UninstallRun]
Filename: "net.exe"; Parameters: "stop JDArchiveSvr"; StatusMsg: "正在停止服务..."; Flags: waituntilterminated runhidden
Filename: "{app}\JDArchiveSvr.exe"; Parameters: "-uninstall"; WorkingDir: "{app}"; StatusMsg: "正在注销服务..."; Flags: waituntilterminated runhidden

[Registry]
Root: HKLM; Subkey: "Software\Abin";
Root: HKLM; Subkey: "Software\Abin\JDArchiveSvr";
Root: HKLM; Subkey: "Software\Abin\JDArchiveSvr"; ValueType: string; ValueName: "Install Path"; ValueData: "{app}";

[Code]
function InitializeSetup(): boolean;
var
ResultStr: String;
ResultCode: Integer;
begin
	Result := true;
	if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{B2A91DAB-703F-4300-92BD-6C9A26299A75}_is1', 'UninstallString', ResultStr) then
	begin
		ResultStr := RemoveQuotes(ResultStr);
		if (MsgBox('安装程序必须先卸载旧版本才能继续，点击“确定”进行卸载，或点击“取消”退出安装。', mbConfirmation, MB_OKCANCEL) = idOK) then
		begin
			Exec(ResultStr, '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);			
		end
		else
		begin
			Result := false;
		end;
	end;
end;

