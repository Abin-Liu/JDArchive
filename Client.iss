; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "�����Ͽ�"
#define MyAppVersion "1.32"
#define MyAppPublisher "�����𶼻�е�豸���޹�˾"
#define MyAppURL "http://www.jindu-wx.com/"
#define MyAppExeName "JDArchive.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
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
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]Name: "{commonprograms}\{#MyAppName}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{commonprograms}\{#MyAppName}\ʹ���ֲ�"; Filename: "{app}\JDArchive.chm"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "����{#MyAppName}"; Flags: nowait postinstall

[Registry]
Root: HKLM; Subkey: "Software\Abin"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\Abin\JDArchive"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\Abin\JDArchive"; ValueType: string; ValueName: "Install Path"; ValueData: "{app}"

[Code]
var
IsRunning: Integer;
// ��װʱ�жϿͻ����Ƿ���������
function InitializeSetup(): Boolean;begin
	Result :=true; //��װ�������
	IsRunning:=FindWindowByClassName('JDArchive');
	while IsRunning<>0 do
	begin
		if Msgbox('��װ�����⵽{#MyAppName}�������С�' #13#13 '�������ȹر���Ȼ�󵥻������ԡ�������װ��', mbConfirmation, MB_RETRYCANCEL) = idCancel then
		begin
			Result :=false; //��װ�����˳�
			IsRunning :=0;
		end
		else
		begin
			Result :=true; //��װ�������
			IsRunning:=FindWindowByClassName('JDArchive');
		end;
	end;
end;

// ж��ʱ�жϿͻ����Ƿ���������
function InitializeUninstall(): Boolean;
begin
	Result :=true; //ж�س������
	IsRunning:=FindWindowByClassName('JDArchive');
	while IsRunning<>0 do
	begin
		if Msgbox('ж�س����⵽{#MyAppName}�������С�' #13#13 '�������ȹر���Ȼ�󵥻������ԡ�����ж�ء�', mbConfirmation, MB_RETRYCANCEL) = idCancel then
		begin
			Result :=false; //ж�س����˳�
			IsRunning :=0;
		end
		else
		begin
			Result :=true; //ж�س������
			IsRunning:=FindWindowByClassName('JDArchive');
		end;
	end;
end;