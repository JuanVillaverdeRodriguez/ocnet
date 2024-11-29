[Setup]
AppId={{156ECD98-BFA3-4A04-94CE-8F9EB4920824}
Uninstallable=no
AppName=Ocnet
AppVersion=1.0
DefaultDirName={commoncf}\VST3
DefaultGroupName=Ocnet
OutputDir=..\
ArchitecturesInstallIn64BitMode=x64
OutputBaseFilename=Ocnet_Installer
DirExistsWarning=no

[Files]
Source: ".\Ocnet.vst3"; DestDir: "{commoncf}\VST3"; Flags: ignoreversion
Source: ".\Ocnet\*"; DestDir: "{userdocs}\Ocnet"; Flags: ignoreversion createallsubdirs recursesubdirs