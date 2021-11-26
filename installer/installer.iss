; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Music X Neo"
#define MyAppVersion "0.3"
#define MyAppPublisher "NOVATRON, Inc."
#define MyAppURL "https://www.cocktailaudio.com/home/"
#define MyAppExeName "CaX.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{4A95CB08-A29C-48BC-BAAB-2830ECA6EC7E}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
;LicenseFile=d:\03-Project\CaX\installer\doc\license.txt
;InfoBeforeFile=d:\03-Project\CaX\installer\doc\readme.txt
;InfoAfterFile=d:\03-Project\CaX\installer\doc\changelog.txt
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
ArchitecturesInstallIn64BitMode=x64
OutputDir=d:\03-Project\CaX\installer\release
OutputBaseFilename=MusicXNeo-x64-{#MyAppVersion}-Setup
SetupIconFile=d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-256.ico
;WizardImageFile=compiler:wizmodernimage-IS.bmp
;WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp
WizardImageFile=d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c164.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c192.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c246.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c273.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c328.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c355.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-c410.bmp
WizardSmallImageFile=d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-55.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-64.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-83.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-92.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-110.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-119.bmp,d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-138.bmp
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "korean"; MessagesFile: "compiler:Languages\Korean.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "d:\03-Project\CaX\installer\src\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Positioning.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Qml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Quick.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5QuickWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5SerialPort.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5WebChannel.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5WebEngineCore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5WebEngineWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\QtWebEngineProcess.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "d:\03-Project\CaX\installer\src\bearer\*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\position\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\printsupport\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\resources\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "d:\03-Project\CaX\installer\src\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "d:\03-Project\CaX\installer\resource\logo-icon-musicxneo-256.ico"; DestDir: "{app}"; Flags: ignoreversion
; VC++ redistributable runtime. Extracted by VC2017RedistNeedsInstall(), if needed.
Source: "d:\03-Project\CaX\installer\Redist\vc_redist.x64.exe"; DestDir: {tmp}; Flags: dontcopy
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[UninstallRun]
Filename: "{app}\unins000.exe"; Parameters: "/quiet"; Flags: waituntilterminated

[Run]
Filename: "{tmp}\vc_redist.x64.exe"; Parameters: "/quiet"; Check: VC2017RedistNeedsInstall ; Flags: waituntilterminated
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function VC2017RedistNeedsInstall: Boolean;
var 
  Version: String;
begin
  if RegQueryStringValue(HKEY_LOCAL_MACHINE,
       'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Version', Version) then
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
    ExtractTemporaryFile('vc_redist.x64.exe');
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
