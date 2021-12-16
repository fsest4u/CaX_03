; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Music X Neo"
#define MyAppVersion "0.8"
#define MyAppPublisher "NOVATRON, Inc."
#define MyAppURL "https://www.cocktailaudio.com/home/"
#define MyAppExeName "CaX.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{0681B21F-2557-4024-959C-16A4E3B09491}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={localappdata}\{#MyAppName}-x86
;LicenseFile=d:\03-Project\CaX\installer\doc\license.txt
;InfoBeforeFile=d:\03-Project\CaX\installer\doc\readme.txt
;InfoAfterFile=d:\03-Project\CaX\installer\doc\changelog.txt
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
;ArchitecturesAllowed=x86
OutputDir=D:\03-Project\CaX\installer\release
OutputBaseFilename=MusicXNeo-{#MyAppVersion}-Setup-x86
SetupIconFile=D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-256.ico
WizardImageFile=D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c164.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c192.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c246.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c273.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c328.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c355.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c410.bmp
WizardSmallImageFile=D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-55.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-64.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-83.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-92.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-110.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-119.bmp,D:\03-Project\CaX\installer\resource\logo-icon-musicxneo-138.bmp
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "korean"; MessagesFile: "compiler:Languages\Korean.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; 

[Files]
Source: "D:\03-Project\CaX\installer\src-x86\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Positioning.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Qml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Quick.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5QuickWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5SerialPort.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5WebChannel.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5WebEngineCore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5WebEngineWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\QtWebEngineProcess.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\03-Project\CaX\installer\src-x86\bearer\*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\position\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\printsupport\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\resources\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "D:\03-Project\CaX\installer\src-x86\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs
; VC++ redistributable runtime. Extracted by VC2017RedistNeedsInstall(), if needed.
Source: "d:\03-Project\CaX\installer\Redist\vc_redist.x86.exe"; DestDir: {tmp}; Flags: dontcopy
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Run]
Filename: "{tmp}\vc_redist.x86.exe"; Parameters: "/quiet"; Check: VC2017RedistNeedsInstall ; Flags: waituntilterminated
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function VC2017RedistNeedsInstall: Boolean;
var 
  Version: String;
begin
  if RegQueryStringValue(HKEY_LOCAL_MACHINE,
       'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86', 'Version', Version) then
  begin
    // Is the installed version at least 14.14 ? 
    Log('VC Redist Version check : found ' + Version);
    Result := (CompareStr(Version, 'v14.16.27033.0')<0);
  end
  else 
  begin
    // Not even an old version installed
    Result := True;
  end;
  if (Result) then
  begin
    ExtractTemporaryFile('vc_redist.x86.exe');
  end;
end;

{ ///////////////////////////////////////////////////////////////////// }
function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{#emit SetupSetting("AppId")}_is1');
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;


{ ///////////////////////////////////////////////////////////////////// }
function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;


{ ///////////////////////////////////////////////////////////////////// }
function UnInstallOldVersion(): Integer;
var
  sUnInstallString: String;
  iResultCode: Integer;
begin
{ Return Values: }
{ 1 - uninstall string is empty }
{ 2 - error executing the UnInstallString }
{ 3 - successfully executed the UnInstallString }

  { default return value }
  Result := 0;

  { get the uninstall string of the old app }
  sUnInstallString := GetUninstallString();
  if sUnInstallString <> '' then begin
    sUnInstallString := RemoveQuotes(sUnInstallString);
    if Exec(sUnInstallString, '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode) then
      Result := 3
    else
      Result := 2;
  end else
    Result := 1;
end;

{ ///////////////////////////////////////////////////////////////////// }
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep=ssInstall) then
  begin
    if (IsUpgrade()) then
    begin
      UnInstallOldVersion();
    end;
  end;
end;
