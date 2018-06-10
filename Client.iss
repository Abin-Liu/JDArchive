; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "金都资料库"
#define MyAppVersion "1.32"
#define MyAppPublisher "无锡金都机械设备有限公司"
#define MyAppURL "http://www.jindu-wx.com/"
#define MyAppExeName "JDArchive.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{893B2A40-DE97-444A-A7BC-F8989D447B5C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppCopyright=Copyright (C) 2011 Abin, {#MyAppPublisher}
LicenseFile=EULA.rtf
InfoAfterFile=History.rtf

DefaultDirName={reg:HKLM\Software\Abin\JDArchive,Install Path|{pf}\JDArchive}
DefaultGroupName={#MyAppName}
OutputDir=Distribute
OutputBaseFilename=JDArchive_Setup {#MyAppVersion}
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "JDArchive.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "LBNetwork.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "LBClient.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "EULA.rtf"; DestDir: "{app}"; Flags: ignoreversion
Source: "History.rtf"; DestDir: "{app}"; Flags: ignoreversion
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]Name: "{commonprograms}\{#MyAppName}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{commonprograms}\{#MyAppName}\使用手册"; Filename: "{app}\JDArchive.chm"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "运行{#MyAppName}"; Flags: nowait postinstall

[Registry]
Root: HKLM; Subkey: "Software\Abin"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Abin\JDArchive"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\Abin\JDArchive"; ValueType: string; ValueName: "Install Path"; ValueData: "{app}"

[Code]
var
IsRunning: Integer;
// 安装时判断客户端是否正在运行
function InitializeSetup(): Boolean;begin
	Result :=true; //安装程序继续
	IsRunning:=FindWindowByClassName('JDArchive');
	while IsRunning<>0 do
	begin
		if Msgbox('安装程序检测到{#MyAppName}正在运行。' #13#13 '您必须先关闭它然后单击“重试”继续安装。', mbConfirmation, MB_RETRYCANCEL) = idCancel then
		begin
			Result :=false; //安装程序退出
			IsRunning :=0;
		end
		else
		begin
			Result :=true; //安装程序继续
			IsRunning:=FindWindowByClassName('JDArchive');
		end;
	end;
end;

// 卸载时判断客户端是否正在运行
function InitializeUninstall(): Boolean;
begin
	Result :=true; //卸载程序继续
	IsRunning:=FindWindowByClassName('JDArchive');
	while IsRunning<>0 do
	begin
		if Msgbox('卸载程序检测到{#MyAppName}正在运行。' #13#13 '您必须先关闭它然后单击“重试”继续卸载。', mbConfirmation, MB_RETRYCANCEL) = idCancel then
		begin
			Result :=false; //卸载程序退出
			IsRunning :=0;
		end
		else
		begin
			Result :=true; //卸载程序继续
			IsRunning:=FindWindowByClassName('JDArchive');
		end;
	end;
end;