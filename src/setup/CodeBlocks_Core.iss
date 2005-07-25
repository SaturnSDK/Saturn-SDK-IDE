; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Code::Blocks
AppVerName=Code::Blocks 1.0-RC1
AppPublisher=Code::Blocks
DefaultDirName={pf}\CodeBlocks
DefaultGroupName=CodeBlocks
LicenseFile=..\COPYING
OutputBaseFilename=CodeBlocks-1.0-RC1
AppPublisherURL=www.codeblocks.org
AppVersion=1.0-RC1
UninstallDisplayIcon={app}\codeblocks.exe
UninstallDisplayName=Code::Blocks
ShowLanguageDialog=no
AppSupportURL=www.codeblocks.org
AppUpdatesURL=www.codeblocks.org
VersionInfoVersion=1.0
VersionInfoDescription=Code::Blocks IDE
WizardImageFile=setup.bmp
WizardSmallImageFile=setupsmall.bmp
InfoAfterFile=COMPILERS_win32.rtf
InternalCompressLevel=ultra
SolidCompression=true
Compression=lzma/ultra
InfoBeforeFile=

[Tasks]
; NOTE: The following entry contains English phrases ("Create a desktop icon" and "Additional icons"). You are free to translate them into another language if required.
Name: desktopicon; Description: Create a &desktop icon; GroupDescription: Additional icons:
; NOTE: The following entry contains English phrases ("Create a Quick Launch icon" and "Additional icons"). You are free to translate them into another language if required.
Name: quicklaunchicon; Description: Create a &Quick Launch icon; GroupDescription: Additional icons:

[Files]
Source: ..\output\codeblocks.exe; DestDir: {app}; Flags: ignoreversion; Components: ProgramFiles
Source: ..\output\codeblocks.dll; DestDir: {app}; Components: ProgramFiles
Source: codeblocks.exe.manifest; DestDir: {app}; Components: ProgramFiles
Source: mingwm10.dll; DestDir: {app}; Components: ProgramFiles
Source: wxmsw242.dll; DestDir: {app}; Components: ProgramFiles
Source: ..\output\exchndl.dll; DestDir: {app}; Components: ProgramFiles
Source: ..\output\console_runner.exe; DestDir: {app}; Components: ProgramFiles
Source: ..\AUTHORS; DestDir: {app}; DestName: AUTHORS.txt; Components: ProgramFiles
Source: ..\COPYING; DestDir: {app}; DestName: COPYING.txt; Components: ProgramFiles
Source: ..\README; DestDir: {app}; DestName: README.txt; Components: ProgramFiles
Source: ..\tips.txt; DestDir: {app}; Components: ProgramFiles
Source: ..\ChangeLog; DestDir: {app}; Components: ProgramFiles
Source: WebSite.url; DestDir: {app}; Components: ProgramFiles
Source: Forums.url; DestDir: {app}; Components: ProgramFiles
Source: WiKi.url; DestDir: {app}; Components: ProgramFiles
Source: ReportBugs.url; DestDir: {app}; Components: ProgramFiles
Source: Download_BCC55.url; DestDir: {app}; Components: ProgramFiles
Source: Download_DMC.url; DestDir: {app}; Components: ProgramFiles
Source: Download_MSVC2003.url; DestDir: {app}; Components: ProgramFiles
Source: Download_MINGW.url; DestDir: {app}; Components: ProgramFiles
Source: Download_OW.url; DestDir: {app}; Components: ProgramFiles
Source: COMPILERS_win32.rtf; DestDir: {app}; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\resources.zip; DestDir: {app}\share\CodeBlocks; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\start_here.zip; DestDir: {app}\share\CodeBlocks; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\astyle.zip; DestDir: {app}\share\CodeBlocks; Components: AStyleFormatter
Source: ..\output\share\CodeBlocks\class_wizard.zip; DestDir: {app}\share\CodeBlocks; Components: ClassWizard
Source: ..\output\share\CodeBlocks\code_completion.zip; DestDir: {app}\share\CodeBlocks; Components: CodeCompletion
Source: ..\output\share\CodeBlocks\compiler_gcc.zip; DestDir: {app}\share\CodeBlocks; Components: CompilerGCC
Source: ..\output\share\CodeBlocks\debugger_gdb.zip; DestDir: {app}\share\CodeBlocks; Components: DebuggerGDB
Source: ..\output\share\CodeBlocks\defaultmimehandler.zip; DestDir: {app}\share\CodeBlocks; Components: DefMimeHandler
Source: ..\output\share\CodeBlocks\manager_resources.zip; DestDir: {app}\share\CodeBlocks; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\plugin_wizard.zip; DestDir: {app}\share\CodeBlocks; Components: PluginWizard
Source: ..\output\share\CodeBlocks\todo.zip; DestDir: {app}\share\CodeBlocks; Components: ToDo
Source: ..\output\share\CodeBlocks\devpakupdater.zip; DestDir: {app}\share\CodeBlocks; Components: DevPak
Source: ..\output\share\CodeBlocks\plugins\astyle.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: AStyleFormatter
Source: ..\output\share\CodeBlocks\plugins\classwizard.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: ClassWizard
Source: ..\output\share\CodeBlocks\plugins\codecompletion.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: CodeCompletion
Source: ..\output\share\CodeBlocks\plugins\compilergcc.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: CompilerGCC
Source: ..\output\share\CodeBlocks\plugins\debuggergdb.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: DebuggerGDB
Source: ..\output\share\CodeBlocks\plugins\defaultmimehandler.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: DefMimeHandler
Source: ..\output\share\CodeBlocks\plugins\pluginwizard.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: PluginWizard
Source: ..\output\share\CodeBlocks\plugins\xpmanifest.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: XPManifest
Source: ..\output\share\CodeBlocks\plugins\todo.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: ToDo
Source: ..\output\share\CodeBlocks\plugins\devpakupdater.dll; DestDir: {app}\share\CodeBlocks\plugins; Components: DevPak
Source: ..\output\share\CodeBlocks\images\ascii.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\codeblocks.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\compile.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\compilerun.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\contents_16x16.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\dbgnext.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\dbgrun.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\dbgrunto.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\dbgstep.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\edit_16x16.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\editcopy.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\editcut.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\editpaste.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\filefind.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\filenew.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\fileopen.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\filesave.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\filesaveas.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\flag_16x16.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\folder.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\folder_new.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\folder_open.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\gohome.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\misc_16x16.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\modified_file.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\newproject.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\rebuild.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\redo.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\run.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\searchreplace.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\source.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\splash.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\stop.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\output\share\CodeBlocks\images\undo.png; DestDir: {app}\share\CodeBlocks\images; Components: ProgramFiles
Source: ..\plugins\codecompletion\resources\images\class.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\class_folder.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\ctor_private.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\ctor_protected.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\ctor_public.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\dtor_private.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\dtor_protected.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\dtor_public.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\enum.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\enumerator.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\enums_folder.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\method_private.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\method_protected.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\method_public.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\namespace.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\others_folder.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\preproc.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\preproc_folder.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\symbols_folder.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\var_private.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\var_protected.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\plugins\codecompletion\resources\images\var_public.png; DestDir: {app}\share\CodeBlocks\images\codecompletion; Components: CodeCompletion
Source: ..\templates\common\console.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\console.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\console.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\console-main-c.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\console-main-cpp.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\dll.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\ogre_gcc.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\ogre_vctk.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\ogrelogo.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\ogre-main.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\qt.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\qt.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\qtlogo32.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\qt-main.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\sdl.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\sdlapp.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\sdl-cb.bmp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\sdl-main.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\staticlib.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\staticlib.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\staticlib-sample.c; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wx-app-ash.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wx-app-ash.h; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wx-main-ash.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wx-main-ash.h; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wx-main-s.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wx-main-sh.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wx-main-sh.h; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\common\wxwidgets.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\dll.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\dll.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\dll-main.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\gui.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\ogre.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\opengl.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\opengl.png; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\opengl.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\opengl-main.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\sdlapp.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\win32.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\win32gui.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\win32-main.cpp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\wxwidgets.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\wxwidgets.template; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\templates\win32\wxwidgets_static.cbp; DestDir: {app}\share\CodeBlocks\templates; Components: Templates
Source: ..\sdk\resources\lexers\lexer_cpp.sample; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_cpp.xml; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_gm.sample; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_gm.xml; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_hitasm.sample; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_hitasm.xml; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_lua.sample; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_lua.xml; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_rc.sample; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_rc.xml; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_xml.sample; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles
Source: ..\sdk\resources\lexers\lexer_xml.xml; DestDir: {app}\share\CodeBlocks\lexers; Components: ProgramFiles

[Icons]
Name: {group}\CodeBlocks; Filename: {app}\codeblocks.exe; IconIndex: 0; WorkingDir: {app}; Comment: Code::Blocks IDE; Components: ProgramFiles
Name: {userdesktop}\CodeBlocks; Filename: {app}\codeblocks.exe; Tasks: desktopicon; IconIndex: 0; WorkingDir: {app}; Comment: Code::Blocks IDE; Components: ProgramFiles
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\CodeBlocks; Filename: {app}\codeblocks.exe; Tasks: quicklaunchicon; IconIndex: 0; WorkingDir: {app}; Comment: Code::Blocks IDE; Components: ProgramFiles
Name: {group}\Links\CodeBlocks on-line; Filename: {app}\WebSite.url; Comment: Go to Code::Blocks IDE website; Components: ProgramFiles
Name: {group}\Links\CodeBlocks forums; Filename: {app}\Forums.url; Comment: Go to Code::Blocks IDE discussion forums; Components: ProgramFiles
Name: {group}\Links\CodeBlocks WiKi; Filename: {app}\WiKi.url; Comment: Go to Code::Blocks IDE WiKi site; Components: ProgramFiles
Name: {group}\Docs\License; Filename: {app}\COPYING.txt; Components: ProgramFiles
Name: {group}\Links\Report a bug; Filename: {app}\ReportBugs.url; Comment: Report bugs you 've found in Code::Blocks; Components: ProgramFiles
Name: {group}\Links\Download MinGW Compiler & Debugger; Filename: {app}\Download_MINGW.url; Comment: Download the MinGW distribution which contains the GNU GCC compiler and GDB debugger; Components: ProgramFiles
Name: {group}\Links\Download Microsoft Visual C++ Free Toolkit 2003; Filename: {app}\Download_MSVC2003.url; Comment: Download the Microsoft Visual C++ Free Toolkit 2003; Components: ProgramFiles
Name: {group}\Links\Download Borland C++ Compiler 5.5; Filename: {app}\Download_BCC55.url; Comment: Download the Borland C++ Compiler 5.5; Components: ProgramFiles
Name: {group}\Links\Download Digital Mars Compiler; Filename: {app}\Download_DMC.url; Comment: Download the Digital Mars free compiler; Components: ProgramFiles
Name: {group}\Links\Download OpenWatcom; Filename: {app}\Download_OW.url; Comment: Download the OpenWatcom portable compiler; Components: ProgramFiles
Name: {group}\Docs\How to install a compiler; Filename: {app}\COMPILERS_win32.rtf

[Run]
; NOTE: The following entry contains an English phrase ("Launch"). You are free to translate it into another language if required.
Filename: {app}\codeblocks.exe; Description: Launch Code::Blocks; Flags: nowait postinstall skipifsilent

[Components]
Name: ProgramFiles; Description: Required program files; Types: custom compact full; Flags: fixed
Name: ClassWizard; Description: Class wizard plugin; Types: custom full
Name: CodeCompletion; Description: Code completion / Class browser plugin; Types: custom full
Name: CompilerGCC; Description: Compiler plugin; Types: custom compact full
Name: DebuggerGDB; Description: GDB debugger plugin; Types: custom compact full
Name: PluginWizard; Description: Code::Blocks Plugin wizard plugin; Types: custom full
Name: ToDo; Description: To-Do List plugin; Types: custom full
Name: XPManifest; Description: WindowsXP Manifest plugin; Types: custom full
Name: Templates; Description: Project templates; Types: custom full
Name: AStyleFormatter; Description: Astyle code formatter plugin; Types: custom full
Name: DefMimeHandler; Description: Default MIME handler; Types: custom compact full
Name: DevPak; Description: DevPaks support plugin; Types: custom

[UninstallRun]
Filename: {app}\codeblocks.exe; Parameters: --clear-configuration; WorkingDir: {app}
