; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "�����Ͽ������"
#define MyAppVersion "2.12"
#define MyAppPublisher "�����𶼻�е�豸���޹�˾"
#define MyAppURL "http://www.jindu-wx.com/"
#define MyAppExeName "JDArchiveSvrCfg.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
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
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{userprograms}\{#MyAppName}\����������"; Filename: "{app}\{#MyAppExeName}"
Name: "{userprograms}\{#MyAppName}\��������"; Filename: "net.exe"; Parameters: "start JDArchiveSvr"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 2
Name: "{userprograms}\{#MyAppName}\ֹͣ����"; Filename: "net.exe"; Parameters: "stop JDArchiveSvr"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 3
Name: "{userprograms}\{#MyAppName}\�鿴������־"; Filename: "notepad.exe"; Parameters: "{app}\Service.log"; WorkingDir: "{app}"; IconFilename: "{app}\{#MyAppExeName}"; IconIndex: 1

[Run]
Filename: "{app}\JDArchiveSvr.exe"; Parameters: "-install"; WorkingDir: "{app}"; StatusMsg: "����ע�����..."; Flags: waituntilterminated runhidden
Filename: "{app}\{#MyAppExeName}"; Parameters: "-silent"; WorkingDir: "{app}"; Flags: waituntilterminated hidewizard
Filename: "net.exe"; Parameters: "start JDArchiveSvr"; StatusMsg: "������������..."; Flags: waituntilterminated runhidden

[UninstallRun]
Filename: "net.exe"; Parameters: "stop JDArchiveSvr"; StatusMsg: "����ֹͣ����..."; Flags: waituntilterminated runhidden
Filename: "{app}\JDArchiveSvr.exe"; Parameters: "-uninstall"; WorkingDir: "{app}"; StatusMsg: "����ע������..."; Flags: waituntilterminated runhidden

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
		if (MsgBox('��װ���������ж�ؾɰ汾���ܼ����������ȷ��������ж�أ�������ȡ�����˳���װ��', mbConfirmation, MB_OKCANCEL) = idOK) then
		begin
			Exec(ResultStr, '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);			
		end
		else
		begin
			Result := false;
		end;
	end;
end;

