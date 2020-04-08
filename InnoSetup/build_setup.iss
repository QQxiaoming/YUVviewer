
#define MyAppName "YUVviewer"
#define MyAppVersion "0.3.3"
#define MyAppPublisher "zhiliao007 <wenqing.li@aliyun.com> QQxiaoming <2014500726@smail.xtu.edu.cn>"
#define MyAppURL "https://github.com/zhiliao007/YUVviewer"
#define MyAppExeName "YUVviewer_setup.exe"
#define MyAppOutputName "YUVviewer_setup"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{4368AAE0-A354-4127-94A9-0C38727B1DCC}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
VersionInfoVersion=0.3.3.000
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
InfoBeforeFile=..\InnoSetup\Info.txt
OutputBaseFilename={#MyAppOutputName}
OutPutdir=..\InnoSetup
SetupIconFile=..\img\ico.ico
Compression=lzma
SolidCompression=yes


[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\InnoSetup\build\YUVviewer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\InnoSetup\build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
