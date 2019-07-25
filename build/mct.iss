; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

#define appVersion "0.0.1.6"
#define appPublisher "MCT-MRX-T4"

[Setup]
AppName=MCT-MRX-T4
AppVersion={#appVersion}
AppPublisher={#appPublisher}
DefaultDirName={pf}\MCT-MRX-T4
DefaultGroupName=MEGAROBO Technology
;UninstallDisplayIcon={app}\MCT
Compression=lzma2

SolidCompression=yes
OutputBaseFilename= "MCT-MRX-T4 {#appVersion}"

PrivilegesRequired= admin

;OutputDir=userdocs:Inno Setup Examples Output

[Files]
Source: "*.exe"; DestDir: "{app}"

Source: "*.dll"; DestDir: "{app}"
Source: "*.conf"; DestDir: "{app}"

Source: "translate\*"; DestDir: "{app}\translate"
Source: "style\*"; DestDir: "{app}\style"
Source: "help\*"; DestDir: "{app}\help"; Flags: recursesubdirs 
Source: "license\*"; DestDir: "{app}\license"

Source: "plugins\imageformats\*"; DestDir: "{app}\plugins\imageformats"
Source: "plugins\platforms\*"; DestDir: "{app}\plugins\platforms"
;Source: "plugins\sqldrivers\*"; DestDir: "{app}\plugins\sqldrivers"
;Source: "plugins\mediaservice\*"; DestDir: "{app}\plugins\mediaservice"
;Source: "plugins\printsupport\*"; DestDir: "{app}\plugins\printsupport"
Source: "plugins\styles\*"; DestDir: "{app}\plugins\styles" 

Source: "doc\*"; DestDir: "{app}\doc";  Flags: recursesubdirs 
;Source: "C:\Users\lwq\AppData\Roaming\mct\*"; DestDir: "{userappdata}\mct"; Flags: recursesubdirs   

Source: "MRQ_Update\*"; DestDir: "{app}\MRQ_Update"

;Source: "NetFramework\*"; DestDir: "{app}\NetFramework"

[Dirs]
;Name: "{app}\temp"

;Type: filesandordirs; Name: "{app}\temp"
[UninstallDelete]
Type: files; Name: "{app}\*.xml"

[Run]
;Filename: "{app}\NetFramework\NetFramework 4.5.exe"; Description: "NETFramework 4.5"; Parameters: ""; Flags: postinstall runascurrentuser
Filename: "{app}\doc\readme.txt"; Description: "View the readme file"; Flags: postinstall shellexec skipifsilent runascurrentuser waituntilterminated

[UninstallRun] 
;Filename: "{app}\uregsrv.bat"; Parameters: "{app}"; Flags: runascurrentuser

[Tasks]
Name: "desktopicon"; Description: "Create a desktop icon"; GroupDescription: "Additional Icons"; 

[Icons]
Name: "{commondesktop}\MCT-MRX-T4"; Filename: "{app}\mct.exe"; Tasks: desktopicon

Name: "{group}\MCT-MRX-T4"; Filename: "{app}\mct.exe"
Name: "{group}\Uninstall MCT-MRX-T4"; Filename: "{uninstallexe}"